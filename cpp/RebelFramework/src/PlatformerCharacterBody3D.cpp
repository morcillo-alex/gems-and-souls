#include "Rebel/CharacterBody/PlatformerCharacterBody3D.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/project_settings.hpp>

#include "godot_cpp/classes/engine.hpp"

using namespace godot;

namespace Rebel::CharacterBody {
    void PlatformerCharacterBody3D::ApplyGravity(const double delta) {
        // 2. Setup Gravity Variables
        Vector3 gravityDirection;
        real_t gravityStrength;

        if (useCustomGravity) {
            gravityDirection = customGravityDirection.normalized();
            gravityStrength = customGravityMagnitude;
        } else {
            // Use cached values instead of querying ProjectSettings every frame
            gravityDirection = defaultGravityVec.normalized();
            gravityStrength = defaultGravityStrength;
        }

        // 3. Handle "On Floor" logic properly
        // If we are on the floor, we generally don't want to accumulate massive falling speed,
        // BUT we shouldn't return early. We want gravity to exist, so we stick to slopes.
        // However, usually 'move_and_slide' resets velocity.y on collision.
        // If you want to ensure the character doesn't accumulate infinite gravity while standing:
        if (is_on_floor() && get_velocity().dot(gravityDirection) > 0) {
            // Reset velocity in the direction of gravity to a tiny value
            // to ensure we stay snapped to the floor but don't accumulate speed.
            // (Optional: standard CharacterBody3D behavior usually handles this via move_and_slide)
        }

        // 4. Apply Gravity
        const Vector3 gravityVector = gravityDirection * gravityStrength;
        Vector3 current_velocity = get_velocity();

        // Apply gravity
        current_velocity += gravityVector * static_cast<real_t>(delta);

        // 5. Clamp fall speed
        // This logic is valid: It checks how fast we are moving IN THE DIRECTION of gravity

        if (const real_t currentFallSpeed = current_velocity.dot(gravityDirection); currentFallSpeed > maxFallSpeed) {
            // We subtract the difference so we are exactly at maxFallSpeed
            current_velocity -= gravityDirection * (currentFallSpeed - maxFallSpeed);
        }

        //godot::UtilityFunctions::print("[PlatformerCharacterBody3D] Velocity: ", current_velocity);
        //godot::UtilityFunctions::print("[PlatformerCharacterBody3D] Current Fall speed: ", current_velocity.dot(gravityDirection));
        //godot::UtilityFunctions::print("[PlatformerCharacterBody3D] Delta: ", delta);


        set_velocity(current_velocity);
    }

