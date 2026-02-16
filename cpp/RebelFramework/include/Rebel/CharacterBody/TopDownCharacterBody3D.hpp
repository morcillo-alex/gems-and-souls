// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#pragma once

#include "Rebel/CharacterBody/BaseCharacterBody3D.hpp"

namespace Rebel::CharacterBody {

/**
 * @brief A CharacterBody3D for top-down rogue-like games with optional gravity.
 *
 * Inherits movement, gravity, easing, and notification routing from BaseCharacterBody3D.
 * Gravity is off by default (base defaults: useCustomGravity=false, magnitude=0).
 * Overrides ApplyGravity with an early-out when gravity is disabled.
 */
class REBEL_FRAMEWORK TopDownCharacterBody3D : public BaseCharacterBody3D {
    GDCLASS(TopDownCharacterBody3D, BaseCharacterBody3D);

protected:
    /**
     * @brief Applies gravity with early-out when gravity is disabled.
     *
     * Returns immediately if custom gravity is off or magnitude is zero.
     * Otherwise delegates to the base implementation.
     *
     * @param delta Time elapsed since the last frame in seconds.
     */
    void ApplyGravity(double delta) override;

    static void _bind_methods();
};

} // namespace Rebel::CharacterBody
