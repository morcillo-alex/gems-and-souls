// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#pragma once

#include "Rebel/Core.hpp"
#include "Rebel/Ability/Ability.hpp"
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/array.hpp>

namespace Rebel::Ability {

/**
 * @brief One node in the ability tree, wrapping an Ability with prerequisite links.
 *
 * The ability tree's shape is encoded entirely through prerequisites — each
 * AbilityNode holds a list of other AbilityNodes that must be enabled before
 * this one can be unlocked. This avoids a rigid parent/child hierarchy and
 * allows diamond-shaped dependencies (an ability that requires two others).
 *
 * Serializable as a .tres file and fully editable in the Godot Inspector.
 *
 * @note The can_unlock() helper is intentionally exposed to GDScript so that
 *       UI code can query readiness without reimplementing the prerequisite logic.
 */
class REBEL_FRAMEWORK AbilityNode : public godot::Resource {
    GDCLASS(AbilityNode, godot::Resource);

    /** The ability data wrapped by this node. */
    godot::Ref<Ability> m_ability{};

    /**
     * @brief Nodes whose wrapped abilities must be enabled before this one.
     *
     * Array of Ref<AbilityNode>. All elements must have their ability enabled
     * for can_unlock() to return true.
     */
    godot::Array m_prerequisites{};

protected:
    static void _bind_methods();

public:
    AbilityNode() = default;

    /**
     * @brief Sets the ability wrapped by this node.
     * @param ability The Ability resource for this node.
     */
    void set_ability(const godot::Ref<Ability>& ability);

    /**
     * @brief Returns the ability wrapped by this node.
     * @return Ref<Ability>, may be null if not yet assigned.
     */
    [[nodiscard]] godot::Ref<Ability> get_ability() const;

    /**
     * @brief Sets the prerequisite node list.
     *
     * Each element should be a Ref<AbilityNode>. Elements that are not
     * AbilityNode resources are skipped during can_unlock() evaluation.
     *
     * @param prerequisites Array of Ref<AbilityNode>.
     */
    void set_prerequisites(const godot::Array& prerequisites);

    /**
     * @brief Returns the prerequisite node list.
     * @return Array of Ref<AbilityNode>.
     */
    [[nodiscard]] godot::Array get_prerequisites() const;

    /**
     * @brief Checks whether all prerequisite abilities are enabled.
     *
     * Returns true when every element in prerequisites is a valid AbilityNode
     * whose wrapped Ability has enabled == true.
     *
     * Returns true immediately when the prerequisites list is empty (root node).
     *
     * @return True if this node can be unlocked right now.
     */
    [[nodiscard]] bool can_unlock() const;
};

} // namespace Rebel::Ability