    void PlatformerCharacterBody3D::_bind_methods() {
        // Bind getter/setter methods - Movement
        ClassDB::bind_method(D_METHOD("set_moving_speed", "speed"), &PlatformerCharacterBody3D::set_moving_speed);
        ClassDB::bind_method(D_METHOD("get_moving_speed"), &PlatformerCharacterBody3D::get_moving_speed);

        ClassDB::bind_method(D_METHOD("set_acceleration_time", "value"), &PlatformerCharacterBody3D::set_acceleration_time);
        ClassDB::bind_method(D_METHOD("get_acceleration_time"), &PlatformerCharacterBody3D::get_acceleration_time);

        ClassDB::bind_method(D_METHOD("set_deceleration_time", "value"), &PlatformerCharacterBody3D::set_deceleration_time);
        ClassDB::bind_method(D_METHOD("get_deceleration_time"), &PlatformerCharacterBody3D::get_deceleration_time);

        ClassDB::bind_method(D_METHOD("set_acceleration_curve_intensity", "value"), &PlatformerCharacterBody3D::set_acceleration_curve_intensity);
        ClassDB::bind_method(D_METHOD("get_acceleration_curve_intensity"), &PlatformerCharacterBody3D::get_acceleration_curve_intensity);

        ClassDB::bind_method(D_METHOD("set_deceleration_curve_intensity", "value"), &PlatformerCharacterBody3D::set_deceleration_curve_intensity);
        ClassDB::bind_method(D_METHOD("get_deceleration_curve_intensity"), &PlatformerCharacterBody3D::get_deceleration_curve_intensity);

        ClassDB::bind_method(D_METHOD("set_rotation_speed", "value"), &PlatformerCharacterBody3D::set_rotation_speed);
        ClassDB::bind_method(D_METHOD("get_rotation_speed"), &PlatformerCharacterBody3D::get_rotation_speed);

        ClassDB::bind_method(D_METHOD("set_jump_force", "jump_force"), &PlatformerCharacterBody3D::set_jump_force);
        ClassDB::bind_method(D_METHOD("get_jump_force"), &PlatformerCharacterBody3D::get_jump_force);

        ClassDB::bind_method(D_METHOD("set_max_jump_hold_time", "time"), &PlatformerCharacterBody3D::set_max_jump_hold_time);
        ClassDB::bind_method(D_METHOD("get_max_jump_hold_time"), &PlatformerCharacterBody3D::get_max_jump_hold_time);

        ClassDB::bind_method(D_METHOD("set_jump_hold_force", "force"), &PlatformerCharacterBody3D::set_jump_hold_force);
        ClassDB::bind_method(D_METHOD("get_jump_hold_force"), &PlatformerCharacterBody3D::get_jump_hold_force);

        ClassDB::bind_method(D_METHOD("set_jump_release_cut_factor", "factor"), &PlatformerCharacterBody3D::set_jump_release_cut_factor);
        ClassDB::bind_method(D_METHOD("get_jump_release_cut_factor"), &PlatformerCharacterBody3D::get_jump_release_cut_factor);

        ClassDB::bind_method(D_METHOD("set_attack_charge_time", "time"), &PlatformerCharacterBody3D::set_attack_charge_time);
        ClassDB::bind_method(D_METHOD("get_attack_charge_time"), &PlatformerCharacterBody3D::get_attack_charge_time);

        // Bind getter/setter methods - Gravity
        ClassDB::bind_method(D_METHOD("set_use_custom_gravity", "enabled"), &PlatformerCharacterBody3D::set_use_custom_gravity);
        ClassDB::bind_method(D_METHOD("get_use_custom_gravity"), &PlatformerCharacterBody3D::get_use_custom_gravity);

        ClassDB::bind_method(D_METHOD("set_custom_gravity_direction", "direction"), &PlatformerCharacterBody3D::set_custom_gravity_direction);
        ClassDB::bind_method(D_METHOD("get_custom_gravity_direction"), &PlatformerCharacterBody3D::get_custom_gravity_direction);

        ClassDB::bind_method(D_METHOD("set_custom_gravity_magnitude", "magnitude"), &PlatformerCharacterBody3D::set_custom_gravity_magnitude);
        ClassDB::bind_method(D_METHOD("get_custom_gravity_magnitude"), &PlatformerCharacterBody3D::get_custom_gravity_magnitude);

        ClassDB::bind_method(D_METHOD("set_max_fall_speed", "speed"), &PlatformerCharacterBody3D::set_max_fall_speed);
        ClassDB::bind_method(D_METHOD("get_max_fall_speed"), &PlatformerCharacterBody3D::get_max_fall_speed);

        // Register properties - Movement
        ADD_GROUP("Movement", "");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "moving_speed", PROPERTY_HINT_RANGE, "0,100,0.1,or_greater"), "set_moving_speed", "get_moving_speed");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "acceleration_time", PROPERTY_HINT_RANGE, "0.01,2.0,0.01,or_greater"), "set_acceleration_time", "get_acceleration_time");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "deceleration_time", PROPERTY_HINT_RANGE, "0.01,2.0,0.01,or_greater"), "set_deceleration_time", "get_deceleration_time");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "acceleration_curve_intensity", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_acceleration_curve_intensity", "get_acceleration_curve_intensity");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "deceleration_curve_intensity", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_deceleration_curve_intensity", "get_deceleration_curve_intensity");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "rotation_speed", PROPERTY_HINT_RANGE, "0,50,0.1,or_greater"), "set_rotation_speed", "get_rotation_speed");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "jump_force", PROPERTY_HINT_RANGE, "0,100,0.1,or_greater"), "set_jump_force", "get_jump_force");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_jump_hold_time", PROPERTY_HINT_RANGE, "0,2.0,0.01"), "set_max_jump_hold_time", "get_max_jump_hold_time");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "jump_hold_force", PROPERTY_HINT_RANGE, "0,100,0.1,or_greater"), "set_jump_hold_force", "get_jump_hold_force");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "jump_release_cut_factor", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_jump_release_cut_factor", "get_jump_release_cut_factor");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "attack_charge_time", PROPERTY_HINT_RANGE, "0.1,5.0,0.1,or_greater"), "set_attack_charge_time", "get_attack_charge_time");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_fall_speed", PROPERTY_HINT_RANGE, "0,200,0.1,or_greater"), "set_max_fall_speed", "get_max_fall_speed");

        // Register properties - Gravity
        ADD_GROUP("Gravity", "");
        ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_custom_gravity"), "set_use_custom_gravity", "get_use_custom_gravity");
        ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "custom_gravity_direction"), "set_custom_gravity_direction", "get_custom_gravity_direction");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "custom_gravity_magnitude", PROPERTY_HINT_RANGE, "0,100,0.1,or_greater"), "set_custom_gravity_magnitude", "get_custom_gravity_magnitude");
    }

    void PlatformerCharacterBody3D::set_use_custom_gravity(const bool enabled) {
        useCustomGravity = enabled;
    }

    bool PlatformerCharacterBody3D::get_use_custom_gravity() const {
        return useCustomGravity;
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

    void PlatformerCharacterBody3D::set_attack_charge_time(const float time) {
        attackChargeTime = Math::max(0.1f, time);
    }

    float PlatformerCharacterBody3D::get_attack_charge_time() const {
        return attackChargeTime;
    }

    void PlatformerCharacterBody3D::set_custom_gravity_direction(const Vector3& direction) {
        customGravityDirection = direction.normalized();
    }

    Vector3 PlatformerCharacterBody3D::get_custom_gravity_direction() const {
        return customGravityDirection;
    }

    void PlatformerCharacterBody3D::set_custom_gravity_magnitude(const float magnitude) {
        customGravityMagnitude = magnitude;
    }

    float PlatformerCharacterBody3D::get_custom_gravity_magnitude() const {
        return customGravityMagnitude;
    }

    void PlatformerCharacterBody3D::set_max_fall_speed(const float speed) {
        maxFallSpeed = speed;
    }

    float PlatformerCharacterBody3D::get_max_fall_speed() const {
        return maxFallSpeed;
    }

    float PlatformerCharacterBody3D::get_moving_speed() const {
        return movingSpeed;
    }

    void PlatformerCharacterBody3D::set_moving_speed(const float speed) {
        movingSpeed = speed;
    }

    float PlatformerCharacterBody3D::get_acceleration_time() const {
        return accelerationTime;
    }

    void PlatformerCharacterBody3D::set_acceleration_time(const float value) {
        accelerationTime = Math::max(0.01f, value);
    }

    float PlatformerCharacterBody3D::get_deceleration_time() const {
        return decelerationTime;
    }

    void PlatformerCharacterBody3D::set_deceleration_time(const float value) {
        decelerationTime = Math::max(0.01f, value);
    }

    float PlatformerCharacterBody3D::get_acceleration_curve_intensity() const {
        return accelerationCurveIntensity;
    }

    void PlatformerCharacterBody3D::set_acceleration_curve_intensity(const float value) {
        accelerationCurveIntensity = Math::clamp(value, 0.0f, 1.0f);
    }

    float PlatformerCharacterBody3D::get_deceleration_curve_intensity() const {
        return decelerationCurveIntensity;
    }

    void PlatformerCharacterBody3D::set_deceleration_curve_intensity(const float value) {
        decelerationCurveIntensity = Math::clamp(value, 0.0f, 1.0f);
    }

    float PlatformerCharacterBody3D::ease_in(const float t, const float intensity) {
        const float power = 1.0f + intensity * 3.0f;  // Range: 1 (linear) to 4 (sharp)
        return Math::pow(t, power);
    }

    float PlatformerCharacterBody3D::ease_out(const float t, const float intensity) {
        const float power = 1.0f + intensity * 3.0f;
        return 1.0f - Math::pow(1.0f - t, power);
    }

    float PlatformerCharacterBody3D::get_rotation_speed() const {
        return rotationSpeed;
    }

    void PlatformerCharacterBody3D::set_rotation_speed(const float value) {
        rotationSpeed = value;
    }

    Vector3 PlatformerCharacterBody3D::get_gravity_up_direction() const {
        if (useCustomGravity) {
            return -customGravityDirection.normalized();
        }
        return -defaultGravityVec.normalized();
    }


    void PlatformerCharacterBody3D::_notification(const int p_what) {
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

    void PlatformerCharacterBody3D::_internal_enter_tree() {
        // Base class: nothing to do on enter tree
    }

    void PlatformerCharacterBody3D::_internal_ready() {
        // Cache the settings once at startup to save performance
        const ProjectSettings* settings = ProjectSettings::get_singleton();
        // Note: In GDExtension/C++, explicit casting from Variant is safer
        defaultGravityVec = settings->get_setting("physics/3d/default_gravity_vector");
        defaultGravityStrength = static_cast<float>(settings->get_setting("physics/3d/default_gravity"));

        // FORCE STOP any accumulated velocity from startup lag
        set_velocity(Vector3(0, 0, 0));
    }

    void PlatformerCharacterBody3D::_internal_physics_process(const double delta) {
        if (Engine::get_singleton()->is_editor_hint()) {
            return;
        }
        ApplyGravity(delta);
        move_and_slide();
    }

    void PlatformerCharacterBody3D::_internal_process(const double delta) {
        // Base class: nothing to do on process
    }

    void PlatformerCharacterBody3D::_physics_process(const double p_delta) {
        // This may not be called if GDScript overrides it.
        // Core logic is in _internal_physics_process() called via _notification().
    }

    void PlatformerCharacterBody3D::_ready() {
        // This may not be called if GDScript overrides it.
        // Core logic is in _internal_ready() called via _notification().
    }
} // namespace Rebel
