// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#pragma once

#include "Rebel/Core.hpp"
#include "Rebel/Ability/Ability.hpp"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

namespace Rebel::Ability {

/**
 * @brief Scene-resident node that encapsulates an ability's runtime behaviour.
 *
 * Place one or more AbilityScriptContainerNode children directly inside the
 * character scene. Assign the matching Ability resource to the `ability`
 * property in the Inspector. The node is disabled by default and activated /
 * deactivated exclusively by AbilityTree.
 *
 * When activated, the node's process_mode is set to PROCESS_MODE_INHERIT so
 * _process() and _physics_process() run normally. When deactivated, it is
 * set back to PROCESS_MODE_DISABLED — the node is never removed from the tree.
 *
 * Extend this class in GDScript to define ability behaviour:
 *
 * @code
 * extends AbilityScriptContainerNode
 *
 * func _on_activated() -> void:
 *     print("Ability started!")
 *
 * func _process(delta: float) -> void:
 *     # Runs every frame while the ability is active.
 *     pass
 *
 * func _on_deactivated() -> void:
 *     print("Ability ended!")
 * @endcode
 *
 * @note The character node is always accessible via get_parent() since this
 *       node is a direct child of the character scene root.
 */
class REBEL_FRAMEWORK AbilityScriptContainerNode : public godot::Node {
    GDCLASS(AbilityScriptContainerNode, godot::Node);

    /**
     * @brief The Ability resource this container corresponds to.
     *
     * Set in the Godot Inspector. AbilityTree matches containers to AbilityNodes
     * by comparing this reference against AbilityNode::get_ability(). Must be
     * unique per character — two containers on the same character should not
     * reference the same Ability resource.
     */
    godot::Ref<Ability> m_ability{};

protected:
    static void _bind_methods();

public:
    /**
     * @brief Constructor — sets process_mode to PROCESS_MODE_DISABLED.
     *
     * The node remains dormant until AbilityTree::try_activate() enables it.
     */
    AbilityScriptContainerNode();

    /**
     * @brief Sets the Ability resource this container serves.
     * @param ability The ability resource assigned in the Inspector.
     */
    void set_ability(const godot::Ref<Ability>& ability);

    /**
     * @brief Returns the Ability resource this container serves.
     * @return The assigned Ability resource, may be null if not yet configured.
     */
    [[nodiscard]] godot::Ref<Ability> get_ability() const;

    /**
     * @brief Enables the node and notifies it that its ability was activated.
     *
     * Sets process_mode to PROCESS_MODE_INHERIT so _process() starts running,
     * then dispatches the GDScript virtual _on_activated().
     *
     * Called automatically by AbilityTree::try_activate(). Do not call directly.
     */
    virtual void on_activated();

    /**
     * @brief Disables the node and notifies it that its ability was deactivated.
     *
     * Dispatches the GDScript virtual _on_deactivated(), then sets process_mode
     * back to PROCESS_MODE_DISABLED. The node remains in the scene tree.
     *
     * Called automatically by AbilityTree::deactivate(). Do not call directly.
     */
    virtual void on_deactivated();

    GDVIRTUAL0(_on_activated)
    GDVIRTUAL0(_on_deactivated)
};

} // namespace Rebel::Ability
