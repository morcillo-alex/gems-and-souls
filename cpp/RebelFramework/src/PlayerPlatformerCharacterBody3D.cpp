// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#include "Rebel/CharacterBody/PlayerPlatformerCharacterBody3D.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/math.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/input.hpp"

using namespace godot;

namespace Rebel::CharacterBody {

    struct PlayerCharacterBody3DSignals {
        static constexpr auto PLAYER_MOVEMENT_CHANGED = "player_movement_changed";
        static constexpr auto ATTACK1_RELEASED = "attack1_released";
        static constexpr auto ATTACK2_RELEASED = "attack2_released";
    };

    void PlayerPlatformerCharacterBody3D::setup_default_camera() {
        cameraSpringArm = cast_to<SpringArm3D>(get_node_or_null(NodePath("CameraSpringArm")));

        if (cameraSpringArm == nullptr) {
            cameraSpringArm = memnew(godot::SpringArm3D);
            cameraSpringArm->set_name("CameraSpringArm");

            // Default transform: height offset and tilt
            Transform3D transform;
            transform.origin = springArmOffset;
            transform.basis = transform.basis.rotated(Vector3(1.0f, 0.0f, 0.0f), Math::deg_to_rad(-15.0f));
            transform.basis = transform.basis.rotated(Vector3(0.0f, 1.0f, 0.0f), Math::deg_to_rad(180.0f));
            cameraSpringArm->set_transform(transform);
            cameraSpringArm->set_length(springArmLength);
            cameraSpringArm->set_collision_mask(1);

            add_child(cameraSpringArm);

            // This way we will apply global transform
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
            // Create Camera3D
            playerCamera = memnew(godot::Camera3D);
            playerCamera->set_name("PlayerCamera");

            cameraSpringArm->add_child(playerCamera);

            // Set the owner so the node is saved with a scene and visible in the editor
            if (get_owner() != nullptr) {
                playerCamera->set_owner(get_owner());
            } else if (Engine::get_singleton()->is_editor_hint()) {
                playerCamera->set_owner(get_owner() != nullptr ? get_owner() : this);
            }
        }
    }

