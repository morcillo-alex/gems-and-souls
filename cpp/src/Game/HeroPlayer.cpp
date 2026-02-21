// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#include "HeroPlayer.hpp"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

using namespace godot;
using namespace GaS;

// -----------------------------------------------------------------------------
// _bind_methods
// -----------------------------------------------------------------------------

void HeroPlayer::_bind_methods() {
    // -------------------------------------------------------------------------
    // Signal handler
    // -------------------------------------------------------------------------
    ClassDB::bind_method(
        D_METHOD("_on_player_movement_changed", "instance_id", "velocity", "input_direction", "is_on_floor"),
        &HeroPlayer::_on_player_movement_changed);

    // -------------------------------------------------------------------------
    // AbilityTree resource property
    // -------------------------------------------------------------------------
    ClassDB::bind_method(D_METHOD("set_ability_tree", "ability_tree"), &HeroPlayer::set_ability_tree);
    ClassDB::bind_method(D_METHOD("get_ability_tree"), &HeroPlayer::get_ability_tree);
    ClassDB::add_property(
        "HeroPlayer",
        PropertyInfo(Variant::OBJECT, "ability_tree", PROPERTY_HINT_RESOURCE_TYPE, "AbilityTree"),
        "set_ability_tree",
        "get_ability_tree");

    // -------------------------------------------------------------------------
    // AnimationTree node property
    // -------------------------------------------------------------------------
    ClassDB::bind_method(D_METHOD("set_animation_tree", "animation_tree"), &HeroPlayer::set_animation_tree);
    ClassDB::bind_method(D_METHOD("get_animation_tree"), &HeroPlayer::get_animation_tree);
    ClassDB::add_property(
        "HeroPlayer",
        PropertyInfo(Variant::OBJECT, "animation_tree", PROPERTY_HINT_NODE_TYPE, "AnimationTree"),
        "set_animation_tree",
        "get_animation_tree");

    // -------------------------------------------------------------------------
    // Animation state name properties
    // Grouped under "Animation States" so they appear cleanly in the inspector.
    // StringName is used so that comparisons and lookups in the state machine
    // remain O(1) at runtime.
    // -------------------------------------------------------------------------
    ADD_GROUP("Animation States", "anim_state_");

    ClassDB::bind_method(D_METHOD("set_anim_state_idle", "name"), &HeroPlayer::set_anim_state_idle);
    ClassDB::bind_method(D_METHOD("get_anim_state_idle"), &HeroPlayer::get_anim_state_idle);
    ADD_PROPERTY(
        PropertyInfo(Variant::STRING_NAME, "anim_state_idle"),
        "set_anim_state_idle",
        "get_anim_state_idle");

    ClassDB::bind_method(D_METHOD("set_anim_state_move", "name"), &HeroPlayer::set_anim_state_move);
    ClassDB::bind_method(D_METHOD("get_anim_state_move"), &HeroPlayer::get_anim_state_move);
    ADD_PROPERTY(
        PropertyInfo(Variant::STRING_NAME, "anim_state_move"),
        "set_anim_state_move",
        "get_anim_state_move");

    ClassDB::bind_method(D_METHOD("set_anim_state_jump_air", "name"), &HeroPlayer::set_anim_state_jump_air);
    ClassDB::bind_method(D_METHOD("get_anim_state_jump_air"), &HeroPlayer::get_anim_state_jump_air);
    ADD_PROPERTY(
        PropertyInfo(Variant::STRING_NAME, "anim_state_jump_air"),
        "set_anim_state_jump_air",
        "get_anim_state_jump_air");

    // -------------------------------------------------------------------------
    // Animation parameter path properties
    // -------------------------------------------------------------------------
    ADD_GROUP("Animation Parameters", "anim_");

    ClassDB::bind_method(D_METHOD("set_anim_blend_move_param", "path"), &HeroPlayer::set_anim_blend_move_param);
    ClassDB::bind_method(D_METHOD("get_anim_blend_move_param"), &HeroPlayer::get_anim_blend_move_param);
    ADD_PROPERTY(
        PropertyInfo(Variant::STRING, "anim_blend_move_param"),
        "set_anim_blend_move_param",
        "get_anim_blend_move_param");
}

// -----------------------------------------------------------------------------
// Lifecycle
// -----------------------------------------------------------------------------

