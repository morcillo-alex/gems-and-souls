// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#pragma once

#include "Rebel/Core.hpp"
#include "Rebel/Ability/AbilityNode.hpp"
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
};

} // namespace Rebel::Ability
