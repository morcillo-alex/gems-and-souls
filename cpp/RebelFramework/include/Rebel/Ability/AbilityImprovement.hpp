// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#pragma once

#include "Rebel/Core.hpp"
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/variant/string.hpp>

namespace Rebel::Ability {

/**
 * @brief Represents one upgrade level for an Ability.
 *
 * An AbilityImprovement describes what changes when an Ability is upgraded
 * to a specific level (1–10). Each level can optionally carry its own icon;
 * if left null the parent Ability's icon is used by convention.
 *
 * Serializable as a .tres file and fully editable in the Godot Inspector.
 *
 * @note Intended to be stored in Ability::improvements (Array, 10 fixed slots).
 */
class REBEL_FRAMEWORK AbilityImprovement : public godot::Resource {
    GDCLASS(AbilityImprovement, godot::Resource);

    /** The upgrade tier this improvement represents (1–10). */
    int m_level{1};

    /** Human-readable description of what this upgrade does. */
    godot::String m_description{};

    /**
     * @brief Optional icon for this upgrade level.
     *
     * When null the UI should fall back to the parent Ability's icon.
     */
    godot::Ref<godot::Texture2D> m_icon{};

    /** Resource cost to activate/purchase this improvement level. */
    float m_cost{0.0f};

protected:
    static void _bind_methods();

public:
    AbilityImprovement() = default;

    /**
     * @brief Sets the upgrade level (clamped to 1–10).
     * @param level The upgrade tier.
     */
    void set_level(int level);

    /**
     * @brief Returns the upgrade level.
     * @return Integer in the range 1–10.
     */
    [[nodiscard]] int get_level() const;

    /**
     * @brief Sets the human-readable description for this upgrade.
     * @param description Text shown in the UI for this improvement.
     */
    void set_description(const godot::String& description);

    /**
     * @brief Returns the description of this upgrade.
     * @return Description string.
     */
    [[nodiscard]] godot::String get_description() const;

    /**
     * @brief Sets the optional icon for this upgrade level.
     *
     * Pass a null Ref to indicate that the parent Ability icon should be used.
     *
     * @param icon Texture to display, or null.
     */
    void set_icon(const godot::Ref<godot::Texture2D>& icon);

    /**
     * @brief Returns the icon for this upgrade level.
     * @return Icon texture, or null if falling back to the parent Ability icon.
     */
    [[nodiscard]] godot::Ref<godot::Texture2D> get_icon() const;

    /**
     * @brief Sets the resource cost for this improvement.
     * @param cost Cost value (clamped to >= 0).
     */
    void set_cost(float cost);

    /**
     * @brief Returns the resource cost for this improvement.
     * @return Cost as a float.
     */
    [[nodiscard]] float get_cost() const;
};

} // namespace Rebel::Ability