void HeroPlayer::_internal_ready() {
    // Always call the parent first. PlayerTopDownCharacterBody3D::_internal_ready()
    // activates the camera and resets acceleration/deceleration state.
    PlayerTopDownCharacterBody3D::_internal_ready();

    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    // Connect the movement signal from the base class to our animation handler.
    // Use CONNECT_ONE_SHOT is NOT appropriate here — we want the connection
    // to persist for the lifetime of the node. connect() is safe to call
    // multiple times on the same signal+callable pair; Godot will ignore
    // duplicate connections.
    if (!is_connected("player_movement_changed", Callable(this, "_on_player_movement_changed"))) {
        connect("player_movement_changed", Callable(this, "_on_player_movement_changed"));
    }

    // Retrieve the state machine playback object from the AnimationTree.
    if (m_animationTree != nullptr) {
        m_animationStateMachine = m_animationTree->get("parameters/playback");
        if (m_animationStateMachine.is_null()) {
            UtilityFunctions::push_warning(
                "[HeroPlayer] AnimationTree does not have a 'parameters/playback' state machine. "
                "Ensure the AnimationTree root node is an AnimationNodeStateMachine.");
        }
    } else {
        UtilityFunctions::push_warning(
            "[HeroPlayer] AnimationTree not assigned. "
            "Assign an AnimationTree node in the inspector under the 'animation_tree' property.");
    }
}

// -----------------------------------------------------------------------------
// Signal handler
// -----------------------------------------------------------------------------

void HeroPlayer::_on_player_movement_changed(
    const RID& /*instanceId*/,
    const Vector3& /*velocity*/,
    const Vector2& inputDirection,
    const bool isOnFloor) const
{
    if (m_animationStateMachine.is_null() || m_animationTree == nullptr) {
        return;
    }

    // Airborne check takes priority over ground movement states.
    // Only travel if a jump/air state has been configured (top-down games may omit it).
    if (!isOnFloor && !m_animStateJumpAir.is_empty()) {
        m_animationStateMachine->travel(m_animStateJumpAir, false);
        return;
    }

    if (inputDirection.is_zero_approx()) {
        // No input — transition to idle and reset blend position to 0.
        m_animationStateMachine->travel(m_animStateIdle, false);
        m_animationTree->set(m_animBlendMoveParam, 0.0f);
    } else {
        // Has input — transition to the movement state and drive the blend
        // position by the magnitude of the input vector (0=walk, 1=run).
        m_animationStateMachine->travel(m_animStateMove, false);
        m_animationTree->set(m_animBlendMoveParam, inputDirection.length());
    }
}

// -----------------------------------------------------------------------------
// AbilityTree property
// -----------------------------------------------------------------------------

void HeroPlayer::set_ability_tree(const Ref<Rebel::Ability::AbilityTree>& tree) {
    m_abilityTree = tree;
}

Ref<Rebel::Ability::AbilityTree> HeroPlayer::get_ability_tree() const {
    return m_abilityTree;
}

// -----------------------------------------------------------------------------
// AnimationTree property
// -----------------------------------------------------------------------------

void HeroPlayer::set_animation_tree(AnimationTree* tree) {
    m_animationTree = tree;
}

AnimationTree* HeroPlayer::get_animation_tree() const {
    return m_animationTree;
}

// -----------------------------------------------------------------------------
// Animation state name properties
// -----------------------------------------------------------------------------

void HeroPlayer::set_anim_state_idle(const StringName& name) {
    m_animStateIdle = name;
}

StringName HeroPlayer::get_anim_state_idle() const {
    return m_animStateIdle;
}

void HeroPlayer::set_anim_state_move(const StringName& name) {
    m_animStateMove = name;
}

StringName HeroPlayer::get_anim_state_move() const {
    return m_animStateMove;
}

void HeroPlayer::set_anim_state_jump_air(const StringName& name) {
    m_animStateJumpAir = name;
}

StringName HeroPlayer::get_anim_state_jump_air() const {
    return m_animStateJumpAir;
}

// -----------------------------------------------------------------------------
// Animation parameter path properties
// -----------------------------------------------------------------------------

void HeroPlayer::set_anim_blend_move_param(const String& path) {
    m_animBlendMoveParam = path;
}

String HeroPlayer::get_anim_blend_move_param() const {
    return m_animBlendMoveParam;
}
