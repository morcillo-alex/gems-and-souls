// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#pragma once

#include "Rebel/Core.hpp"
#include "Rebel/Ability/AbilityImprovement.hpp"
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>

namespace Rebel::Ability {

/**
 * @brief A single configurable ability that can be unlocked and upgraded.
 *
 * Stores all static data for one ability: identity (name, description, icon),
 * unlock state, unlock cost, and a fixed array of 10 AbilityImprovement slots
 * indexed 0–9 (levels 1–10).
 *
 * The constructor pre-populates all 10 improvement slots so the Godot Inspector
 * always exposes all slots regardless of how many have been filled in.
 *
 * Serializable as a .tres file and fully editable in the Godot Inspector.
 *
 * @note current_level == 0 means no improvement is active.
 *       current_level == N means the improvement at index N-1 is active.
 */
class REBEL_FRAMEWORK Ability : public godot::Resource {
    GDCLASS(Ability, godot::Resource);

    /** Display name of the ability. */
    godot::String m_name{};

    /** Description shown in ability selection UI. */
    godot::String m_description{};

    /** Icon representing this ability in the UI. */
    godot::Ref<godot::Texture2D> m_icon{};

    /** Whether this ability has been unlocked/activated. */
    bool m_enabled{false};

    /** Resource cost to unlock this ability. */
    float m_cost{0.0f};

    /**
     * @brief Fixed array of exactly 10 AbilityImprovement resources.
     *
     * Indexed 0–9, corresponding to upgrade levels 1–10.
     * Pre-populated in the constructor.
     */
    godot::Array m_improvements{};

    /**
     * @brief Currently active improvement level.
     *
     * 0 = no upgrade active. 1–10 = the improvement at index (current_level - 1)
     * is active.
     */
    int m_current_level{0};

protected:
    static void _bind_methods();

public:
    /**
     * @brief Constructor — pre-populates all 10 improvement slots.
     *
     * Ensures the Inspector always shows a full, consistent set of slots even
     * before the designer has authored any improvements.
     */
    Ability();

    /**
     * @brief Sets the display name of this ability.
     * @param name Ability name string.
     */
    void set_name(const godot::String& name);

    /**
     * @brief Returns the display name of this ability.
     * @return Name string.
     */
    [[nodiscard]] godot::String get_name() const;

    /**
     * @brief Sets the description of this ability.
     * @param description Human-readable description.
     */
    void set_description(const godot::String& description);

    /**
     * @brief Returns the description of this ability.
     * @return Description string.
     */
    [[nodiscard]] godot::String get_description() const;

    /**
     * @brief Sets the icon for this ability.
     * @param icon Texture displayed in the UI.
     */
    void set_icon(const godot::Ref<godot::Texture2D>& icon);

    /**
     * @brief Returns the icon for this ability.
     * @return Icon texture.
     */
    [[nodiscard]] godot::Ref<godot::Texture2D> get_icon() const;

    /**
     * @brief Sets whether this ability is unlocked/active.
     * @param enabled True if the ability is unlocked.
     */
    void set_enabled(bool enabled);

    /**
     * @brief Returns whether this ability is unlocked/active.
     * @return True if enabled.
     */
    [[nodiscard]] bool get_enabled() const;

    /**
     * @brief Sets the resource cost to unlock this ability.
     * @param cost Unlock cost (clamped to >= 0).
     */
    void set_cost(float cost);

    /**
     * @brief Returns the resource cost to unlock this ability.
     * @return Cost as a float.
     */
    [[nodiscard]] float get_cost() const;

    /**
     * @brief Sets the improvements array.
     *
     * The array must contain exactly 10 elements of type AbilityImprovement.
     * Extra elements are ignored; if fewer than 10 are provided, the missing
     * slots are filled with default AbilityImprovement instances.
     *
     * @param improvements Array of AbilityImprovement resources.
     */
    void set_improvements(const godot::Array& improvements);

    /**
     * @brief Returns the improvements array (always 10 elements).
     * @return Array of Ref<AbilityImprovement>.
     */
    [[nodiscard]] godot::Array get_improvements() const;

    /**
     * @brief Sets the current active upgrade level (0 = none, 1–10 = level).
     * @param level Level clamped to [0, 10].
     */
    void set_current_level(int level);

    /**
     * @brief Returns the current active upgrade level.
     * @return Integer in [0, 10].
     */
    [[nodiscard]] int get_current_level() const;

    /**
     * @brief Returns the AbilityImprovement active at the current level.
     *
     * @return The active Ref<AbilityImprovement>, or null if current_level is 0.
     */
    [[nodiscard]] godot::Ref<AbilityImprovement> get_active_improvement() const;

private:
    /** Ensures m_improvements always holds exactly 10 slots. */
    void normalize_improvements();

    static constexpr int IMPROVEMENT_COUNT = 10;
};

} // namespace Rebel::Ability
