// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved



#pragma once
#include "godot_cpp/classes/animation_node_state_machine_playback.hpp"
#include "godot_cpp/classes/animation_tree.hpp"
#include "godot_cpp/variant/string_name.hpp"
#include "Rebel/CharacterBody/PlayerTopDownCharacterBody3D.hpp"
#include "Rebel/Ability/AbilityTree.hpp"

namespace GaS {

/**
 * @brief The player-controlled hero character for Gems and Souls.
 *
 * Extends PlayerTopDownCharacterBody3D with animation state machine integration.
 * All animation state names and the blend parameter path are exposed to the Godot
 * editor so designers can configure them without modifying C++ code.
 *
 * ## Editor-Exposed Properties
 *
 * ### Ability Tree
 * - `ability_tree` — The AbilityTree resource that defines the hero's abilities and upgrade paths.
 *
 * ### Animation Node
 * - `animation_tree` — The AnimationTree node to drive.
 *
 * ### Animation States (group: "Animation States")
 * - `anim_state_idle`       — State machine node name for the idle state.
 * - `anim_state_move`       — State machine node name for the movement state.
 * - `anim_state_jump_air`   — State machine node name for the airborne/jump state.
 * - `anim_blend_move_param` — AnimationTree parameter path for the movement blend position.
 *
 * ## Signals
 * Inherits `player_movement_changed`, `attack1_released`, `attack2_released`,
 * and `dodge_performed` from PlayerTopDownCharacterBody3D.
 */
class HeroPlayer : public Rebel::CharacterBody::PlayerTopDownCharacterBody3D {
    GDCLASS(HeroPlayer, Rebel::CharacterBody::PlayerTopDownCharacterBody3D);

    // -------------------------------------------------------------------------
    // Ability system
    // -------------------------------------------------------------------------

    /** The AbilityTree resource assigned in the editor. */
    godot::Ref<Rebel::Ability::AbilityTree> m_abilityTree{};

    // -------------------------------------------------------------------------
    // Animation node references
    // -------------------------------------------------------------------------

    /** The AnimationTree node assigned in the editor. */
    godot::AnimationTree* m_animationTree{nullptr};

    /** Cached reference to the AnimationNodeStateMachinePlayback from the tree. */
    godot::Ref<godot::AnimationNodeStateMachinePlayback> m_animationStateMachine{nullptr};

    // -------------------------------------------------------------------------
    // Configurable animation state names
    // These are passed to AnimationNodeStateMachinePlayback::travel().
    // StringName is used because travel() accepts StringName and comparisons
    // are O(1) hash lookups — important for a call that runs every physics frame.
    // -------------------------------------------------------------------------

    /** State machine node name for the idle animation state. */
    godot::StringName m_animStateIdle{"Idle"};

    /** State machine node name for the movement animation state. */
    godot::StringName m_animStateMove{"Move"};

    /** State machine node name for the airborne/jump animation state. Empty = no airborne transition. */
    godot::StringName m_animStateJumpAir{};

    // -------------------------------------------------------------------------
    // Configurable AnimationTree parameter paths
    // -------------------------------------------------------------------------

    /**
     * @brief AnimationTree parameter path for the movement blend space position.
     *
     * This controls the blend between walk and run within the Move state's BlendSpace1D.
     * Default matches the default AnimationTree layout: "parameters/Move/BlendSpace1D/blend_position".
     */
    godot::String m_animBlendMoveParam{"parameters/Move/BlendSpace1D/blend_position"};

protected:
    static void _bind_methods();

    /**
     * @brief Internal ready logic — initializes the animation state machine.
     *
     * Calls the parent implementation first (which activates the camera and
     * resets movement state), then retrieves the playback object from the
     * assigned AnimationTree.
     */
    void _internal_ready() override;

public:
    /**
     * @brief Responds to the player_movement_changed signal from the base class.
     *
     * Drives the AnimationTree state machine based on whether the player has
     * input and whether they are grounded.
     *
     * @param instanceId  The RID of the CharacterBody3D instance (unused here).
     * @param velocity    The current world-space velocity of the character.
     * @param inputDirection  The 2D input vector (from the input action axes).
     * @param isOnFloor   Whether the character is currently on the floor.
     */
    void _on_player_movement_changed(
        const godot::RID& instanceId,
        const godot::Vector3& velocity,
        const godot::Vector2& inputDirection,
        bool isOnFloor) const;

    // -------------------------------------------------------------------------
    // AbilityTree property accessors
    // -------------------------------------------------------------------------

    /**
     * @brief Assigns the AbilityTree resource for this hero.
     * @param tree The AbilityTree resource. May be null to clear.
     */
    void set_ability_tree(const godot::Ref<Rebel::Ability::AbilityTree>& tree);

    /**
     * @brief Returns the currently assigned AbilityTree resource.
     * @return Ref<AbilityTree>, may be null if not assigned.
     */
    [[nodiscard]] godot::Ref<Rebel::Ability::AbilityTree> get_ability_tree() const;

    // -------------------------------------------------------------------------
    // AnimationTree property accessors
    // -------------------------------------------------------------------------

    /**
     * @brief Assigns the AnimationTree node used for animation playback.
     * @param tree Pointer to the AnimationTree node. May be nullptr to clear the reference.
     */
    void set_animation_tree(godot::AnimationTree* tree);

    /**
     * @brief Returns the currently assigned AnimationTree node.
     * @return Pointer to the AnimationTree, or nullptr if not assigned.
     */
    [[nodiscard]] godot::AnimationTree* get_animation_tree() const;

    // -------------------------------------------------------------------------
    // Animation state name accessors
    // -------------------------------------------------------------------------

    /** @brief Sets the state machine node name for the idle state. */
    void set_anim_state_idle(const godot::StringName& name);
    /** @brief Returns the state machine node name for the idle state. */
    [[nodiscard]] godot::StringName get_anim_state_idle() const;

    /** @brief Sets the state machine node name for the movement state. */
    void set_anim_state_move(const godot::StringName& name);
    /** @brief Returns the state machine node name for the movement state. */
    [[nodiscard]] godot::StringName get_anim_state_move() const;

    /** @brief Sets the state machine node name for the airborne/jump state. */
    void set_anim_state_jump_air(const godot::StringName& name);
    /** @brief Returns the state machine node name for the airborne/jump state. */
    [[nodiscard]] godot::StringName get_anim_state_jump_air() const;

    // -------------------------------------------------------------------------
    // Animation parameter path accessors
    // -------------------------------------------------------------------------

    /**
     * @brief Sets the AnimationTree parameter path for the movement blend position.
     * @param path Full parameter path, e.g. "parameters/Move/BlendSpace1D/blend_position".
     */
    void set_anim_blend_move_param(const godot::String& path);

    /**
     * @brief Returns the AnimationTree parameter path for the movement blend position.
     * @return The parameter path string.
     */
    [[nodiscard]] godot::String get_anim_blend_move_param() const;

    HeroPlayer() = default;
    ~HeroPlayer() override = default;
};

} // namespace GaS
