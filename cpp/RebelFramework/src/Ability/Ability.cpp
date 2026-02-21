// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#include "Rebel/Ability/Ability.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

namespace Rebel::Ability {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

Ability::Ability() {
    // Pre-populate exactly IMPROVEMENT_COUNT improvement slots so the
    // Godot Inspector always shows a consistent, fully-formed array.
    m_improvements.resize(IMPROVEMENT_COUNT);
    for (int i = 0; i < IMPROVEMENT_COUNT; ++i) {
        Ref<AbilityImprovement> improvement;
        improvement.instantiate();
        improvement->set_level(i + 1); // Levels are 1-indexed
        m_improvements[i] = improvement;
    }
}

// ---------------------------------------------------------------------------
// _bind_methods
// ---------------------------------------------------------------------------

void Ability::_bind_methods() {
    // --- name ---
    ClassDB::bind_method(D_METHOD("set_name", "name"), &Ability::set_name);
    ClassDB::bind_method(D_METHOD("get_name"), &Ability::get_name);

    // --- description ---
    ClassDB::bind_method(D_METHOD("set_description", "description"), &Ability::set_description);
    ClassDB::bind_method(D_METHOD("get_description"), &Ability::get_description);

    // --- icon ---
    ClassDB::bind_method(D_METHOD("set_icon", "icon"), &Ability::set_icon);
    ClassDB::bind_method(D_METHOD("get_icon"), &Ability::get_icon);

    // --- enabled ---
    ClassDB::bind_method(D_METHOD("set_enabled", "enabled"), &Ability::set_enabled);
    ClassDB::bind_method(D_METHOD("get_enabled"), &Ability::get_enabled);

    // --- cost ---
    ClassDB::bind_method(D_METHOD("set_cost", "cost"), &Ability::set_cost);
    ClassDB::bind_method(D_METHOD("get_cost"), &Ability::get_cost);

    // --- improvements ---
    ClassDB::bind_method(D_METHOD("set_improvements", "improvements"), &Ability::set_improvements);
    ClassDB::bind_method(D_METHOD("get_improvements"), &Ability::get_improvements);

    // --- current_level ---
    ClassDB::bind_method(D_METHOD("set_current_level", "level"), &Ability::set_current_level);
    ClassDB::bind_method(D_METHOD("get_current_level"), &Ability::get_current_level);

    // --- helper ---
    ClassDB::bind_method(D_METHOD("get_active_improvement"), &Ability::get_active_improvement);

    ADD_GROUP("Ability", "");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name",          PROPERTY_HINT_NONE),                         "set_name",          "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "description",   PROPERTY_HINT_MULTILINE_TEXT),               "set_description",   "get_description");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "icon",          PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"),   "set_icon",          "get_icon");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL,   "enabled"),                                                   "set_enabled",       "get_enabled");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT,  "cost",          PROPERTY_HINT_RANGE, "0,99999,0.1,or_greater"), "set_cost",       "get_cost");
    ADD_PROPERTY(PropertyInfo(Variant::INT,    "current_level", PROPERTY_HINT_RANGE, "0,10,1"),              "set_current_level", "get_current_level");

    // Expose the improvements array with an element type hint so the inspector
    // knows each slot should be an AbilityImprovement resource.
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "improvements",
                              PROPERTY_HINT_ARRAY_TYPE, "AbilityImprovement"),
                 "set_improvements", "get_improvements");
}

// ---------------------------------------------------------------------------
// Setters / getters
// ---------------------------------------------------------------------------

void Ability::set_name(const String& name) {
    m_name = name;
}

String Ability::get_name() const {
    return m_name;
}

void Ability::set_description(const String& description) {
    m_description = description;
}

String Ability::get_description() const {
    return m_description;
}

void Ability::set_icon(const Ref<Texture2D>& icon) {
    m_icon = icon;
}

Ref<Texture2D> Ability::get_icon() const {
    return m_icon;
}

void Ability::set_enabled(const bool enabled) {
    m_enabled = enabled;
}

bool Ability::get_enabled() const {
    return m_enabled;
}

void Ability::set_cost(const float cost) {
    m_cost = Math::max(0.0f, cost);
}

float Ability::get_cost() const {
    return m_cost;
}

void Ability::set_improvements(const Array& improvements) {
    m_improvements = improvements;
    normalize_improvements();
}

Array Ability::get_improvements() const {
    return m_improvements;
}

void Ability::set_current_level(const int level) {
    m_current_level = Math::clamp(level, 0, IMPROVEMENT_COUNT);
}

int Ability::get_current_level() const {
    return m_current_level;
}

Ref<AbilityImprovement> Ability::get_active_improvement() const {
    if (m_current_level <= 0) {
        return {};
    }
    const int index = m_current_level - 1;
    if (index >= m_improvements.size()) {
        return {};
    }
    return m_improvements[index];
}

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

void Ability::normalize_improvements() {
    // Ensure we always have exactly IMPROVEMENT_COUNT slots.
    // If the incoming array was shorter, pad with default improvements.
    const int current_size = m_improvements.size();
    if (current_size < IMPROVEMENT_COUNT) {
        m_improvements.resize(IMPROVEMENT_COUNT);
        for (int i = current_size; i < IMPROVEMENT_COUNT; ++i) {
            Ref<AbilityImprovement> improvement;
            improvement.instantiate();
            improvement->set_level(i + 1);
            m_improvements[i] = improvement;
        }
    }
    // If larger than IMPROVEMENT_COUNT, truncate silently — the designer
    // added more slots than the system supports.
    if (m_improvements.size() > IMPROVEMENT_COUNT) {
        m_improvements.resize(IMPROVEMENT_COUNT);
    }
}

} // namespace Rebel::Ability
