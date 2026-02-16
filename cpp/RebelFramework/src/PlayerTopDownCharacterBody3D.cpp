// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#include "Rebel/CharacterBody/PlayerTopDownCharacterBody3D.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/math.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/input.hpp"

using namespace godot;

namespace Rebel::CharacterBody {

    struct TopDownPlayerSignals {
        static constexpr auto PLAYER_MOVEMENT_CHANGED = "player_movement_changed";
        static constexpr auto ATTACK1_RELEASED = "attack1_released";
        static constexpr auto ATTACK2_RELEASED = "attack2_released";
        static constexpr auto DODGE_PERFORMED = "dodge_performed";
    };

    void PlayerTopDownCharacterBody3D::setup_default_camera() {
        cameraSpringArm = cast_to<SpringArm3D>(get_node_or_null(NodePath("CameraSpringArm")));

        if (cameraSpringArm == nullptr) {
            cameraSpringArm = memnew(godot::SpringArm3D);
            cameraSpringArm->set_name("CameraSpringArm");

            // Build transform from configurable pitch and yaw angles
            Transform3D transform;
            transform.origin = springArmOffset;
            transform.basis = transform.basis.rotated(Vector3(1.0f, 0.0f, 0.0f), Math::deg_to_rad(springArmPitchAngle));
            transform.basis = transform.basis.rotated(Vector3(0.0f, 1.0f, 0.0f), Math::deg_to_rad(springArmYawAngle));
            cameraSpringArm->set_transform(transform);
            cameraSpringArm->set_length(springArmLength);
            cameraSpringArm->set_collision_mask(1);

            add_child(cameraSpringArm);

            // Apply global transform
            cameraSpringArm->set_as_top_level(true);

            // Set the owner so the node is saved with a scene and visible in the editor
            if (get_owner() != nullptr) {
                cameraSpringArm->set_owner(get_owner());
            } else if (Engine::get_singleton()->is_editor_hint()) {
                cameraSpringArm->set_owner(this);
            }
        }

        // Check if Camera already exists
        playerCamera = cast_to<Camera3D>(cameraSpringArm->get_node_or_null(NodePath("PlayerCamera")));

        if (playerCamera == nullptr) {
            playerCamera = memnew(godot::Camera3D);
            playerCamera->set_name("PlayerCamera");

            cameraSpringArm->add_child(playerCamera);

            if (get_owner() != nullptr) {
                playerCamera->set_owner(get_owner());
            } else if (Engine::get_singleton()->is_editor_hint()) {
                playerCamera->set_owner(get_owner() != nullptr ? get_owner() : this);
            }
        }
    }

