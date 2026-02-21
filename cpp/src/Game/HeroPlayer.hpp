// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved



#pragma once
#include "godot_cpp/classes/animation_node_state_machine_playback.hpp"
#include "godot_cpp/classes/animation_tree.hpp"
#include "Rebel/CharacterBody/PlayerTopDownCharacterBody3D.hpp"

namespace GaS {
    class HeroPlayer : public Rebel::CharacterBody::PlayerTopDownCharacterBody3D {
        GDCLASS(HeroPlayer, Rebel::CharacterBody::PlayerTopDownCharacterBody3D);

        godot::AnimationTree* animationTree{nullptr};
        godot::Ref<godot::AnimationNodeStateMachinePlayback> animationStateMachine{nullptr};

    protected:
        static void _bind_methods();

        /**
         * @brief Internal ready logic - initializes animation tree.
         * Calls parent's version first.
         */
        void _internal_ready() override;

    public:
        void _on_player_movement_changed(const godot::RID& instanceId, const godot::Vector3& velocity, const godot::Vector2& inputDirection, bool isOnFloor) const;

        // Animation tree property accessors
        void set_animation_tree(godot::AnimationTree* tree);
        godot::AnimationTree* get_animation_tree() const;

        HeroPlayer() = default;
        ~HeroPlayer() override = default;

    };
}
