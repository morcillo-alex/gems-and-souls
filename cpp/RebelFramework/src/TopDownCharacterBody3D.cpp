// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#include "Rebel/CharacterBody/TopDownCharacterBody3D.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

namespace Rebel::CharacterBody {
    void TopDownCharacterBody3D::ApplyGravity(const double delta) {
        if (!get_use_custom_gravity() || get_custom_gravity_magnitude() <= 0.0f) {
            return;
        }
        BaseCharacterBody3D::ApplyGravity(delta);
    }

    void TopDownCharacterBody3D::_bind_methods() {
        // All properties inherited from BaseCharacterBody3D
    }
} // namespace Rebel::CharacterBody
