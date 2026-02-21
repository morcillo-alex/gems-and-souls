// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#pragma once

#include "Rebel/Core.hpp"
#include "Rebel/Ability/AbilityScriptContainerNode.hpp"
#include "Rebel/Ability/AbilityNode.hpp"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/array.hpp>

namespace Rebel::Ability {

/**
 * @brief The complete ability tree for a character or system.
 *
 * Stores a flat list of all AbilityNodes. The tree shape is implied by each
 * node's prerequisites list — AbilityTree itself does not store parent/child
 * relationships explicitly. This keeps the data structure simple and makes it
 * easy to serialise as a single .tres file.
 *
 * Typical usage:
 *   1. Author the tree in the Godot editor (add nodes, assign prerequisites).
 *   2. Attach the tree to a character via a property.
 *   3. Call try_unlock() when the player attempts to unlock an ability.
 *   4. Call get_root_nodes() to know where to start rendering the tree UI.
 *
 * Serializable as a .tres file and fully editable in the Godot Inspector.
 */
class REBEL_FRAMEWORK AbilityTree : public godot::Resource {
    GDCLASS(AbilityTree, godot::Resource);

    /**
     * @brief Flat list of all nodes in this tree.
     *
     * Array of Ref<AbilityNode>. Order does not imply hierarchy — the tree
     * shape comes from each node's prerequisites list.
     */
    godot::Array m_nodes{};

protected:
    static void _bind_methods();

public:
    AbilityTree() = default;

    /**
     * @brief Sets the flat list of all nodes in the tree.
     * @param nodes Array of Ref<AbilityNode>.
     */
    void set_nodes(const godot::Array& nodes);

    /**
     * @brief Returns the flat list of all nodes in the tree.
     * @return Array of Ref<AbilityNode>.
     */
    [[nodiscard]] godot::Array get_nodes() const;

    /**
     * @brief Returns nodes that have no prerequisites (tree entry points).
     *
     * Iterates m_nodes and collects every AbilityNode whose prerequisites
     * array is empty. These are the valid starting points for a UI tree render.
     *
     * @return Array of Ref<AbilityNode> with no prerequisites.
     */
    [[nodiscard]] godot::Array get_root_nodes() const;

    /**
     * @brief Attempts to unlock a node's ability if prerequisites are satisfied.
     *
     * Checks can_unlock() on the provided node. If true, sets the wrapped
     * Ability's enabled flag to true and returns true. Returns false if:
     *   - node is null
     *   - node's wrapped Ability is null
     *   - prerequisites are not all enabled
     *   - the ability is already enabled
     *
     * @param node The AbilityNode to unlock.
     * @return True if the ability was successfully unlocked.
     */
    bool try_unlock(const godot::Ref<AbilityNode>& node);

    /**
     * @brief Unlocks an ability and activates its scene-resident container node.
     *
     * Calls try_unlock() for the prerequisite/data check. Then searches the
     * direct children of @p character for an AbilityScriptContainerNode whose
     * ability property matches the node's ability. If found, calls
     * on_activated() on it, enabling its processing.
     *
     * If no matching container is found the ability is still unlocked in data —
     * this is valid for passive abilities whose effects are applied by reading
     * the Ability resource directly.
     *
     * @param node      The AbilityNode to unlock.
     * @param character The character node whose children are searched for the
     *                  matching AbilityScriptContainerNode.
     * @return The activated AbilityScriptContainerNode, or nullptr if no
     *         matching container was found or if unlock failed.
     */
    godot::Node* try_activate(const godot::Ref<AbilityNode>& node, godot::Node* character);

    /**
     * @brief Calls on_deactivated() on the container node, disabling it.
     *
     * Sets the container's process_mode back to PROCESS_MODE_DISABLED.
     * The node is NOT removed from the scene tree — it remains in place for
     * future reactivation. Safe to call with nullptr — does nothing in that case.
     *
     * @param behavior_node The node returned by a previous try_activate() call.
     */
    void deactivate(godot::Node* behavior_node);
};

} // namespace Rebel::Ability
