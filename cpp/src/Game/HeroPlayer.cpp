// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

//
//

#include "HeroPlayer.hpp"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/variant/utility_functions.hpp"


using namespace godot;
using namespace GaS;

void HeroPlayer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_player_movement_changed", "instance_id", "velocity", "input_direction", "is_on_floor"), &HeroPlayer::_on_player_movement_changed);

    // Expose animation tree to editor
    ClassDB::bind_method(D_METHOD("set_animation_tree", "animation_tree"), &HeroPlayer::set_animation_tree);
    ClassDB::bind_method(D_METHOD("get_animation_tree"), &HeroPlayer::get_animation_tree);
    ClassDB::add_property("HeroPlayer", PropertyInfo(Variant::OBJECT, "animation_tree", PROPERTY_HINT_NODE_TYPE, "AnimationTree"), "set_animation_tree", "get_animation_tree");
}

void HeroPlayer::_internal_ready() {
    // Call parent first (initializes camera, gravity, state)

    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    // Initialize animation state machine from the assigned animation tree
    if (animationTree) {
        animationStateMachine = animationTree->get("parameters/playback");
    } else {
        UtilityFunctions::push_warning("[HeroPlayer] AnimationTree not assigned in editor");
    }
}

void HeroPlayer::_on_player_movement_changed(const RID& instanceId, const Vector3& velocity, const Vector2& inputDirection, bool isOnFloor) const {
    if (animationStateMachine.is_null() || animationTree == nullptr) {
        return;
    }

    // Check if falling
    if (!isOnFloor) {
        animationStateMachine->travel("JumpAir", false);
        return;
    }

    // Switch animation state based on input
    if (inputDirection.is_zero_approx()) {
        animationStateMachine->travel("Idle", false);
        // Update the blend position based on input intensity
        animationTree->set("parameters/Move/BlendSpace1D/blend_position", 0.0f);
    } else {
        animationStateMachine->travel("Move", false);
        // Update the blend position based on input intensity
        animationTree->set("parameters/Move/BlendSpace1D/blend_position", inputDirection.length());
    }
}

void HeroPlayer::set_animation_tree(AnimationTree* tree) {
    animationTree = tree;
}

AnimationTree* HeroPlayer::get_animation_tree() const {
    return animationTree;
}
