// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#pragma once
#include "PlatformerCharacterBody3D.hpp"
#include "godot_cpp/classes/spring_arm3d.hpp"
#include "godot_cpp/classes/camera3d.hpp"

namespace Rebel::CharacterBody {

/**
 * @brief A player character body for platformer games with a default camera setup.
 * This class extends PlatformerCharacterBody3D and automatically adds a SpringArm3D and Camera3D
 * as child nodes for third-person camera functionality. It provides the baseline functionality needed
 * for having a player-controlled character with a follow camera in a 3D platformer environment.
 */
class REBEL_FRAMEWORK PlayerPlatformerCharacterBody3D : public PlatformerCharacterBody3D {
    GDCLASS(PlayerPlatformerCharacterBody3D, PlatformerCharacterBody3D);

private:

    /** Acceleration/deceleration state tracking */

    /** Time elapsed since acceleration phase started. */
    float accelerationElapsed{0.0f};

    /** Time elapsed since deceleration phase started. */
    float decelerationElapsed{0.0f};

    /** Horizontal velocity when the current phase began. */
    godot::Vector3 velocityAtPhaseStart{0.0f, 0.0f, 0.0f};

    /** Target velocity when acceleration phase began (for direction change detection). */
    godot::Vector3 targetVelocityAtPhaseStart{0.0f, 0.0f, 0.0f};

    /** Whether currently in acceleration phase. */
    bool isAccelerating{false};

    /** Jump hold state tracking */

    /** Time elapsed since jump started (for variable jump). */
    float jumpHoldElapsed{0.0f};

    /** Whether currently holding jump after initial press. */
    bool isJumpHeld{false};

    /** Attack charge state tracking */

    /** Time elapsed charging primary attack. */
    float attack1ChargeElapsed{0.0f};

    /** Time elapsed charging secondary attack. */
    float attack2ChargeElapsed{0.0f};

    /** Whether primary attack is currently being charged. */
    bool isAttack1Charging{false};

    /** Whether secondary attack is currently being charged. */
    bool isAttack2Charging{false};

    /** Camera components */

    /** The spring arm used to position the camera behind the player. */
    godot::SpringArm3D* cameraSpringArm = nullptr;
    /** The camera that follows the player. */
    godot::Camera3D* playerCamera = nullptr;
    /** Default spring arm offset from the player character. */
    godot::Vector3 springArmOffset{0.0f, 2.0f, 0.0f};
    /** Default spring arm length. */
    float springArmLength {7.0f};

    /** Input action names **/

    /** Action name for moving left. */
    godot::String moveLeftAction{"move_left"};

    /** Action name for moving right. */
    godot::String moveRightAction{"move_right"};

    /** Action name for moving forward. */
    godot::String moveForwardAction{"move_forward"};

    /** Action name for moving backward. */
    godot::String moveBackwardAction{"move_backward"};

    /** Action name for jumping. */
    godot::String jumpAction{"move_jump"};

    /** Action name for interacting. */
    godot::String interactAction{"interact"};

    /** Action name for primary attack. */
    godot::String attackAction1 {"attack_1"};

    /** Action name for secondary attack. */
    godot::String attackAction2 {"attack_2"};

    /** Action name for pausing the game. */
    godot::String pauseAction{"pause"};

    /** Action name for opening the menu. */
    godot::String menuAction{"menu"};

    /**
     * @brief Sets up the default camera and spring arm if they do not already exist.
     *
     * This method checks for existing Camera3D and SpringArm3D nodes. If they are not found,
     * it creates them, configures their properties, and adds them as children of the player character.
     */
    void setup_default_camera();

    /**
     * @brief Internal enter tree logic called from _notification.
     *
     * Sets up the default camera. Calls parent's version first.
     */
    void _internal_enter_tree() override;

    /**
     * @brief Internal ready logic called from _notification.
     *
     * Activates the camera. Calls parent's version first.
     */
    void _internal_ready() override;

    /**
     * @brief Internal physics process logic called from _notification.
     *
     * Handles player movement. Calls parent's version first for gravity.
     *
     * @param delta Time elapsed since the last physics frame in seconds.
     */
    void _internal_physics_process(double delta) override;

    /**
     * @brief Internal process logic called from _notification.
     *
     * Updates camera position. Calls parent's version first.
     *
     * @param delta Time elapsed since the last frame in seconds.
     */
    void _internal_process(double delta) override;

protected:
    /** Binds methods and properties to Godot's ClassDB. */
    static void _bind_methods();

public:
    /**
     * @brief Called when the node enters the scene tree.
     *
     * Note: If GDScript overrides this, the override will be called instead.
     * Core logic is in _internal_enter_tree() which is always called via _notification().
     */
    void _enter_tree() override;

    /**
     * @brief Applies movement logic for the player character.
     *
     * This method can be extended to include player-specific movement logic. It is going to be called every frame.
     *
     * @param delta The frame time delta.
     */
    void apply_movement(double delta);

    /**
     * @brief Called when the node is ready.
     *
     * This method ensures that the default camera and spring arm are set up when the node is ready.
     * Note: If GDScript overrides _ready(), this won't be called. Use _notification() for essential setup.
     */
    void _ready() override;

    /**
     * @brief Activates the player camera.
     *
     * Makes the player camera the current active camera.
     */
    void activate_camera() const;

