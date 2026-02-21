// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#include "Rebel/Ability/AbilityTree.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

namespace Rebel::Ability {

void AbilityTree::_bind_methods() {
    // --- nodes ---
    ClassDB::bind_method(D_METHOD("set_nodes", "nodes"), &AbilityTree::set_nodes);
    ClassDB::bind_method(D_METHOD("get_nodes"), &AbilityTree::get_nodes);

    // --- helpers ---
    ClassDB::bind_method(D_METHOD("get_root_nodes"), &AbilityTree::get_root_nodes);
    ClassDB::bind_method(D_METHOD("try_unlock", "node"), &AbilityTree::try_unlock);

    ADD_GROUP("AbilityTree", "");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "nodes",
                              PROPERTY_HINT_ARRAY_TYPE, "AbilityNode"),
                 "set_nodes", "get_nodes");
}

// ---------------------------------------------------------------------------
// Setters / getters
// ---------------------------------------------------------------------------

void AbilityTree::set_nodes(const Array& nodes) {
    m_nodes = nodes;
}

Array AbilityTree::get_nodes() const {
    return m_nodes;
}

// ---------------------------------------------------------------------------
// get_root_nodes
// ---------------------------------------------------------------------------

Array AbilityTree::get_root_nodes() const {
    Array result{};
    for (int i = 0; i < m_nodes.size(); ++i) {
        const Ref<AbilityNode> node = m_nodes[i];
        if (node.is_null()) {
            continue;
        }
        // A root node has no prerequisites.
        if (node->get_prerequisites().is_empty()) {
            result.append(node);
        }
    }
    return result;
}

// ---------------------------------------------------------------------------
// try_unlock
// ---------------------------------------------------------------------------

bool AbilityTree::try_unlock(const Ref<AbilityNode>& node) {
    if (node.is_null()) {
        return false;
    }

    const Ref<Ability> ability = node->get_ability();
    if (ability.is_null()) {
        return false;
    }

    // Already unlocked — nothing to do.
    if (ability->get_enabled()) {
        return false;
    }

    // Prerequisites not satisfied.
    if (!node->can_unlock()) {
        return false;
    }

    ability->set_enabled(true);
    return true;
}

} // namespace Rebel::Ability