    void PlayerPlatformerCharacterBody3D::_bind_methods() {
        using namespace godot;

        // Register utility signals
        ADD_SIGNAL(MethodInfo("player_movement_changed",
            PropertyInfo(Variant::RID, "instance"),
            PropertyInfo(Variant::VECTOR3, "velocity"),
            PropertyInfo(Variant::VECTOR2, "input_direction"),
            PropertyInfo(Variant::BOOL, "is_on_floor")));

        // Attack charge signals - emitted when attack button is released
        ADD_SIGNAL(MethodInfo("attack1_released",
            PropertyInfo(Variant::FLOAT, "charge_level")));

        ADD_SIGNAL(MethodInfo("attack2_released",
            PropertyInfo(Variant::FLOAT, "charge_level")));

        // Attack charge getters
        ClassDB::bind_method(D_METHOD("get_attack1_charge_level"), &PlayerPlatformerCharacterBody3D::get_attack1_charge_level);
        ClassDB::bind_method(D_METHOD("get_attack2_charge_level"), &PlayerPlatformerCharacterBody3D::get_attack2_charge_level);
        ClassDB::bind_method(D_METHOD("is_attack1_charging"), &PlayerPlatformerCharacterBody3D::is_attack1_charging);
        ClassDB::bind_method(D_METHOD("is_attack2_charging"), &PlayerPlatformerCharacterBody3D::is_attack2_charging);


        // Camera accessors
        ClassDB::bind_method(D_METHOD("get_spring_arm"), &PlayerPlatformerCharacterBody3D::get_spring_arm);
        ClassDB::bind_method(D_METHOD("get_camera"), &PlayerPlatformerCharacterBody3D::get_camera);
        ClassDB::bind_method(D_METHOD("activate_camera"), &PlayerPlatformerCharacterBody3D::activate_camera);

        ClassDB::bind_method(D_METHOD("get_spring_arm_offset"), &PlayerPlatformerCharacterBody3D::get_spring_arm_offset);
        ClassDB::bind_method(D_METHOD("set_spring_arm_offset", "offset"), &PlayerPlatformerCharacterBody3D::set_spring_arm_offset);

        ClassDB::bind_method(D_METHOD("get_springarm_default_length"), &PlayerPlatformerCharacterBody3D::get_springarm_default_length);
        ClassDB::bind_method(D_METHOD("set_springarm_default_length", "length"), &PlayerPlatformerCharacterBody3D::set_springarm_default_length);

        // Input action bindings
        ClassDB::bind_method(D_METHOD("set_move_left_action", "action_name"), &PlayerPlatformerCharacterBody3D::set_move_left_action);
        ClassDB::bind_method(D_METHOD("get_move_left_action"), &PlayerPlatformerCharacterBody3D::get_move_left_action);

        ClassDB::bind_method(D_METHOD("set_move_right_action", "action_name"), &PlayerPlatformerCharacterBody3D::set_move_right_action);
        ClassDB::bind_method(D_METHOD("get_move_right_action"), &PlayerPlatformerCharacterBody3D::get_move_right_action);

        ClassDB::bind_method(D_METHOD("set_move_forward_action", "action_name"), &PlayerPlatformerCharacterBody3D::set_move_forward_action);
        ClassDB::bind_method(D_METHOD("get_move_forward_action"), &PlayerPlatformerCharacterBody3D::get_move_forward_action);

        ClassDB::bind_method(D_METHOD("set_move_backward_action", "action_name"), &PlayerPlatformerCharacterBody3D::set_move_backward_action);
        ClassDB::bind_method(D_METHOD("get_move_backward_action"), &PlayerPlatformerCharacterBody3D::get_move_backward_action);

        ClassDB::bind_method(D_METHOD("set_attack_action1", "action_name"), &PlayerPlatformerCharacterBody3D::set_attack_action1);
        ClassDB::bind_method(D_METHOD("get_attack_action1"), &PlayerPlatformerCharacterBody3D::get_attack_action1);

        ClassDB::bind_method(D_METHOD("set_attack_action2", "action_name"), &PlayerPlatformerCharacterBody3D::set_attack_action2);
        ClassDB::bind_method(D_METHOD("get_attack_action2"), &PlayerPlatformerCharacterBody3D::get_attack_action2);


        ClassDB::bind_method(D_METHOD("set_jump_action", "action_name"), &PlayerPlatformerCharacterBody3D::set_jump_action);
        ClassDB::bind_method(D_METHOD("get_jump_action"), &PlayerPlatformerCharacterBody3D::get_jump_action);

        ClassDB::bind_method(D_METHOD("set_interact_action", "action_name"), &PlayerPlatformerCharacterBody3D::set_interact_action);
        ClassDB::bind_method(D_METHOD("get_interact_action"), &PlayerPlatformerCharacterBody3D::get_interact_action);

        ClassDB::bind_method(D_METHOD("set_pause_action", "action_name"), &PlayerPlatformerCharacterBody3D::set_pause_action);
        ClassDB::bind_method(D_METHOD("get_pause_action"), &PlayerPlatformerCharacterBody3D::get_pause_action);

        ClassDB::bind_method(D_METHOD("set_menu_action", "action_name"), &PlayerPlatformerCharacterBody3D::set_menu_action);
        ClassDB::bind_method(D_METHOD("get_menu_action"), &PlayerPlatformerCharacterBody3D::get_menu_action);

        ADD_GROUP("Third-Person Camera Settings", "");
        ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "spring_arm_offset"), "set_spring_arm_offset", "get_spring_arm_offset");
        ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "springarm_default_length", PROPERTY_HINT_RANGE, "0,100,0.1,or_greater"), "set_springarm_default_length", "get_springarm_default_length");

        // Register input action properties
        ADD_GROUP("Input Actions", "");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "move_left_action"), "set_move_left_action", "get_move_left_action");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "move_right_action"), "set_move_right_action", "get_move_right_action");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "move_forward_action"), "set_move_forward_action", "get_move_forward_action");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "move_backward_action"), "set_move_backward_action", "get_move_backward_action");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "jump_action"), "set_jump_action", "get_jump_action");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "interact_action"), "set_interact_action", "get_interact_action");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "primary_attack"), "set_attack_action1", "get_attack_action1");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "secondary_attack"), "set_attack_action2", "get_attack_action2");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "pause_action"), "set_pause_action", "get_pause_action");
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "menu_action"), "set_menu_action", "get_menu_action");
    }

    void PlayerPlatformerCharacterBody3D::apply_movement(const double delta) {
        const auto input = Input::get_singleton();
        const float f_delta = static_cast<float>(delta);

        // Get the up direction based on gravity settings
        const Vector3 up = get_gravity_up_direction();

        // Capture the CURRENT velocity (which already includes gravity from the parent class)
        const Vector3 current_velocity = get_velocity();

        // Get Input
        const Vector2 input_dir = input->get_vector(moveLeftAction, moveRightAction, moveForwardAction, moveBackwardAction);

        // Calculate Direction Relative to Camera
        Vector3 forward{0, 0, -1};
        Vector3 right{1, 0, 0};

        if (cameraSpringArm) {
            const Transform3D cam_xf = cameraSpringArm->get_global_transform();
            forward = -cam_xf.basis.get_column(2);
            right = cam_xf.basis.get_column(0);
            // Project onto plane perpendicular to up (instead of zeroing Y)
            forward = (forward - up * forward.dot(up)).normalized();
            right = (right - up * right.dot(up)).normalized();
        }

        const Vector3 move_direction = (right * input_dir.x - forward * input_dir.y).normalized();

        // Extract vertical component: projection onto up
        float vertical_speed = current_velocity.dot(up);
        const Vector3 vertical_velocity_component = up * vertical_speed;

        // Extract horizontal component: everything perpendicular to up
        Vector3 horizontal_velocity = current_velocity - vertical_velocity_component;

        const Vector3 target_velocity = move_direction * get_moving_speed();

        const bool hasInput = input_dir.length_squared() > 0.001f;

        if (hasInput) {
            // Check if direction changed significantly (opposite direction)
            const bool directionChanged = target_velocity.dot(targetVelocityAtPhaseStart) < 0;

            // Acceleration phase
            if (!isAccelerating || directionChanged) {
                // Just started accelerating or changed direction - reset timer
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
            // Deceleration phase
            if (isAccelerating) {
                // Just started decelerating - reset timer and capture start velocity
                isAccelerating = false;
                decelerationElapsed = 0.0f;
                velocityAtPhaseStart = horizontal_velocity;
            }
            decelerationElapsed += f_delta;

            const float t = Math::clamp(decelerationElapsed / get_deceleration_time(), 0.0f, 1.0f);
            const float eased_t = ease_out(t, get_deceleration_curve_intensity());

            horizontal_velocity = velocityAtPhaseStart.lerp(Vector3(), eased_t);
        }

        // Variable Jump System
        const bool onFloor = is_on_floor();
        const bool jumpJustPressed = input->is_action_just_pressed(jumpAction);
        const bool jumpPressed = input->is_action_pressed(jumpAction);
        const bool jumpJustReleased = input->is_action_just_released(jumpAction);

        // Initial jump: apply impulse when pressing jump while on floor
        if (onFloor && jumpJustPressed) {
            vertical_speed = get_jump_force();
            jumpHoldElapsed = 0.0f;
            isJumpHeld = true;
        }

        // Handle jump hold state
        if (isJumpHeld) {
            if (onFloor && !jumpJustPressed) {
                // Landed after jumping - reset state
                isJumpHeld = false;
            } else if (jumpJustReleased) {
                // Button was just released - cut velocity if still ascending
                if (vertical_speed > 0.0f) {
                    vertical_speed *= get_jump_release_cut_factor();
                }
                isJumpHeld = false;
            } else if (jumpPressed && jumpHoldElapsed < get_max_jump_hold_time() && !onFloor) {
                // Still holding in air - apply upward force
                vertical_speed += get_jump_hold_force() * f_delta;
                jumpHoldElapsed += f_delta;
            } else if (jumpHoldElapsed >= get_max_jump_hold_time()) {
                // Max hold time reached
                isJumpHeld = false;
            }
        }

        // Attack Charge System
        // Primary Attack
        if (input->is_action_just_pressed(attackAction1)) {
            isAttack1Charging = true;
            attack1ChargeElapsed = 0.0f;
        }
        if (isAttack1Charging) {
            if (input->is_action_pressed(attackAction1)) {
                attack1ChargeElapsed += f_delta;
            } else {
                // Button released - emit signal with charge level
                const float chargeLevel = Math::clamp(attack1ChargeElapsed / get_attack_charge_time(), 0.0f, 1.0f);
                emit_signal(PlayerCharacterBody3DSignals::ATTACK1_RELEASED, chargeLevel);
                isAttack1Charging = false;
                attack1ChargeElapsed = 0.0f;
            }
        }

        // Secondary Attack
        if (input->is_action_just_pressed(attackAction2)) {
            isAttack2Charging = true;
            attack2ChargeElapsed = 0.0f;
        }
        if (isAttack2Charging) {
            if (input->is_action_pressed(attackAction2)) {
                attack2ChargeElapsed += f_delta;
            } else {
                // Button released - emit signal with charge level
                const float chargeLevel = Math::clamp(attack2ChargeElapsed / get_attack_charge_time(), 0.0f, 1.0f);
                emit_signal(PlayerCharacterBody3DSignals::ATTACK2_RELEASED, chargeLevel);
                isAttack2Charging = false;
                attack2ChargeElapsed = 0.0f;
            }
        }

        // Recombine horizontal and vertical components
        const Vector3 final_velocity = horizontal_velocity + up * vertical_speed;

        set_velocity(final_velocity);

        emit_signal(PlayerCharacterBody3DSignals::PLAYER_MOVEMENT_CHANGED, get_rid(), final_velocity, input_dir, is_on_floor());

        move_and_slide();

        // Smooth Rotation to face movement direction (only when moving)
        if (input_dir.length_squared() > 0.001f && move_direction.length_squared() > 0.001f) {
            // Project move_direction onto movement plane (should already be there, but ensure it)
            Vector3 target_dir = (move_direction - up * move_direction.dot(up)).normalized();

            if (target_dir.length_squared() > 0.001f) {
                // Create target rotation looking along target_dir with up as the up vector
                // Note: looking_at() points -Z toward target, but character model has forward as +Z,
                // so we negate target_dir to make +Z point toward move_direction
                Basis target_basis = Basis::looking_at(-target_dir, up);
                Basis current_basis = get_transform().basis;

                // Slerp between current and target quaternions
                Quaternion current_quat = current_basis.get_quaternion();
                Quaternion target_quat = target_basis.get_quaternion();
                Quaternion result = current_quat.slerp(target_quat, get_rotation_speed() * f_delta);

                Transform3D t = get_transform();
                t.basis = Basis(result);
                set_transform(t);
            }
        }
    }

    void PlayerPlatformerCharacterBody3D::_internal_enter_tree() {
        // Call parent first
        PlatformerCharacterBody3D::_internal_enter_tree();
        // Then setup camera
        setup_default_camera();
    }

    void PlayerPlatformerCharacterBody3D::_internal_ready() {
        // Call parent first (caches gravity settings, resets velocity)
        PlatformerCharacterBody3D::_internal_ready();

        // Initialize acceleration/deceleration state
        accelerationElapsed = 0.0f;
        decelerationElapsed = 0.0f;
        velocityAtPhaseStart = Vector3();
        targetVelocityAtPhaseStart = Vector3();
        isAccelerating = false;

        // Initialize jump hold state
        jumpHoldElapsed = 0.0f;
        isJumpHeld = false;

        // Initialize attack charge state
        attack1ChargeElapsed = 0.0f;
        attack2ChargeElapsed = 0.0f;
        isAttack1Charging = false;
        isAttack2Charging = false;

        // Then activate camera
        activate_camera();
    }

    void PlayerPlatformerCharacterBody3D::_internal_physics_process(double delta) {
        if (Engine::get_singleton()->is_editor_hint()) {
            return;
        }
        // Apply gravity (from parent)
        ApplyGravity(delta);
        // Then apply player movement (replaces parent's move_and_slide)
        apply_movement(delta);
    }

    void PlayerPlatformerCharacterBody3D::_internal_process(double delta) {
        // Call parent first
        PlatformerCharacterBody3D::_internal_process(delta);

        if (Engine::get_singleton()->is_editor_hint()) {
            return;
        }

        if (cameraSpringArm) {
            // Smoothly follow the player's global position with an offset
            Vector3 target_pos = get_global_position();
            target_pos += springArmOffset;
            cameraSpringArm->set_global_position(target_pos);
        }
    }

    void PlayerPlatformerCharacterBody3D::_enter_tree() {
        // This may not be called if GDScript overrides it.
        // Core logic is in _internal_enter_tree() called via _notification().
    }

    void PlayerPlatformerCharacterBody3D::_ready() {
        // This may not be called if GDScript overrides it.
        // Core logic is in _internal_ready() called via _notification().
    }

    void PlayerPlatformerCharacterBody3D::activate_camera() const {
        // Make the camera current only at runtime, not in the editor
        if (!Engine::get_singleton()->is_editor_hint() && playerCamera != nullptr) {
            playerCamera->set_current(true);
        }
    }

    void PlayerPlatformerCharacterBody3D::_physics_process(double p_delta) {
        // This may not be called if GDScript overrides it.
        // Core logic is in _internal_physics_process() called via _notification().
    }

    void PlayerPlatformerCharacterBody3D::_process(double p_delta) {
        // This may not be called if GDScript overrides it.
        // Core logic is in _internal_process() called via _notification().
    }

    Vector3 PlayerPlatformerCharacterBody3D::get_spring_arm_offset() const {
        return springArmOffset;
    }

    void PlayerPlatformerCharacterBody3D::set_spring_arm_offset(const Vector3 &offset) {
        springArmOffset = offset;
        if (cameraSpringArm) {
            Transform3D transform = cameraSpringArm->get_transform();
            transform.origin = offset;
            cameraSpringArm->set_transform(transform);
        }
    }

    float PlayerPlatformerCharacterBody3D::get_springarm_default_length() const {
        return springArmLength;
    }

    void PlayerPlatformerCharacterBody3D::set_springarm_default_length(const float length) {
        springArmLength = length;
        if (cameraSpringArm) {
            cameraSpringArm->set_length(length);
        }
    }

    SpringArm3D* PlayerPlatformerCharacterBody3D::get_spring_arm() const {
        return cameraSpringArm;
    }

    Camera3D* PlayerPlatformerCharacterBody3D::get_camera() const {
        return playerCamera;
    }

    String PlayerPlatformerCharacterBody3D::get_move_left_action() const {
        return moveLeftAction;
    }

    String PlayerPlatformerCharacterBody3D::get_move_right_action() const {
        return moveRightAction;
    }

    String PlayerPlatformerCharacterBody3D::get_move_forward_action() const {
        return moveForwardAction;
    }

    String PlayerPlatformerCharacterBody3D::get_move_backward_action() const {
        return moveBackwardAction;
    }

    String PlayerPlatformerCharacterBody3D::get_jump_action() const {
        return jumpAction;
    }

    String PlayerPlatformerCharacterBody3D::get_interact_action() const {
        return interactAction;
    }

    String PlayerPlatformerCharacterBody3D::get_pause_action() const {
        return pauseAction;
    }

    String PlayerPlatformerCharacterBody3D::get_menu_action() const {
        return menuAction;
    }

    String PlayerPlatformerCharacterBody3D::get_attack_action1() const {
        return attackAction1;
    }

    String PlayerPlatformerCharacterBody3D::get_attack_action2() const {
        return attackAction2;
    }

    void PlayerPlatformerCharacterBody3D::set_attack_action1(const String &action_name) {
        attackAction1 = action_name;
    }

    void PlayerPlatformerCharacterBody3D::set_attack_action2(const String &action_name) {
        attackAction2 = action_name;
    }

    void PlayerPlatformerCharacterBody3D::set_move_left_action(const String &action_name) {
        moveLeftAction = action_name;
    }

    void PlayerPlatformerCharacterBody3D::set_move_right_action(const String &action_name) {
        moveRightAction = action_name;
    }

    void PlayerPlatformerCharacterBody3D::set_move_forward_action(const String &action_name) {
        moveForwardAction = action_name;
    }

    void PlayerPlatformerCharacterBody3D::set_move_backward_action(const String &action_name) {
        moveBackwardAction = action_name;
    }

    void PlayerPlatformerCharacterBody3D::set_jump_action(const String &action_name) {
        jumpAction = action_name;
    }

    void PlayerPlatformerCharacterBody3D::set_interact_action(const String &action_name) {
        interactAction = action_name;
    }

    void PlayerPlatformerCharacterBody3D::set_pause_action(const String &action_name) {
        pauseAction = action_name;
    }

    void PlayerPlatformerCharacterBody3D::set_menu_action(const String &action_name) {
        menuAction = action_name;
    }

    float PlayerPlatformerCharacterBody3D::get_attack1_charge_level() const {
        if (!isAttack1Charging) {
            return 0.0f;
        }
        return Math::clamp(attack1ChargeElapsed / get_attack_charge_time(), 0.0f, 1.0f);
    }

    float PlayerPlatformerCharacterBody3D::get_attack2_charge_level() const {
        if (!isAttack2Charging) {
            return 0.0f;
        }
        return Math::clamp(attack2ChargeElapsed / get_attack_charge_time(), 0.0f, 1.0f);
    }

    bool PlayerPlatformerCharacterBody3D::is_attack1_charging() const {
        return isAttack1Charging;
    }

    bool PlayerPlatformerCharacterBody3D::is_attack2_charging() const {
        return isAttack2Charging;
    }
} // namespace Rebel