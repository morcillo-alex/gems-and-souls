#pragma once

#include "Rebel/Core.hpp"
#include "godot_cpp/classes/character_body3d.hpp"

namespace Rebel::CharacterBody {

/**
 * @brief A custom CharacterBody3D for platformer games with configurable gravity.
 *
 * This class extends Godot's CharacterBody3D to allow for custom gravity settings,
 * enabling platformer-style movement mechanics. You can specify whether to use
 * custom gravity, the direction of gravity, and its magnitude.
 */
class REBEL_FRAMEWORK PlatformerCharacterBody3D : public godot::CharacterBody3D {
    GDCLASS(PlatformerCharacterBody3D, godot::CharacterBody3D);

    /** Movement Properties **/

    /** Whether to use custom gravity instead of the global gravity setting. */
    bool useCustomGravity{true};

    /** Direction of custom gravity (normalized vector). */
    godot::Vector3 customGravityDirection{0.0f, -1.0f, 0.0f};

    /** Magnitude of custom gravity (units per second squared). */
    float customGravityMagnitude{30.f};

    /** Upward force applied when jumping (initial impulse). */
    float jumpForce{8.0f};

    /** Maximum time jump can be extended by holding the button (seconds). */
    float maxJumpHoldTime{0.50f};

    /** Upward force applied while holding the jump button (units per second squared). */
    float jumpHoldForce{35.0f};

    /** Factor to cut vertical velocity when the jump is released early (0.0-1.0). */
    float jumpReleaseCutFactor{0.25f};

    /** Time to reach full attack charge (seconds). */
    float attackChargeTime{1.0f};

    /** Maximum downward velocity (terminal velocity). */
    float maxFallSpeed{120.0f};

    /** Movement speed in units per second. */
    float movingSpeed{8.0f};

    /** Time to reach max speed in seconds. */
    float accelerationTime{0.3f};

    /** Time to stop in seconds. */
    float decelerationTime{0.2f};

    /** Curve intensity for acceleration (0=linear, 1=sharp ease-in). */
    float accelerationCurveIntensity{0.5f};

    /** Curve intensity for deceleration (0=linear, 1=sharp ease-out). */
    float decelerationCurveIntensity{0.5f};

    /** Rotation speed multiplier for turning the character. */
    float rotationSpeed{15.0f};

    /** Default gravity settings for reference **/
    godot::Vector3 defaultGravityVec{0.0f, -1.f, 0.0f};

    /** Default gravity strength for reference **/
    float defaultGravityStrength {9.8f};

protected:
    /**
     * @brief Applies gravity to the character's velocity.
     * @param delta Time elapsed since the last frame in seconds.
     */
    void ApplyGravity(double delta);

    /**
     * @brief Ease-in function for acceleration curves.
     *
     * Slow start, fast finish. Higher intensity = sharper curve.
     *
     * @param t Normalized time (0 to 1).
     * @param intensity Curve intensity (0=linear, 1=sharp).
     * @return Eased value between 0 and 1.
     */
    static float ease_in(float t, float intensity);

    /**
     * @brief Ease-out function for deceleration curves.
     *
     * Fast start, slow finish. Higher intensity = sharper curve.
     *
     * @param t Normalized time (0 to 1).
     * @param intensity Curve intensity (0=linear, 1=sharp).
     * @return Eased value between 0 and 1.
     */
    static float ease_out(float t, float intensity);

    /**
     * @brief Internal enter tree logic called from _notification.
     *
     * Override in derived classes to add enter tree logic.
     * Always call the parent's version first.
     */
    virtual void _internal_enter_tree();

    /**
     * @brief Internal ready logic called from _notification.
     *
     * Override in derived classes to add ready logic.
     * Always call the parent's version first.
     */
    virtual void _internal_ready();

    /**
     * @brief Internal physics process logic called from _notification.
     *
     * Override in derived classes to add physics processing.
     * Always call the parent's version first.
     *
     * @param delta Time elapsed since the last physics frame in seconds.
     */
    virtual void _internal_physics_process(double delta);

    /**
     * @brief Internal process logic called from _notification.
     *
     * Override in derived classes to add frame processing.
     * Always call the parent's version first.
     *
     * @param delta Time elapsed since the last frame in seconds.
     */
    virtual void _internal_process(double delta);

    static void _bind_methods();

public:
    /**
     * @brief Sets whether to use custom gravity for this character.
     * @param enabled If true, the character uses custom gravity instead of global gravity.
     */
    void set_use_custom_gravity(bool enabled);

    /**
     * @brief Returns whether custom gravity is enabled.
     * @return True if custom gravity is enabled, false otherwise.
     */
    [[nodiscard]] bool get_use_custom_gravity() const;

    /**
     * @brief Sets the jump force for the character.
     * @param jump_force The upward force applied when jumping.
     */
    void set_jump_force(float jump_force);

    /**
     * @brief Returns the jump force of the character.
     * @return The upward force applied when jumping.
     */
    [[nodiscard]] float get_jump_force() const;

    /**
     * @brief Sets the maximum jump hold time.
     * @param time Maximum time in seconds that holding jump extends the jump.
     */
    void set_max_jump_hold_time(float time);

    /**
     * @brief Returns the maximum jump hold time.
     * @return Maximum time in seconds that holding jump extends the jump.
     */
    [[nodiscard]] float get_max_jump_hold_time() const;

