// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#include "Rebel/CharacterBody/BaseCharacterBody3D.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/project_settings.hpp>

#include "godot_cpp/classes/engine.hpp"

using namespace godot;

namespace Rebel::CharacterBody {
    void BaseCharacterBody3D::ApplyGravity(const double delta) {
        Vector3 gravityDirection;
        real_t gravityStrength;

        if (useCustomGravity) {
            gravityDirection = customGravityDirection.normalized();
            gravityStrength = customGravityMagnitude;
        } else {
            gravityDirection = defaultGravityVec.normalized();
            gravityStrength = defaultGravityStrength;
        }

        if (is_on_floor() && get_velocity().dot(gravityDirection) > 0) {
            // Reset velocity in the direction of gravity to a tiny value
            // to ensure we stay snapped to the floor but don't accumulate speed.
        }

        const Vector3 gravityVector = gravityDirection * gravityStrength;
        Vector3 current_velocity = get_velocity();
        current_velocity += gravityVector * static_cast<real_t>(delta);

        set_velocity(current_velocity);
    }

    void BaseCharacterBody3D::_bind_methods() {
        // Bind getter/setter methods - Movement
        ClassDB::bind_method(D_METHOD("set_moving_speed", "speed"), &BaseCharacterBody3D::set_moving_speed);
        ClassDB::bind_method(D_METHOD("get_moving_speed"), &BaseCharacterBody3D::get_moving_speed);

        ClassDB::bind_method(D_METHOD("set_acceleration_time", "value"), &BaseCharacterBody3D::set_acceleration_time);
        ClassDB::bind_method(D_METHOD("get_acceleration_time"), &BaseCharacterBody3D::get_acceleration_time);

        ClassDB::bind_method(D_METHOD("set_deceleration_time", "value"), &BaseCharacterBody3D::set_deceleration_time);
        ClassDB::bind_method(D_METHOD("get_deceleration_time"), &BaseCharacterBody3D::get_deceleration_time);

        ClassDB::bind_method(D_METHOD("set_acceleration_curve_intensity", "value"), &BaseCharacterBody3D::set_acceleration_curve_intensity);
        ClassDB::bind_method(D_METHOD("get_acceleration_curve_intensity"), &BaseCharacterBody3D::get_acceleration_curve_intensity);

        ClassDB::bind_method(D_METHOD("set_deceleration_curve_intensity", "value"), &BaseCharacterBody3D::set_deceleration_curve_intensity);
        ClassDB::bind_method(D_METHOD("get_deceleration_curve_intensity"), &BaseCharacterBody3D::get_deceleration_curve_intensity);

        ClassDB::bind_method(D_METHOD("set_rotation_speed", "value"), &BaseCharacterBody3D::set_rotation_speed);
        ClassDB::bind_method(D_METHOD("get_rotation_speed"), &BaseCharacterBody3D::get_rotation_speed);

        ClassDB::bind_method(D_METHOD("set_attack_charge_time", "time"), &BaseCharacterBody3D::set_attack_charge_time);
        ClassDB::bind_method(D_METHOD("get_attack_charge_time"), &BaseCharacterBody3D::get_attack_charge_time);

        // Bind getter/setter methods - Gravity
        ClassDB::bind_method(D_METHOD("set_use_custom_gravity", "enabled"), &BaseCharacterBody3D::set_use_custom_gravity);
        ClassDB::bind_method(D_METHOD("get_use_custom_gravity"), &BaseCharacterBody3D::get_use_custom_gravity);

        ClassDB::bind_method(D_METHOD("set_custom_gravity_direction", "direction"), &BaseCharacterBody3D::set_custom_gravity_direction);
        ClassDB::bind_method(D_METHOD("get_custom_gravity_direction"), &BaseCharacterBody3D::get_custom_gravity_direction);

        ClassDB::bind_method(D_METHOD("set_custom_gravity_magnitude", "magnitude"), &BaseCharacterBody3D::set_custom_gravity_magnitude);
        ClassDB::bind_method(D_METHOD("get_custom_gravity_magnitude"), &BaseCharacterBody3D::get_custom_gravity_magnitude);

        // Register properties - Movement
        ADD_GROUP("Movement", "");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "moving_speed", PROPERTY_HINT_RANGE, "0,100,0.1,or_greater"), "set_moving_speed", "get_moving_speed");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "acceleration_time", PROPERTY_HINT_RANGE, "0.01,2.0,0.01,or_greater"), "set_acceleration_time", "get_acceleration_time");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "deceleration_time", PROPERTY_HINT_RANGE, "0.01,2.0,0.01,or_greater"), "set_deceleration_time", "get_deceleration_time");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "acceleration_curve_intensity", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_acceleration_curve_intensity", "get_acceleration_curve_intensity");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "deceleration_curve_intensity", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_deceleration_curve_intensity", "get_deceleration_curve_intensity");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "rotation_speed", PROPERTY_HINT_RANGE, "0,50,0.1,or_greater"), "set_rotation_speed", "get_rotation_speed");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_charge_time", PROPERTY_HINT_RANGE, "0.1,5.0,0.1,or_greater"), "set_attack_charge_time", "get_attack_charge_time");

        // Register properties - Gravity
        ADD_GROUP("Gravity", "");
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_custom_gravity"), "set_use_custom_gravity", "get_use_custom_gravity");
        ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "custom_gravity_direction"), "set_custom_gravity_direction", "get_custom_gravity_direction");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "custom_gravity_magnitude", PROPERTY_HINT_RANGE, "0,100,0.1,or_greater"), "set_custom_gravity_magnitude", "get_custom_gravity_magnitude");
    }

    void BaseCharacterBody3D::set_use_custom_gravity(const bool enabled) {
        useCustomGravity = enabled;
    }

    bool BaseCharacterBody3D::get_use_custom_gravity() const {
        return useCustomGravity;
    }

    void BaseCharacterBody3D::set_custom_gravity_direction(const Vector3& direction) {
        customGravityDirection = direction.normalized();
    }

    Vector3 BaseCharacterBody3D::get_custom_gravity_direction() const {
        return customGravityDirection;
    }

    void BaseCharacterBody3D::set_custom_gravity_magnitude(const float magnitude) {
        customGravityMagnitude = magnitude;
    }

    float BaseCharacterBody3D::get_custom_gravity_magnitude() const {
        return customGravityMagnitude;
    }

    void BaseCharacterBody3D::set_attack_charge_time(const float time) {
        attackChargeTime = Math::max(0.1f, time);
    }

    float BaseCharacterBody3D::get_attack_charge_time() const {
        return attackChargeTime;
    }

    float BaseCharacterBody3D::get_moving_speed() const {
        return movingSpeed;
    }

    void BaseCharacterBody3D::set_moving_speed(const float speed) {
        movingSpeed = speed;
    }

    float BaseCharacterBody3D::get_acceleration_time() const {
        return accelerationTime;
    }

    void BaseCharacterBody3D::set_acceleration_time(const float value) {
        accelerationTime = Math::max(0.01f, value);
    }

    float BaseCharacterBody3D::get_deceleration_time() const {
        return decelerationTime;
    }

    void BaseCharacterBody3D::set_deceleration_time(const float value) {
        decelerationTime = Math::max(0.01f, value);
    }

    float BaseCharacterBody3D::get_acceleration_curve_intensity() const {
        return accelerationCurveIntensity;
    }

    void BaseCharacterBody3D::set_acceleration_curve_intensity(const float value) {
        accelerationCurveIntensity = Math::clamp(value, 0.0f, 1.0f);
    }

    float BaseCharacterBody3D::get_deceleration_curve_intensity() const {
        return decelerationCurveIntensity;
    }

    void BaseCharacterBody3D::set_deceleration_curve_intensity(const float value) {
        decelerationCurveIntensity = Math::clamp(value, 0.0f, 1.0f);
    }

    float BaseCharacterBody3D::ease_in(const float t, const float intensity) {
        const float power = 1.0f + intensity * 3.0f;  // Range: 1 (linear) to 4 (sharp)
        return Math::pow(t, power);
    }

    float BaseCharacterBody3D::ease_out(const float t, const float intensity) {
        const float power = 1.0f + intensity * 3.0f;
        return 1.0f - Math::pow(1.0f - t, power);
    }

    float BaseCharacterBody3D::get_rotation_speed() const {
        return rotationSpeed;
    }

    void BaseCharacterBody3D::set_rotation_speed(const float value) {
        rotationSpeed = value;
    }

    Vector3 BaseCharacterBody3D::get_gravity_up_direction() const {
        if (useCustomGravity) {
            return -customGravityDirection.normalized();
        }
        return -defaultGravityVec.normalized();
    }

    void BaseCharacterBody3D::_notification(const int p_what) {
        switch (p_what) {
            case NOTIFICATION_ENTER_TREE:
                _internal_enter_tree();
                break;
            case NOTIFICATION_READY:
                _internal_ready();
                break;
            case NOTIFICATION_PHYSICS_PROCESS:
                _internal_physics_process(get_physics_process_delta_time());
                break;
            case NOTIFICATION_PROCESS:
                _internal_process(get_process_delta_time());
                break;
            default:
                break;
        }
    }

    void BaseCharacterBody3D::_internal_enter_tree() {
        // Base class: nothing to do on enter tree
    }

    void BaseCharacterBody3D::_internal_ready() {
        // Cache the settings once at startup to save performance
        const ProjectSettings* settings = ProjectSettings::get_singleton();
        defaultGravityVec = settings->get_setting("physics/3d/default_gravity_vector");
        defaultGravityStrength = static_cast<float>(settings->get_setting("physics/3d/default_gravity"));

        // FORCE STOP any accumulated velocity from startup lag
        set_velocity(Vector3(0, 0, 0));
    }

    void BaseCharacterBody3D::_internal_physics_process(const double delta) {
        if (Engine::get_singleton()->is_editor_hint()) {
            return;
        }
        ApplyGravity(delta);
        move_and_slide();
    }

    void BaseCharacterBody3D::_internal_process(const double delta) {
        // Base class: nothing to do on process
    }

    void BaseCharacterBody3D::_physics_process(const double p_delta) {
        // This may not be called if GDScript overrides it.
        // Core logic is in _internal_physics_process() called via _notification().
    }

    void BaseCharacterBody3D::_ready() {
        // This may not be called if GDScript overrides it.
        // Core logic is in _internal_ready() called via _notification().
    }
} // namespace Rebel::CharacterBody
