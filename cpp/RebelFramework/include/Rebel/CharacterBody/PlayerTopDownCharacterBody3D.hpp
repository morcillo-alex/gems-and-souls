// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#pragma once
#include "TopDownCharacterBody3D.hpp"
#include "godot_cpp/classes/spring_arm3d.hpp"
#include "godot_cpp/classes/camera3d.hpp"

namespace Rebel::CharacterBody {

/**
 * @brief A player character body for top-down rogue-like games with a configurable camera.
 *
 * This class extends TopDownCharacterBody3D and automatically adds a SpringArm3D and Camera3D
 * as child nodes. The camera pitch angle is configurable: -90 degrees for full top-down,
 * -50 degrees (default) for a Moonlighter 2-style isometric perspective.
 *
 * Movement is purely on the XZ plane, camera-relative. Includes a dodge signal for
 * rogue-like dodge mechanics (implementation deferred to game code).
 */
class REBEL_FRAMEWORK PlayerTopDownCharacterBody3D : public TopDownCharacterBody3D {
    GDCLASS(PlayerTopDownCharacterBody3D, TopDownCharacterBody3D);

private:

    /** Acceleration/deceleration state tracking */

    /** Time elapsed since the acceleration phase started. */
    float accelerationElapsed{0.0f};

    /** Time elapsed since the deceleration phase started. */
    float decelerationElapsed{0.0f};

    /** Horizontal velocity when the current phase began. */
    godot::Vector3 velocityAtPhaseStart{0.0f, 0.0f, 0.0f};

    /** Target velocity when the acceleration phase began (for direction change detection). */
    godot::Vector3 targetVelocityAtPhaseStart{0.0f, 0.0f, 0.0f};

    /** Whether currently in acceleration phase. */
    bool isAccelerating{false};

    /** Attack charge state tracking */

    /** Time elapsed charging the primary attack. */
    float attack1ChargeElapsed{0.0f};

    /** Time elapsed charging a secondary attack. */
    float attack2ChargeElapsed{0.0f};

    /** Whether a primary attack is currently being charged. */
    bool isAttack1Charging{false};

    /** Whether a secondary attack is currently being charged. */
    bool isAttack2Charging{false};

    /** Camera components */

    /** The spring arm used to position the camera above the player. */
    godot::SpringArm3D* cameraSpringArm = nullptr;

    /** The camera that follows the player. */
    godot::Camera3D* playerCamera = nullptr;

    /** Default spring arm offset from the player character. */
    godot::Vector3 springArmOffset{0.0f, 0.0f, 0.0f};

    /** Default spring arm length (longer for overhead views). */
    float springArmLength{12.0f};

    /** Camera pitch angle in degrees. -90 = full top-down, -50 = Moonlighter 2-style. Range: -90 to -15. */
    float springArmPitchAngle{-50.0f};

    /** Camera yaw angle in degrees. Allows rotating the camera's viewing direction. */
    float springArmYawAngle{0.0f};

    /** Camera field of view in degrees. Lower values reduce perspective distortion. */
    float cameraFov{45.0f};

    /** Input action names **/

    godot::String moveLeftAction{"move_left"};
    godot::String moveRightAction{"move_right"};
    godot::String moveForwardAction{"move_forward"};
    godot::String moveBackwardAction{"move_backward"};
    godot::String interactAction{"interact"};
    godot::String attackAction1{"attack_1"};
    godot::String attackAction2{"attack_2"};
    godot::String dodgeAction{"dodge"};
    godot::String pauseAction{"pause"};
    godot::String menuAction{"menu"};

    /**
     * @brief Sets up the default camera and spring arm if they do not already exist.
     */
    void setup_default_camera();

    /**
     * @brief Applies the configured pitch, yaw, offset, length, and FOV to the spring arm and camera.
     *
     * Called after setup_default_camera() and from property setters to ensure the C++ property
     * values always override any stale transforms saved in the scene file.
     */
    void apply_spring_arm_transform();

protected:
    void _internal_enter_tree() override;
    void _internal_ready() override;
    void _internal_physics_process(double delta) override;
    void _internal_process(double delta) override;

    static void _bind_methods();

public:
    void _enter_tree() override;

    /**
     * @brief Applies movement logic for the player character on the XZ plane.
     * @param delta The frame time delta.
     */
    void apply_movement(double delta);

    void _ready() override;
    void _physics_process(double p_delta) override;
    void _process(double p_delta) override;

    void activate_camera() const;

    [[nodiscard]] godot::Vector3 get_spring_arm_offset() const;
    void set_spring_arm_offset(const godot::Vector3& offset);

    [[nodiscard]] float get_springarm_default_length() const;
    void set_springarm_default_length(float length);

    [[nodiscard]] float get_spring_arm_pitch_angle() const;
    void set_spring_arm_pitch_angle(float angle);

    [[nodiscard]] float get_spring_arm_yaw_angle() const;
    void set_spring_arm_yaw_angle(float angle);

    [[nodiscard]] float get_camera_fov() const;
    void set_camera_fov(float fov);

    [[nodiscard]] godot::SpringArm3D* get_spring_arm() const;
    [[nodiscard]] godot::Camera3D* get_camera() const;

    // Input action accessors
    [[nodiscard]] godot::String get_move_left_action() const;
    [[nodiscard]] godot::String get_move_right_action() const;
    [[nodiscard]] godot::String get_move_forward_action() const;
    [[nodiscard]] godot::String get_move_backward_action() const;
    [[nodiscard]] godot::String get_interact_action() const;
    [[nodiscard]] godot::String get_attack_action1() const;
    [[nodiscard]] godot::String get_attack_action2() const;
    [[nodiscard]] godot::String get_dodge_action() const;
    [[nodiscard]] godot::String get_pause_action() const;
    [[nodiscard]] godot::String get_menu_action() const;

    void set_move_left_action(const godot::String& action_name);
    void set_move_right_action(const godot::String& action_name);
    void set_move_forward_action(const godot::String& action_name);
    void set_move_backward_action(const godot::String& action_name);
    void set_interact_action(const godot::String& action_name);
    void set_attack_action1(const godot::String& action_name);
    void set_attack_action2(const godot::String& action_name);
    void set_dodge_action(const godot::String& action_name);
    void set_pause_action(const godot::String& action_name);
    void set_menu_action(const godot::String& action_name);

    [[nodiscard]] float get_attack1_charge_level() const;
    [[nodiscard]] float get_attack2_charge_level() const;
    [[nodiscard]] bool is_attack1_charging() const;
    [[nodiscard]] bool is_attack2_charging() const;
};

} // namespace Rebel::CharacterBody
