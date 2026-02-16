// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#include "Rebel/CharacterBody/PlatformerCharacterBody3D.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

namespace Rebel::CharacterBody {
    PlatformerCharacterBody3D::PlatformerCharacterBody3D() {
        set_use_custom_gravity(true);
        set_custom_gravity_magnitude(30.0f);
    }

    void PlatformerCharacterBody3D::ApplyGravity(const double delta) {
        // Apply base gravity (custom or project-settings)
        BaseCharacterBody3D::ApplyGravity(delta);

        // Clamp fall speed (platformer-specific)
        const Vector3 gravityDirection = -get_gravity_up_direction();
        Vector3 current_velocity = get_velocity();

        if (const real_t currentFallSpeed = current_velocity.dot(gravityDirection); currentFallSpeed > maxFallSpeed) {
            current_velocity -= gravityDirection * (currentFallSpeed - maxFallSpeed);
            set_velocity(current_velocity);
        }
    }

    void PlatformerCharacterBody3D::_bind_methods() {
        ClassDB::bind_method(D_METHOD("set_jump_force", "jump_force"), &PlatformerCharacterBody3D::set_jump_force);
        ClassDB::bind_method(D_METHOD("get_jump_force"), &PlatformerCharacterBody3D::get_jump_force);

        ClassDB::bind_method(D_METHOD("set_max_jump_hold_time", "time"), &PlatformerCharacterBody3D::set_max_jump_hold_time);
        ClassDB::bind_method(D_METHOD("get_max_jump_hold_time"), &PlatformerCharacterBody3D::get_max_jump_hold_time);

        ClassDB::bind_method(D_METHOD("set_jump_hold_force", "force"), &PlatformerCharacterBody3D::set_jump_hold_force);
        ClassDB::bind_method(D_METHOD("get_jump_hold_force"), &PlatformerCharacterBody3D::get_jump_hold_force);

        ClassDB::bind_method(D_METHOD("set_jump_release_cut_factor", "factor"), &PlatformerCharacterBody3D::set_jump_release_cut_factor);
        ClassDB::bind_method(D_METHOD("get_jump_release_cut_factor"), &PlatformerCharacterBody3D::get_jump_release_cut_factor);

        ClassDB::bind_method(D_METHOD("set_max_fall_speed", "speed"), &PlatformerCharacterBody3D::set_max_fall_speed);
        ClassDB::bind_method(D_METHOD("get_max_fall_speed"), &PlatformerCharacterBody3D::get_max_fall_speed);

        // Register properties - Jump & Fall
        ADD_GROUP("Jump", "");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "jump_force", PROPERTY_HINT_RANGE, "0,100,0.1,or_greater"), "set_jump_force", "get_jump_force");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_jump_hold_time", PROPERTY_HINT_RANGE, "0,2.0,0.01"), "set_max_jump_hold_time", "get_max_jump_hold_time");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "jump_hold_force", PROPERTY_HINT_RANGE, "0,100,0.1,or_greater"), "set_jump_hold_force", "get_jump_hold_force");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "jump_release_cut_factor", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_jump_release_cut_factor", "get_jump_release_cut_factor");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_fall_speed", PROPERTY_HINT_RANGE, "0,200,0.1,or_greater"), "set_max_fall_speed", "get_max_fall_speed");
    }

    void PlatformerCharacterBody3D::set_jump_force(const float jump_force) {
        jumpForce = jump_force;
    }

    float PlatformerCharacterBody3D::get_jump_force() const {
        return jumpForce;
    }

    void PlatformerCharacterBody3D::set_max_jump_hold_time(const float time) {
        maxJumpHoldTime = Math::max(0.0f, time);
    }

    float PlatformerCharacterBody3D::get_max_jump_hold_time() const {
        return maxJumpHoldTime;
    }

    void PlatformerCharacterBody3D::set_jump_hold_force(const float force) {
        jumpHoldForce = Math::max(0.0f, force);
    }

    float PlatformerCharacterBody3D::get_jump_hold_force() const {
        return jumpHoldForce;
    }

    void PlatformerCharacterBody3D::set_jump_release_cut_factor(const float factor) {
        jumpReleaseCutFactor = Math::clamp(factor, 0.0f, 1.0f);
    }

    float PlatformerCharacterBody3D::get_jump_release_cut_factor() const {
        return jumpReleaseCutFactor;
    }

    void PlatformerCharacterBody3D::set_max_fall_speed(const float speed) {
        maxFallSpeed = speed;
    }

    float PlatformerCharacterBody3D::get_max_fall_speed() const {
        return maxFallSpeed;
    }
} // namespace Rebel::CharacterBody
