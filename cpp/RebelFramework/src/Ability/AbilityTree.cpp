// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#include "Rebel/Ability/AbilityTree.hpp"
#include "Rebel/Ability/AbilityScriptContainerNode.hpp"

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
    ClassDB::bind_method(D_METHOD("try_activate", "node", "parent"), &AbilityTree::try_activate);
    ClassDB::bind_method(D_METHOD("deactivate", "behavior_node"), &AbilityTree::deactivate);

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

// ---------------------------------------------------------------------------
// try_activate
// ---------------------------------------------------------------------------

Node* AbilityTree::try_activate(const Ref<AbilityNode>& node, Node* character) {
    if (!try_unlock(node)) {
        return nullptr;
    }

    if (character == nullptr) {
        return nullptr;
    }

    const Ref<Ability> ability = node->get_ability();

    // Search the character's direct children for the matching container.
    for (int i = 0; i < character->get_child_count(); ++i) {
        AbilityScriptContainerNode* container =
            Object::cast_to<AbilityScriptContainerNode>(character->get_child(i));

        if (container == nullptr) {
            continue;
        }

        if (container->get_ability() == ability) {
            container->on_activated();
            return container;
        }
    }

    // Ability unlocked in data but no container found — passive ability.
    return nullptr;
}

// ---------------------------------------------------------------------------
// deactivate
// ---------------------------------------------------------------------------

void AbilityTree::deactivate(Node* behavior_node) {
    if (behavior_node == nullptr) {
        return;
    }

    if (AbilityScriptContainerNode* container =
            Object::cast_to<AbilityScriptContainerNode>(behavior_node)) {
        container->on_deactivated();
    }
    // The node is part of the character scene — do NOT queue_free().
}

} // namespace Rebel::Ability
