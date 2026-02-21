// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#include "Rebel/Ability/AbilityScriptContainerNode.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

namespace Rebel::Ability {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

AbilityScriptContainerNode::AbilityScriptContainerNode() {
    // Dormant by default — AbilityTree enables this node on activation.
    set_process_mode(PROCESS_MODE_DISABLED);
}

// ---------------------------------------------------------------------------
// _bind_methods
// ---------------------------------------------------------------------------

void AbilityScriptContainerNode::_bind_methods() {
    // --- ability ---
    ClassDB::bind_method(D_METHOD("set_ability", "ability"), &AbilityScriptContainerNode::set_ability);
    ClassDB::bind_method(D_METHOD("get_ability"), &AbilityScriptContainerNode::get_ability);

    ADD_GROUP("AbilityScriptContainer", "");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "ability", PROPERTY_HINT_RESOURCE_TYPE, "Ability"),
                 "set_ability", "get_ability");

    GDVIRTUAL_BIND(_on_activated);
    GDVIRTUAL_BIND(_on_deactivated);
}

// ---------------------------------------------------------------------------
// Setters / getters
// ---------------------------------------------------------------------------

void AbilityScriptContainerNode::set_ability(const Ref<Ability>& ability) {
    m_ability = ability;
}

Ref<Ability> AbilityScriptContainerNode::get_ability() const {
    return m_ability;
}

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------

void AbilityScriptContainerNode::on_activated() {
    set_process_mode(PROCESS_MODE_INHERIT);
    GDVIRTUAL_CALL(_on_activated);
}

void AbilityScriptContainerNode::on_deactivated() {
    GDVIRTUAL_CALL(_on_deactivated);
    set_process_mode(PROCESS_MODE_DISABLED);
}

} // namespace Rebel::Ability
