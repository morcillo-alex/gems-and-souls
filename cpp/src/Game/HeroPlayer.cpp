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
}

void HeroPlayer::_internal_ready() {
    // Call parent first (initializes camera, gravity, state)

    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }

    // Initialize animation tree
    animationTree = cast_to<AnimationTree>(get_node_or_null(NodePath("Hiker/AnimationTree")));
    if (animationTree) {
        animationStateMachine = animationTree->get("parameters/playback");
    } else {
        UtilityFunctions::push_error("[HeroPlayer] AnimationTree not found at Hiker/AnimationTree");
    }
}

void HeroPlayer::_on_player_movement_changed(const RID& instanceId, const Vector3& velocity, const Vector2& inputDirection, bool isOnFloor) const {
    if (animationStateMachine.is_null() || animationTree == nullptr) {
        return;
    }

    // check if falling
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
