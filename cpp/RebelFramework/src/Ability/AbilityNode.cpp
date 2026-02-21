// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#include "Rebel/Ability/AbilityNode.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

namespace Rebel::Ability {

void AbilityNode::_bind_methods() {
    // --- ability ---
    ClassDB::bind_method(D_METHOD("set_ability", "ability"), &AbilityNode::set_ability);
    ClassDB::bind_method(D_METHOD("get_ability"), &AbilityNode::get_ability);

    // --- prerequisites ---
    ClassDB::bind_method(D_METHOD("set_prerequisites", "prerequisites"), &AbilityNode::set_prerequisites);
    ClassDB::bind_method(D_METHOD("get_prerequisites"), &AbilityNode::get_prerequisites);

    // --- helper ---
    ClassDB::bind_method(D_METHOD("can_unlock"), &AbilityNode::can_unlock);

    ADD_GROUP("AbilityNode", "");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "ability",
                              PROPERTY_HINT_RESOURCE_TYPE, "Ability"),
                 "set_ability", "get_ability");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "prerequisites",
                              PROPERTY_HINT_ARRAY_TYPE, "AbilityNode"),
                 "set_prerequisites", "get_prerequisites");
}

// ---------------------------------------------------------------------------
// Setters / getters
// ---------------------------------------------------------------------------

void AbilityNode::set_ability(const Ref<Ability>& ability) {
    m_ability = ability;
}

Ref<Ability> AbilityNode::get_ability() const {
    return m_ability;
}

void AbilityNode::set_prerequisites(const Array& prerequisites) {
    m_prerequisites = prerequisites;
}

Array AbilityNode::get_prerequisites() const {
    return m_prerequisites;
}

// ---------------------------------------------------------------------------
// can_unlock
// ---------------------------------------------------------------------------

bool AbilityNode::can_unlock() const {
    // A node with no prerequisites is always unlockable (root node).
    if (m_prerequisites.is_empty()) {
        return true;
    }

    for (int i = 0; i < m_prerequisites.size(); ++i) {
        // Cast each prerequisite element to an AbilityNode.
        // Non-AbilityNode elements are treated as unsatisfied prerequisites.
        const Ref<AbilityNode> prereq_node = m_prerequisites[i];
        if (prereq_node.is_null()) {
            return false;
        }

        const Ref<Ability> prereq_ability = prereq_node->get_ability();
        if (prereq_ability.is_null()) {
            return false;
        }

        if (!prereq_ability->get_enabled()) {
            return false;
        }
    }
    return true;
}

} // namespace Rebel::Ability
