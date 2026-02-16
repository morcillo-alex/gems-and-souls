// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#pragma once

#include "Rebel/CharacterBody/BaseCharacterBody3D.hpp"

namespace Rebel::CharacterBody {

/**
 * @brief A CharacterBody3D for platformer games with jump mechanics and fall-speed clamping.
 *
 * Inherits movement, gravity, easing, and notification routing from BaseCharacterBody3D.
 * Adds jump system (force, hold, release cut) and terminal velocity (maxFallSpeed).
 * Overrides ApplyGravity to clamp downward velocity.
 *
 * Constructor enables custom gravity (true) with magnitude 30.0 to restore
 * the expected platformer defaults.
 */
class REBEL_FRAMEWORK PlatformerCharacterBody3D : public BaseCharacterBody3D {
    GDCLASS(PlatformerCharacterBody3D, BaseCharacterBody3D);

    /** Upward force applied when jumping (initial impulse). */
    float jumpForce{8.0f};

    /** Maximum time jump can be extended by holding the button (seconds). */
    float maxJumpHoldTime{0.50f};

    /** Upward force applied while holding the jump button (units per second squared). */
    float jumpHoldForce{35.0f};

    /** Factor to cut vertical velocity when the jump is released early (0.0-1.0). */
    float jumpReleaseCutFactor{0.25f};

    /** Maximum downward velocity (terminal velocity). */
    float maxFallSpeed{120.0f};

protected:
    /**
     * @brief Applies gravity with fall-speed clamping.
     *
     * Calls the base gravity implementation, then clamps the velocity
     * component along the gravity direction to maxFallSpeed.
     *
     * @param delta Time elapsed since the last frame in seconds.
     */
    void ApplyGravity(double delta) override;

    static void _bind_methods();

public:
    PlatformerCharacterBody3D();

    void set_jump_force(float jump_force);
    [[nodiscard]] float get_jump_force() const;

    void set_max_jump_hold_time(float time);
    [[nodiscard]] float get_max_jump_hold_time() const;

    void set_jump_hold_force(float force);
    [[nodiscard]] float get_jump_hold_force() const;

    void set_jump_release_cut_factor(float factor);
    [[nodiscard]] float get_jump_release_cut_factor() const;

    void set_max_fall_speed(float speed);
    [[nodiscard]] float get_max_fall_speed() const;
};

} // namespace Rebel::CharacterBody