    void PlayerTopDownCharacterBody3D::_bind_methods() {
        using namespace godot;

        // Register signals
        ADD_SIGNAL(MethodInfo("player_movement_changed",
            PropertyInfo(Variant::RID, "instance"),
            PropertyInfo(Variant::VECTOR3, "velocity"),
            PropertyInfo(Variant::VECTOR2, "input_direction"),
            PropertyInfo(Variant::BOOL, "is_on_floor")));

        ADD_SIGNAL(MethodInfo("attack1_released",
            PropertyInfo(Variant::FLOAT, "charge_level")));

        ADD_SIGNAL(MethodInfo("attack2_released",
            PropertyInfo(Variant::FLOAT, "charge_level")));

        ADD_SIGNAL(MethodInfo("dodge_performed",
            PropertyInfo(Variant::VECTOR2, "direction")));

        // Attack charge getters
        ClassDB::bind_method(D_METHOD("get_attack1_charge_level"), &PlayerTopDownCharacterBody3D::get_attack1_charge_level);
        ClassDB::bind_method(D_METHOD("get_attack2_charge_level"), &PlayerTopDownCharacterBody3D::get_attack2_charge_level);
        ClassDB::bind_method(D_METHOD("is_attack1_charging"), &PlayerTopDownCharacterBody3D::is_attack1_charging);
        ClassDB::bind_method(D_METHOD("is_attack2_charging"), &PlayerTopDownCharacterBody3D::is_attack2_charging);

        // Camera accessors
        ClassDB::bind_method(D_METHOD("get_spring_arm"), &PlayerTopDownCharacterBody3D::get_spring_arm);
        ClassDB::bind_method(D_METHOD("get_camera"), &PlayerTopDownCharacterBody3D::get_camera);
        ClassDB::bind_method(D_METHOD("activate_camera"), &PlayerTopDownCharacterBody3D::activate_camera);

        ClassDB::bind_method(D_METHOD("get_spring_arm_offset"), &PlayerTopDownCharacterBody3D::get_spring_arm_offset);
        ClassDB::bind_method(D_METHOD("set_spring_arm_offset", "offset"), &PlayerTopDownCharacterBody3D::set_spring_arm_offset);

        ClassDB::bind_method(D_METHOD("get_springarm_default_length"), &PlayerTopDownCharacterBody3D::get_springarm_default_length);
        ClassDB::bind_method(D_METHOD("set_springarm_default_length", "length"), &PlayerTopDownCharacterBody3D::set_springarm_default_length);

        ClassDB::bind_method(D_METHOD("get_spring_arm_pitch_angle"), &PlayerTopDownCharacterBody3D::get_spring_arm_pitch_angle);
        ClassDB::bind_method(D_METHOD("set_spring_arm_pitch_angle", "angle"), &PlayerTopDownCharacterBody3D::set_spring_arm_pitch_angle);

        ClassDB::bind_method(D_METHOD("get_spring_arm_yaw_angle"), &PlayerTopDownCharacterBody3D::get_spring_arm_yaw_angle);
        ClassDB::bind_method(D_METHOD("set_spring_arm_yaw_angle", "angle"), &PlayerTopDownCharacterBody3D::set_spring_arm_yaw_angle);

        // Input action bindings
        ClassDB::bind_method(D_METHOD("set_move_left_action", "action_name"), &PlayerTopDownCharacterBody3D::set_move_left_action);
        ClassDB::bind_method(D_METHOD("get_move_left_action"), &PlayerTopDownCharacterBody3D::get_move_left_action);

        ClassDB::bind_method(D_METHOD("set_move_right_action", "action_name"), &PlayerTopDownCharacterBody3D::set_move_right_action);
        ClassDB::bind_method(D_METHOD("get_move_right_action"), &PlayerTopDownCharacterBody3D::get_move_right_action);

        ClassDB::bind_method(D_METHOD("set_move_forward_action", "action_name"), &PlayerTopDownCharacterBody3D::set_move_forward_action);
        ClassDB::bind_method(D_METHOD("get_move_forward_action"), &PlayerTopDownCharacterBody3D::get_move_forward_action);

        ClassDB::bind_method(D_METHOD("set_move_backward_action", "action_name"), &PlayerTopDownCharacterBody3D::set_move_backward_action);
        ClassDB::bind_method(D_METHOD("get_move_backward_action"), &PlayerTopDownCharacterBody3D::get_move_backward_action);

        ClassDB::bind_method(D_METHOD("set_interact_action", "action_name"), &PlayerTopDownCharacterBody3D::set_interact_action);
        ClassDB::bind_method(D_METHOD("get_interact_action"), &PlayerTopDownCharacterBody3D::get_interact_action);

        ClassDB::bind_method(D_METHOD("set_attack_action1", "action_name"), &PlayerTopDownCharacterBody3D::set_attack_action1);
        ClassDB::bind_method(D_METHOD("get_attack_action1"), &PlayerTopDownCharacterBody3D::get_attack_action1);

        ClassDB::bind_method(D_METHOD("set_attack_action2", "action_name"), &PlayerTopDownCharacterBody3D::set_attack_action2);
        ClassDB::bind_method(D_METHOD("get_attack_action2"), &PlayerTopDownCharacterBody3D::get_attack_action2);

        ClassDB::bind_method(D_METHOD("set_dodge_action", "action_name"), &PlayerTopDownCharacterBody3D::set_dodge_action);
        ClassDB::bind_method(D_METHOD("get_dodge_action"), &PlayerTopDownCharacterBody3D::get_dodge_action);

        ClassDB::bind_method(D_METHOD("set_pause_action", "action_name"), &PlayerTopDownCharacterBody3D::set_pause_action);
        ClassDB::bind_method(D_METHOD("get_pause_action"), &PlayerTopDownCharacterBody3D::get_pause_action);

        ClassDB::bind_method(D_METHOD("set_menu_action", "action_name"), &PlayerTopDownCharacterBody3D::set_menu_action);
        ClassDB::bind_method(D_METHOD("get_menu_action"), &PlayerTopDownCharacterBody3D::get_menu_action);

        // Register properties - Camera
        ADD_GROUP("Top-Down Camera Settings", "");
        ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "spring_arm_offset"), "set_spring_arm_offset", "get_spring_arm_offset");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "springarm_default_length", PROPERTY_HINT_RANGE, "0,100,0.1,or_greater"), "set_springarm_default_length", "get_springarm_default_length");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "spring_arm_pitch_angle", PROPERTY_HINT_RANGE, "-90,-15,0.5"), "set_spring_arm_pitch_angle", "get_spring_arm_pitch_angle");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "spring_arm_yaw_angle", PROPERTY_HINT_RANGE, "-180,180,0.5"), "set_spring_arm_yaw_angle", "get_spring_arm_yaw_angle");

        // Register input action properties
        ADD_GROUP("Input Actions", "");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "move_left_action"), "set_move_left_action", "get_move_left_action");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "move_right_action"), "set_move_right_action", "get_move_right_action");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "move_forward_action"), "set_move_forward_action", "get_move_forward_action");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "move_backward_action"), "set_move_backward_action", "get_move_backward_action");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "interact_action"), "set_interact_action", "get_interact_action");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "primary_attack"), "set_attack_action1", "get_attack_action1");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "secondary_attack"), "set_attack_action2", "get_attack_action2");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "dodge_action"), "set_dodge_action", "get_dodge_action");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "pause_action"), "set_pause_action", "get_pause_action");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "menu_action"), "set_menu_action", "get_menu_action");
    }

    void PlayerTopDownCharacterBody3D::apply_movement(const double delta) {
        const auto input = Input::get_singleton();
        const float f_delta = static_cast<float>(delta);

        const Vector3 current_velocity = get_velocity();

        // Get Input
        const Vector2 input_dir = input->get_vector(moveLeftAction, moveRightAction, moveForwardAction, moveBackwardAction);

        // Calculate Direction Relative to Camera
        // For top-down, we need to handle near-vertical camera angles where the forward
        // projection onto the ground plane approaches zero length.
        Vector3 forward{0, 0, -1};
        Vector3 right{1, 0, 0};

        if (cameraSpringArm) {
            const Transform3D cam_xf = cameraSpringArm->get_global_transform();
            Vector3 cam_forward = -cam_xf.basis.get_column(2);
            Vector3 cam_right = cam_xf.basis.get_column(0);

            // Project onto XZ plane (remove Y component)
            Vector3 projected_forward = Vector3(cam_forward.x, 0.0f, cam_forward.z);
            Vector3 projected_right = Vector3(cam_right.x, 0.0f, cam_right.z);

            // Check if projected forward is too short (near-vertical camera angle)
            constexpr float EPSILON = 0.01f;
            if (projected_forward.length_squared() < EPSILON) {
                // Fall back to using yaw angle to construct forward/right directions
                const float yaw_rad = Math::deg_to_rad(springArmYawAngle);
                forward = Vector3(-Math::sin(yaw_rad), 0.0f, -Math::cos(yaw_rad));
                right = Vector3(Math::cos(yaw_rad), 0.0f, -Math::sin(yaw_rad));
            } else {
                forward = projected_forward.normalized();
                right = projected_right.normalized();
            }
        }

        const Vector3 move_direction = (right * input_dir.x - forward * input_dir.y).normalized();

        // For top-down, movement is purely horizontal (XZ plane)
        // Extract only the horizontal component of current velocity
        Vector3 horizontal_velocity = Vector3(current_velocity.x, 0.0f, current_velocity.z);
        const float vertical_speed = current_velocity.y;

        const Vector3 target_velocity = move_direction * get_moving_speed();

        const bool hasInput = input_dir.length_squared() > 0.001f;

        if (hasInput) {
            const bool directionChanged = target_velocity.dot(targetVelocityAtPhaseStart) < 0;

            if (!isAccelerating || directionChanged) {
                isAccelerating = true;
                accelerationElapsed = 0.0f;
                velocityAtPhaseStart = horizontal_velocity;
                targetVelocityAtPhaseStart = target_velocity;
            }
            accelerationElapsed += f_delta;

            const float t = Math::clamp(accelerationElapsed / get_acceleration_time(), 0.0f, 1.0f);
            const float eased_t = ease_in(t, get_acceleration_curve_intensity());

            horizontal_velocity = velocityAtPhaseStart.lerp(target_velocity, eased_t);
        } else {
            if (isAccelerating) {
                isAccelerating = false;
                decelerationElapsed = 0.0f;
                velocityAtPhaseStart = horizontal_velocity;
            }
            decelerationElapsed += f_delta;

            const float t = Math::clamp(decelerationElapsed / get_deceleration_time(), 0.0f, 1.0f);
            const float eased_t = ease_out(t, get_deceleration_curve_intensity());

            horizontal_velocity = velocityAtPhaseStart.lerp(Vector3(), eased_t);
        }

        // Attack Charge System - Primary Attack
        if (input->is_action_just_pressed(attackAction1)) {
            isAttack1Charging = true;
            attack1ChargeElapsed = 0.0f;
        }
        if (isAttack1Charging) {
            if (input->is_action_pressed(attackAction1)) {
                attack1ChargeElapsed += f_delta;
            } else {
                const float chargeLevel = Math::clamp(attack1ChargeElapsed / get_attack_charge_time(), 0.0f, 1.0f);
                emit_signal(TopDownPlayerSignals::ATTACK1_RELEASED, chargeLevel);
                isAttack1Charging = false;
                attack1ChargeElapsed = 0.0f;
            }
        }

        // Attack Charge System - Secondary Attack
        if (input->is_action_just_pressed(attackAction2)) {
            isAttack2Charging = true;
            attack2ChargeElapsed = 0.0f;
        }
        if (isAttack2Charging) {
            if (input->is_action_pressed(attackAction2)) {
                attack2ChargeElapsed += f_delta;
            } else {
                const float chargeLevel = Math::clamp(attack2ChargeElapsed / get_attack_charge_time(), 0.0f, 1.0f);
                emit_signal(TopDownPlayerSignals::ATTACK2_RELEASED, chargeLevel);
                isAttack2Charging = false;
                attack2ChargeElapsed = 0.0f;
            }
        }

        // Dodge - emit signal with current input direction, game code handles implementation
        if (input->is_action_just_pressed(dodgeAction)) {
            emit_signal(TopDownPlayerSignals::DODGE_PERFORMED, input_dir);
        }

        // Recombine: horizontal movement + any vertical from optional gravity
        const Vector3 final_velocity = horizontal_velocity + Vector3(0.0f, vertical_speed, 0.0f);

        set_velocity(final_velocity);

        emit_signal(TopDownPlayerSignals::PLAYER_MOVEMENT_CHANGED, get_rid(), final_velocity, input_dir, is_on_floor());

        move_and_slide();

        // Smooth SLERP rotation toward movement direction (only when moving)
        if (input_dir.length_squared() > 0.001f && move_direction.length_squared() > 0.001f) {
            Vector3 target_dir = Vector3(move_direction.x, 0.0f, move_direction.z).normalized();

            if (target_dir.length_squared() > 0.001f) {
                Basis target_basis = Basis::looking_at(-target_dir, Vector3(0, 1, 0));
                Basis current_basis = get_transform().basis;

                Quaternion current_quat = current_basis.get_quaternion();
                Quaternion target_quat = target_basis.get_quaternion();
                Quaternion result = current_quat.slerp(target_quat, get_rotation_speed() * f_delta);

                Transform3D t = get_transform();
                t.basis = Basis(result);
                set_transform(t);
            }
        }
    }

    void PlayerTopDownCharacterBody3D::_internal_enter_tree() {
        TopDownCharacterBody3D::_internal_enter_tree();
        setup_default_camera();
    }

    void PlayerTopDownCharacterBody3D::_internal_ready() {
        TopDownCharacterBody3D::_internal_ready();

        // Initialize acceleration/deceleration state
        accelerationElapsed = 0.0f;
        decelerationElapsed = 0.0f;
        velocityAtPhaseStart = Vector3();
        targetVelocityAtPhaseStart = Vector3();
        isAccelerating = false;

        // Initialize attack charge state
        attack1ChargeElapsed = 0.0f;
        attack2ChargeElapsed = 0.0f;
        isAttack1Charging = false;
        isAttack2Charging = false;

        activate_camera();
    }

    void PlayerTopDownCharacterBody3D::_internal_physics_process(double delta) {
        if (Engine::get_singleton()->is_editor_hint()) {
            return;
        }
        // Apply optional gravity (from parent)
        ApplyGravity(delta);
        // Then apply player movement (replaces parent's move_and_slide)
        apply_movement(delta);
    }

    void PlayerTopDownCharacterBody3D::_internal_process(double delta) {
        TopDownCharacterBody3D::_internal_process(delta);

        if (Engine::get_singleton()->is_editor_hint()) {
            return;
        }

        if (cameraSpringArm) {
            Vector3 target_pos = get_global_position();
            target_pos += springArmOffset;
            cameraSpringArm->set_global_position(target_pos);
        }
    }

    void PlayerTopDownCharacterBody3D::_enter_tree() {
        // Core logic is in _internal_enter_tree() called via _notification().
    }

    void PlayerTopDownCharacterBody3D::_ready() {
        // Core logic is in _internal_ready() called via _notification().
    }

    void PlayerTopDownCharacterBody3D::activate_camera() const {
        if (!Engine::get_singleton()->is_editor_hint() && playerCamera != nullptr) {
            playerCamera->set_current(true);
        }
    }

    void PlayerTopDownCharacterBody3D::_physics_process(double p_delta) {
        // Core logic is in _internal_physics_process() called via _notification().
    }

    void PlayerTopDownCharacterBody3D::_process(double p_delta) {
        // Core logic is in _internal_process() called via _notification().
    }

    Vector3 PlayerTopDownCharacterBody3D::get_spring_arm_offset() const {
        return springArmOffset;
    }

    void PlayerTopDownCharacterBody3D::set_spring_arm_offset(const Vector3 &offset) {
        springArmOffset = offset;
        if (cameraSpringArm) {
            Transform3D transform = cameraSpringArm->get_transform();
            transform.origin = offset;
            cameraSpringArm->set_transform(transform);
        }
    }

    float PlayerTopDownCharacterBody3D::get_springarm_default_length() const {
        return springArmLength;
    }

    void PlayerTopDownCharacterBody3D::set_springarm_default_length(const float length) {
        springArmLength = length;
        if (cameraSpringArm) {
            cameraSpringArm->set_length(length);
        }
    }

    float PlayerTopDownCharacterBody3D::get_spring_arm_pitch_angle() const {
        return springArmPitchAngle;
    }

    void PlayerTopDownCharacterBody3D::set_spring_arm_pitch_angle(const float angle) {
        springArmPitchAngle = Math::clamp(angle, -90.0f, -15.0f);
        if (cameraSpringArm) {
            Transform3D transform;
            transform.origin = springArmOffset;
            transform.basis = transform.basis.rotated(Vector3(1.0f, 0.0f, 0.0f), Math::deg_to_rad(springArmPitchAngle));
            transform.basis = transform.basis.rotated(Vector3(0.0f, 1.0f, 0.0f), Math::deg_to_rad(springArmYawAngle));
            cameraSpringArm->set_transform(transform);
        }
    }

    float PlayerTopDownCharacterBody3D::get_spring_arm_yaw_angle() const {
        return springArmYawAngle;
    }

    void PlayerTopDownCharacterBody3D::set_spring_arm_yaw_angle(const float angle) {
        springArmYawAngle = angle;
        if (cameraSpringArm) {
            Transform3D transform;
            transform.origin = springArmOffset;
            transform.basis = transform.basis.rotated(Vector3(1.0f, 0.0f, 0.0f), Math::deg_to_rad(springArmPitchAngle));
            transform.basis = transform.basis.rotated(Vector3(0.0f, 1.0f, 0.0f), Math::deg_to_rad(springArmYawAngle));
            cameraSpringArm->set_transform(transform);
        }
    }

    SpringArm3D* PlayerTopDownCharacterBody3D::get_spring_arm() const {
        return cameraSpringArm;
    }

    Camera3D* PlayerTopDownCharacterBody3D::get_camera() const {
        return playerCamera;
    }

    String PlayerTopDownCharacterBody3D::get_move_left_action() const {
        return moveLeftAction;
    }

    String PlayerTopDownCharacterBody3D::get_move_right_action() const {
        return moveRightAction;
    }

    String PlayerTopDownCharacterBody3D::get_move_forward_action() const {
        return moveForwardAction;
    }

    String PlayerTopDownCharacterBody3D::get_move_backward_action() const {
        return moveBackwardAction;
    }

    String PlayerTopDownCharacterBody3D::get_interact_action() const {
        return interactAction;
    }

    String PlayerTopDownCharacterBody3D::get_attack_action1() const {
        return attackAction1;
    }

    String PlayerTopDownCharacterBody3D::get_attack_action2() const {
        return attackAction2;
    }

    String PlayerTopDownCharacterBody3D::get_dodge_action() const {
        return dodgeAction;
    }

    String PlayerTopDownCharacterBody3D::get_pause_action() const {
        return pauseAction;
    }

    String PlayerTopDownCharacterBody3D::get_menu_action() const {
        return menuAction;
    }

    void PlayerTopDownCharacterBody3D::set_move_left_action(const String &action_name) {
        moveLeftAction = action_name;
    }

    void PlayerTopDownCharacterBody3D::set_move_right_action(const String &action_name) {
        moveRightAction = action_name;
    }

    void PlayerTopDownCharacterBody3D::set_move_forward_action(const String &action_name) {
        moveForwardAction = action_name;
    }

    void PlayerTopDownCharacterBody3D::set_move_backward_action(const String &action_name) {
        moveBackwardAction = action_name;
    }

    void PlayerTopDownCharacterBody3D::set_interact_action(const String &action_name) {
        interactAction = action_name;
    }

    void PlayerTopDownCharacterBody3D::set_attack_action1(const String &action_name) {
        attackAction1 = action_name;
    }

    void PlayerTopDownCharacterBody3D::set_attack_action2(const String &action_name) {
        attackAction2 = action_name;
    }

    void PlayerTopDownCharacterBody3D::set_dodge_action(const String &action_name) {
        dodgeAction = action_name;
    }

    void PlayerTopDownCharacterBody3D::set_pause_action(const String &action_name) {
        pauseAction = action_name;
    }

    void PlayerTopDownCharacterBody3D::set_menu_action(const String &action_name) {
        menuAction = action_name;
    }

    float PlayerTopDownCharacterBody3D::get_attack1_charge_level() const {
        if (!isAttack1Charging) {
            return 0.0f;
        }
        return Math::clamp(attack1ChargeElapsed / get_attack_charge_time(), 0.0f, 1.0f);
    }

    float PlayerTopDownCharacterBody3D::get_attack2_charge_level() const {
        if (!isAttack2Charging) {
            return 0.0f;
        }
        return Math::clamp(attack2ChargeElapsed / get_attack_charge_time(), 0.0f, 1.0f);
    }

    bool PlayerTopDownCharacterBody3D::is_attack1_charging() const {
        return isAttack1Charging;
    }

    bool PlayerTopDownCharacterBody3D::is_attack2_charging() const {
        return isAttack2Charging;
    }
} // namespace Rebel::CharacterBody