    /**
     * @brief Sets the jump hold force.
     * @param force Upward force applied while holding jump (units per second squared).
     */
    void set_jump_hold_force(float force);

    /**
     * @brief Returns the jump hold force.
     * @return Upward force applied while holding jump.
     */
    [[nodiscard]] float get_jump_hold_force() const;

    /**
     * @brief Sets the jump release cut factor.
     * @param factor Factor to multiply velocity when releasing early (0.0-1.0). Lower = shorter hop.
     */
    void set_jump_release_cut_factor(float factor);

    /**
     * @brief Returns the jump release cut factor.
     * @return Factor applied to velocity on early release.
     */
    [[nodiscard]] float get_jump_release_cut_factor() const;

    /**
     * @brief Sets the attack charge time.
     * @param time Time in seconds to reach full charge.
     */
    void set_attack_charge_time(float time);

    /**
     * @brief Returns the attack charge time.
     * @return Time in seconds to reach full charge.
     */
    [[nodiscard]] float get_attack_charge_time() const;

    /**
     * @brief Sets the direction of custom gravity.
     * @param direction A normalized vector indicating gravity direction (e.g., (0, -1, 0) for downward).
     */
    void set_custom_gravity_direction(const godot::Vector3& direction);

    /**
     * @brief Returns the custom gravity direction.
     * @return The normalized gravity direction vector.
     */
    [[nodiscard]] godot::Vector3 get_custom_gravity_direction() const;

    /**
     * @brief Sets the magnitude of custom gravity.
     * @param magnitude The gravitational force in units per second squared.
     */
    void set_custom_gravity_magnitude(float magnitude);

    /**
     * @brief Returns the custom gravity magnitude.
     * @return The gravitational force magnitude (default: 9.8).
     */
    [[nodiscard]] float get_custom_gravity_magnitude() const;

    /**
     * @brief Sets the maximum fall speed (terminal velocity).
     * @param speed The maximum downward velocity in units per second.
     */
    void set_max_fall_speed(float speed);

    /**
     * @brief Returns the maximum fall speed.
     * @return The terminal velocity (default: 20.0).
     */
    [[nodiscard]] float get_max_fall_speed() const;

    /**
     * @brief Gets the movement speed of the character.
     * @return The movement speed in units per second.
     */
    [[nodiscard]] float get_moving_speed() const;

    /**
     * @brief Sets the movement speed of the character.
     * @param speed The movement speed in units per second.
     */
    void set_moving_speed(float speed);

    /**
     * @brief Gets the acceleration time.
     * @return Time to reach max speed in seconds.
     */
    [[nodiscard]] float get_acceleration_time() const;

    /**
     * @brief Sets the acceleration time.
     * @param value Time to reach max speed in seconds.
     */
    void set_acceleration_time(float value);

    /**
     * @brief Gets the deceleration time.
     * @return Time to stop in seconds.
     */
    [[nodiscard]] float get_deceleration_time() const;

    /**
     * @brief Sets the deceleration time.
     * @param value Time to stop in seconds.
     */
    void set_deceleration_time(float value);

    /**
     * @brief Gets the acceleration curve intensity.
     * @return Curve intensity (0=linear, 1=sharp ease-in).
     */
    [[nodiscard]] float get_acceleration_curve_intensity() const;

    /**
     * @brief Sets the acceleration curve intensity.
     * @param value Curve intensity (0=linear, 1=sharp ease-in).
     */
    void set_acceleration_curve_intensity(float value);

    /**
     * @brief Gets the deceleration curve intensity.
     * @return Curve intensity (0=linear, 1=sharp ease-out).
     */
    [[nodiscard]] float get_deceleration_curve_intensity() const;

    /**
     * @brief Sets the deceleration curve intensity.
     * @param value Curve intensity (0=linear, 1=sharp ease-out).
     */
    void set_deceleration_curve_intensity(float value);

    /**
     * @brief Gets the rotation speed for turning the character.
     * @return The rotation speed multiplier.
     */
    [[nodiscard]] virtual float get_rotation_speed() const;

    /**
     * @brief Sets the rotation speed for turning the character.
     * @param value rotation speed multiplier.
     */
    void set_rotation_speed(float value);

    /**
     * @brief Gets the up direction based on the current gravity settings.
     *
     * Returns the opposite of the gravity direction (normalized).
     * When custom gravity is enabled, returns `-customGravityDirection.normalized()`.
     * Otherwise, returns `-defaultGravityVec.normalized()`.
     *
     * @return The up direction vector (opposite of gravity).
     */
    [[nodiscard]] godot::Vector3 get_gravity_up_direction() const;

    /**
     * @brief Handles Godot notifications.
     *
     * Godot always calls this method, even when GDScript overrides lifecycle methods.
     * Routes to internal methods for actual logic.
     *
     * @param p_what The notification type.
     */
    virtual void _notification(int p_what);

    /**
     * @brief Called every physics frame to update the character's movement.
     *
     * Note: If GDScript overrides this, the override will be called instead.
     * Core logic is in _internal_physics_process() which is always called via _notification().
     *
     * @param p_delta Time elapsed since the last physics frame in seconds.
     */
    void _physics_process(double p_delta) override;

    /**
     * @brief Called when the node enters the scene tree.
     *
     * Note: If GDScript overrides this, the override will be called instead.
     * Core logic is in _internal_ready() which is always called via _notification().
     */
    void _ready() override;
};

} // namespace Rebel