    /**
     * @brief Processes physics updates for the player character.
     *
     * Note: If GDScript overrides this, the override will be called instead.
     * Core logic is in _internal_physics_process() which is always called via _notification().
     *
     * @param p_delta The physics frame time delta.
     */
    void _physics_process(double p_delta) override;

    /**
     * @brief Processes frame updates for the player character.
     *
     * Note: If GDScript overrides this, the override will be called instead.
     * Core logic is in _internal_process() which is always called via _notification().
     *
     * @param p_delta The frame time delta.
     */
    void _process(double p_delta) override;

    [[nodiscard]] godot::Vector3 get_spring_arm_offset() const;

    void set_spring_arm_offset(const godot::Vector3& offset);

    /**
     * @brief Gets the default length of the spring arm.
     * @return The default spring arm length.
     */
    [[nodiscard]] float get_springarm_default_length() const;

    /**
     * @brief Sets the default length of the spring arm.
     * @param length The desired spring arm length.
     */
    void set_springarm_default_length(float length);

    /**
     * @brief Gets the spring arm used for the camera.
     * @return A pointer to the SpringArm3D node.
     */
    [[nodiscard]] godot::SpringArm3D* get_spring_arm() const;

    /**
     * @brief Gets the camera used for the player character.
     * @return A pointer to the Camera3D node.
     */
    [[nodiscard]] godot::Camera3D* get_camera() const;

    /**
     * @brief Gets the input action name for moving left.
     * @return The action name as a String.
     */
    [[nodiscard]] godot::String get_move_left_action() const;

    /**
     * @brief Gets the input action name for moving right.
     * @return The action name as a String.
     */
    [[nodiscard]] godot::String get_move_right_action() const;

    /**
     * @brief Gets the input action name for moving forward.
     * @return The action name as a String.
     */
    [[nodiscard]] godot::String get_move_forward_action() const;

    /**
     * @brief Gets the input action name for moving backward.
     * @return The action name as a String.
     */
    [[nodiscard]] godot::String get_move_backward_action() const;

    /**
     * @brief Gets the input action name for jumping.
     * @return The action name as a String.
     */
    [[nodiscard]] godot::String get_jump_action() const;

    /**
     * @brief Gets the input action name for interacting.
     * @return The action name as a String.
     */
    [[nodiscard]] godot::String get_interact_action() const;

    /**
     * @brief Gets the input action name for pausing the game.
     * @return The action name as a String.
     */
    [[nodiscard]] godot::String get_pause_action() const;

    /**
     * @brief Gets the input action name for opening the menu.
     * @return The action name as a String.
     */
    [[nodiscard]] godot::String get_menu_action() const;

    /**
     * @brief Gets the input action name for the primary attack.
     * @return The action name as a String.
     */
    [[nodiscard]] godot::String get_attack_action1() const;

    /**
     * @brief Gets the input action name for the secondary attack.
     * @return The action name as a String.
     */
    [[nodiscard]] godot::String get_attack_action2() const;

    /**
     * @brief Sets the input action name for the primary attack.
     * @param action_name The action name as a String.
     */
    void set_attack_action1(const godot::String& action_name);

    /**
     * @brief Sets the input action name for the secondary attack.
     * @param action_name The action name as a String.
     */
    void set_attack_action2(const godot::String& action_name);

    /**
     * @brief Sets the input action name for moving left.
     * @param action_name The action name as a String.
     */
    void set_move_left_action(const godot::String& action_name);

    /**
     * @brief Sets the input action name for moving right.
     * @param action_name The action name as a String.
     */
    void set_move_right_action(const godot::String& action_name);

    /**
     * @brief Sets the input action name for moving forward.
     * @param action_name The action name as a String.
     */
    void set_move_forward_action(const godot::String& action_name);

    /**
     * @brief Sets the input action name for moving backward.
     * @param action_name The action name as a String.
     */
    void set_move_backward_action(const godot::String& action_name);

    /**
     * @brief Sets the input action name for jumping.
     * @param action_name The action name as a String.
     */
    void set_jump_action(const godot::String& action_name);

    /**
     * @brief Sets the input action name for interacting.
     * @param action_name The action name as a String.
     */
    void set_interact_action(const godot::String& action_name);

    /**
     * @brief Sets the input action name for pausing the game.
     * @param action_name The action name as a String.
     */
    void set_pause_action(const godot::String& action_name);

    /**
     * @brief Sets the input action name for opening the menu.
     * @param action_name The action name as a String.
     */
    void set_menu_action(const godot::String& action_name);

    /**
     * @brief Gets the current charge level of the primary attack.
     * @return Charge level from 0.0 (no charge) to 1.0 (full charge).
     */
    [[nodiscard]] float get_attack1_charge_level() const;

    /**
     * @brief Gets the current charge level of the secondary attack.
     * @return Charge level from 0.0 (no charge) to 1.0 (full charge).
     */
    [[nodiscard]] float get_attack2_charge_level() const;

    /**
     * @brief Checks if the primary attack is currently being charged.
     * @return True if charging, false otherwise.
     */
    [[nodiscard]] bool is_attack1_charging() const;

    /**
     * @brief Checks if the secondary attack is currently being charged.
     * @return True if charging, false otherwise.
     */
    [[nodiscard]] bool is_attack2_charging() const;

};

} // namespace Rebel
