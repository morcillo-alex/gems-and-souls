// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#include "Rebel/Ability/AbilityImprovement.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

namespace Rebel::Ability {

void AbilityImprovement::_bind_methods() {
    // --- level ---
    ClassDB::bind_method(D_METHOD("set_level", "level"), &AbilityImprovement::set_level);
    ClassDB::bind_method(D_METHOD("get_level"), &AbilityImprovement::get_level);

    // --- description ---
    ClassDB::bind_method(D_METHOD("set_description", "description"), &AbilityImprovement::set_description);
    ClassDB::bind_method(D_METHOD("get_description"), &AbilityImprovement::get_description);

    // --- icon ---
    ClassDB::bind_method(D_METHOD("set_icon", "icon"), &AbilityImprovement::set_icon);
    ClassDB::bind_method(D_METHOD("get_icon"), &AbilityImprovement::get_icon);

    // --- cost ---
    ClassDB::bind_method(D_METHOD("set_cost", "cost"), &AbilityImprovement::set_cost);
    ClassDB::bind_method(D_METHOD("get_cost"), &AbilityImprovement::get_cost);

    ADD_GROUP("Improvement", "");
    ADD_PROPERTY(PropertyInfo(Variant::INT,    "level",       PROPERTY_HINT_RANGE, "1,10,1"),           "set_level",       "get_level");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "description", PROPERTY_HINT_MULTILINE_TEXT),            "set_description", "get_description");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "icon",        PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_icon",        "get_icon");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT,  "cost",        PROPERTY_HINT_RANGE, "0,99999,0.1,or_greater"), "set_cost", "get_cost");
}

// --- level ---

void AbilityImprovement::set_level(const int level) {
    m_level = Math::clamp(level, 1, 10);
}

int AbilityImprovement::get_level() const {
    return m_level;
}

// --- description ---

void AbilityImprovement::set_description(const String& description) {
    m_description = description;
}

String AbilityImprovement::get_description() const {
    return m_description;
}

// --- icon ---

void AbilityImprovement::set_icon(const Ref<Texture2D>& icon) {
    m_icon = icon;
}

Ref<Texture2D> AbilityImprovement::get_icon() const {
    return m_icon;
}

// --- cost ---

void AbilityImprovement::set_cost(const float cost) {
    m_cost = Math::max(0.0f, cost);
}

float AbilityImprovement::get_cost() const {
    return m_cost;
}

} // namespace Rebel::Ability
