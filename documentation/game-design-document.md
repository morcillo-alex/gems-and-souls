# Gems and Souls
## Game Design Document

> **Genre:** Top-Down Rogue-like with RPG Elements

> **Platform:** PC (Godot 4.6 + C++/GDExtension)

> **Target Audience:** Core gamers and casual RPG fans — difficulty scales via optional challenge rooms

> **One-Line Pitch:** Death isn't game over — it's an invitation to a rigged game of hide-and-seek with the Grim Reaper's bored, toy-collecting son.

---

## Table of Contents

- [1. Game Overview](#1-game-overview)
- [2. Core Gameplay Loop](#2-core-gameplay-loop)
- [3. Game Mechanics](#3-game-mechanics)
- [4. Systems Design](#4-systems-design)
  - [4.6 Ability System](#46-ability-system)
- [5. World & Level Design](#5-world--level-design)
- [6. Narrative](#6-narrative)
- [7. Art Direction](#7-art-direction)
- [8. Audio Design](#8-audio-design)
- [9. User Interface](#9-user-interface)
- [10. Technical Specifications](#10-technical-specifications)
- [11. Production](#11-production)
- [Appendix](#appendix)

---

## 1. Game Overview

### 1.1 Concept

**Gems and Souls** is a top-down rogue-like where players explore procedurally-arranged rooms, fight enemies, collect gems, and make meaningful decisions — all under the watchful eye of **Deathy**, the Grim Reaper's young son.

The hook: when the Grim Reaper is too busy to collect your soul, Deathy takes over. Bored and lonely, he has turned the afterlife into his personal toybox, and you are his favorite new action figure. Death isn't a "Game Over" — it's an invitation to a rigged game of hide-and-seek in **Deathy's Playground**.

The twist: instead of a conclusion, death is a **high-stakes transition**. When your health hits zero, you don't restart — you are pulled into Deathy's bedroom for a chaotic micro-game. Win, and you keep your loot and skills. Lose, and the kid keeps your shiny gems for himself.

### 1.2 Core Fantasy

The player is a soul trapped in Deathy's domain, navigating rooms that shift between the living world and the afterlife. Every run feels like a game within a game — you're both the adventurer *and* the toy. The tension between progressing through dangerous rooms and the looming threat of Deathy's micro-games creates a unique risk/reward dynamic.

### 1.3 Unique Selling Points

- **Death as Transition:** Dying triggers a micro-game in Deathy's Playground rather than a restart. Win to keep your loot; lose to forfeit your gems.
- **Dual Difficulty Tracks:** Normal rooms offer progressive challenge; marked doors lead to optional high-risk/high-reward rooms.
- **Rogue-like Choice System:** Choose from 3 options at decision points (rooms, upgrades, paths), blending rogue-like randomness with player agency.
- **Permanent Progression:** In-game experience carries between runs as permanent attribute upgrades, so every loop makes you stronger.
- **Decision Memory:** Player choices are tracked and influence future encounters, narrative, and Deathy's behavior.

### 1.4 References & Inspirations

> [!QUESTION] What are the primary game references and inspirations? (e.g., Hades, The Binding of Isaac, Slay the Spire, WarioWare for the micro-games?)

---

## 2. Core Gameplay Loop

### 2.1 Primary Loop

```mermaid
flowchart TD
    A[Enter Room] --> B[Explore & Fight]
    B --> C[Collect Gems / Loot]
    C --> D{Choose Next Room\n3 options}
    D --> A

    B -- On Death --> E[Deathy's Playground\nMicro-Game]
    E -- Win --> F[Keep Loot]
    E -- Lose --> G[Deathy Keeps Gems]
    F --> A
    G --> H[Restart Run\nKeep Permanent XP]
    H --> A
```

1. **Enter a room** — combat encounters, puzzles, or narrative events.
2. **Clear the room** — defeat enemies, solve challenges, collect gems and items.
3. **Choose your path** — pick from three rogue-like options (next room, upgrade, event).
4. **On death** — transition to Deathy's Playground micro-game.
5. **Micro-game outcome** — win to continue with loot; lose to restart the run but keep permanent XP.

### 2.2 Secondary Loops

- **Permanent Progression Loop:** Gathered in-game experience converts to permanent attribute upgrades between runs. Each run starts slightly stronger.
- **Decision Loop:** Choices made during a run (spare an NPC, take a risk, choose a path) are remembered and affect future runs, room generation, and Deathy's dialogue.
- **Challenge Room Loop:** Marked doors indicate optional high-difficulty rooms with greater loot. Risk/reward decision for experienced players.
- **Gem Economy Loop:** Gems are the primary currency — used for upgrades, but also what Deathy steals on failed micro-games. Managing gem risk is a meta-strategy.

### 2.3 Session Structure

> [!QUESTION] What is the target run length? (e.g., 15-20 minutes per run, 5-10 rooms per floor?)

---

## 3. Game Mechanics

### 3.1 Player Mechanics

The player character is a 3D humanoid navigating a top-down world. Core mechanics:

- **Movement:** WASD-based XZ-plane movement (camera-relative) with acceleration/deceleration curves for responsive but smooth feel. Configurable speed (default 8.0 units/s), acceleration time (0.3s), and deceleration time (0.2s).
- **Camera:** SpringArm3D-based camera system. Default pitch angle: -55° (Hades-like isometric perspective). Configurable from -90° (full top-down) to -15° (closer to third-person). Spring arm length: 12.0 units.
- **Rotation:** Character faces movement direction with smooth interpolation (rotation speed multiplier: 15.0).
- **Gravity:** Optional custom gravity system (disabled by default for pure top-down gameplay). Configurable direction and magnitude when enabled.

> **Technical Note:** Movement is implemented in `PlayerTopDownCharacterBody3D` (Rebel Framework) with ease-in/ease-out acceleration curves. Character hierarchy: `BaseCharacterBody3D` → `TopDownCharacterBody3D` → `PlayerTopDownCharacterBody3D` → `GaS::HeroPlayer`. See `cpp/RebelFramework/include/Rebel/CharacterBody/PlayerTopDownCharacterBody3D.hpp`.

### 3.2 Combat System

Combat uses a dual-attack charge system:

- **Attack 1 (F key):** Primary attack — tap for quick strike, hold to charge for a power attack.
- **Attack 2 (G key):** Secondary attack — different attack type with its own charge mechanic.
- **Charge Mechanic:** Both attacks support hold-to-charge (default charge time: 1.0s). Charge level (0.0–1.0) is tracked while button is held and available via `get_attack1_charge_level()` / `get_attack2_charge_level()` methods.
- **Dodge (Space key):** Dodge mechanic signal emitted when pressed. Implementation deferred to game code.

> **Technical Note:** Attack charging is implemented in `PlayerTopDownCharacterBody3D` (Rebel Framework). Charge state is tracked internally with elapsed time counters (`attack1ChargeElapsed`, `attack2ChargeElapsed`). Boolean flags indicate charging state (`isAttack1Charging`, `isAttack2Charging`). Input actions are configurable properties. See `cpp/RebelFramework/include/Rebel/CharacterBody/PlayerTopDownCharacterBody3D.hpp`.

> [!DECISION] Define attack design. Key questions:
> - What are Attack 1 and Attack 2? (e.g., melee swing vs. ranged projectile? light vs. heavy?)
> - How does charge level (0.0–1.0) scale? Linear damage? Threshold tiers (25%/50%/100%)?
> - Should full charge have a special effect (knockback, AoE, stun)?
> - How does the dodge mechanic work? (Invincibility frames, distance, cooldown?)

### 3.3 Movement System

See [3.1 Player Mechanics](#31-player-mechanics) for core movement. Additional details:

- **Physics Engine:** Jolt Physics 3D for collision detection and resolution.
- **Move-and-Slide:** Standard Godot CharacterBody3D integration for smooth collision handling.
- **Acceleration Curves:** Ease-in for acceleration (slow start, fast finish), ease-out for deceleration (fast start, slow finish). Configurable curve intensity (0=linear, 1=sharp).
- **XZ-Plane Movement:** Movement is constrained to the horizontal plane (XZ). Camera-relative input ensures intuitive WASD control regardless of camera angle.

### 3.4 Animation System

Character animations are driven by an AnimationTree with a state machine:

- **Animation States:**
  - `Idle`: Standing still (no input)
  - `Move`: Walking/running (BlendSpace1D blends based on input magnitude)
  - `JumpAir`: In the air / falling (when `is_on_floor()` is false)
- **State Transitions:** Handled by `HeroPlayer::_on_player_movement_changed()` callback, which receives velocity, input direction, and floor state.
- **Blend Parameters:** Movement blend position set to input direction length (0.0 = idle, 1.0 = full speed).

**Available Character Models:** Kaykit Protagonist B (medium rig) with backpack. Additional models available but unused: Knight, Mage, Ranger, Rogue, Warrior, Caveman, Druid, Engineer, Frostgolem, Vampire, Witch, Tiefling, Clanker, Combat Mech, Black Knight, Barbarians.

> **Technical Note:** Animation integration is implemented in `GaS::HeroPlayer`. AnimationTree is assigned via editor property. State machine playback retrieved with `animationTree->get("parameters/playback")`. See `cpp/src/Game/HeroPlayer.cpp`.

> [!QUESTION] Should alternate character models be unlockable skins as part of permanent progression?

### 3.5 Interaction System

- **Interact (E key):** Context-sensitive interaction with world objects, NPCs, items, altars.

> [!DECISION] Define interaction types and priority:
> 1. **Door entry** — normal and marked doors (core loop)
> 2. **Item/gem pickup** — auto-pickup radius vs. manual interact?
> 3. **Altar activation** — the altar system from the roadmap (see Section 4)
> 4. **NPC dialogue** — if NPCs exist in rooms
> 5. **Chest/container opening** — loot source

---

## 4. Systems Design

### 4.1 Progression System

**Dual Progression Model:**

1. **Per-Run Progression (Temporary):**
   - Rogue-like choices — pick 1 of 3 options at decision points (buffs, items, room paths).
   - Gems collected during the run serve as currency and risk asset.
   - Items and power-ups found in rooms.

2. **Permanent Progression (Across Runs):**
   - In-game experience (XP) gathered during runs converts to permanent attribute upgrades.
   - Attributes persist across all future runs, creating a meta-progression layer.
   - Each game loop starts stronger based on accumulated permanent upgrades.

> [!DECISION] Define the permanent attribute list (e.g., max health, base damage, speed, luck, gem magnet range) and their upgrade costs/scaling.

### 4.2 Economy System

- **Gems:** Primary currency. Collected from enemies, chests, and room rewards. At risk during Deathy's micro-games.
- **Experience (XP):** Earned from combat and exploration. Converts to permanent attribute upgrades. Not lost on death.

> [!DECISION] Define gem drop rates, micro-game gem stakes, and permanent upgrade costs.

### 4.3 Inventory System

> [!QUESTION] Will there be an inventory system? Or are items immediate-use / passive equipped slots?

### 4.4 AI Systems

**Enemy AI:**

> [!DECISION] Define enemy types, behaviors, and AI patterns for each room tier.

**Deathy AI:**

Deathy observes player decisions and adjusts his behavior accordingly. Player choices influence:
- Deathy's dialogue and personality during micro-games.
- The difficulty/type of micro-games triggered on death.
- Room generation and encounter balance in future runs.

### 4.5 Save/Load System

> [!DECISION] Define save structure. Suggested split:
> - **Persistent (saved to disk):** Permanent attributes, total XP earned, decision history, unlocked skins/characters, Deathy relationship state.
> - **Per-run (lost on quit):** Current room, current gems, temporary buffs, run-specific choices.
> - **Format:** JSON or Godot Resource? Consider ease of modding vs. save corruption risk.

### 4.6 Ability System

The Ability System is a **designer-first, data-driven framework** built into RebelFramework. All configuration is done through Godot's inspector using `.tres` Resource files — no code changes required to define new abilities, trees, or upgrade paths.

> **Technical Note:** Implemented as three `Resource`-derived C++ classes registered via GDExtension: `Rebel::Ability::AbilityImprovement`, `Rebel::Ability::Ability`, `Rebel::Ability::AbilityNode`, and `Rebel::Ability::AbilityTree`. Headers under `cpp/RebelFramework/include/Rebel/Ability/`.

---

#### Class Overview

```mermaid
classDiagram
    class AbilityImprovement {
        +int level
        +String description
        +Ref~Texture2D~ icon
        +float cost
    }

    class Ability {
        +String name
        +String description
        +Ref~Texture2D~ icon
        +bool enabled
        +float cost
        +Array improvements
        +int current_level
    }

    class AbilityNode {
        +Ref~Ability~ ability
        +Array prerequisites
        +bool can_unlock()
    }

    class AbilityTree {
        +Array nodes
        +Array get_root_nodes()
        +bool try_unlock(node)
    }

    Ability "1" --> "10" AbilityImprovement : has improvements
    AbilityNode "1" --> "1" Ability : holds
    AbilityNode "0..*" --> "0..*" AbilityNode : prerequisites
    AbilityTree "1" --> "0..*" AbilityNode : contains
```

---

#### `AbilityImprovement` — One Upgrade Level

Represents a single tier of improvement for an ability. Each `Ability` holds exactly **10 slots** (levels 1–10), pre-populated so the inspector always shows all upgrade tiers.

| Property | Type | Description |
|----------|------|-------------|
| `level` | `int` | The tier number (1–10). Informational; actual level is tracked on the parent `Ability`. |
| `description` | `String` | What this level upgrade does. Displayed in the ability UI tooltip. |
| `icon` | `Ref<Texture2D>` | Optional icon override for this level. If `null`, the parent `Ability`'s icon is used instead. Useful for visually representing a powered-up version of the ability. |
| `cost` | `float` | The resource cost to upgrade to this level. The game decides what currency this maps to (gems, XP, soul shards, etc.). |

**Example — "Fireball" Level 3 improvement:**
- `level`: 3
- `description`: "Fireball splits into 3 projectiles on impact."
- `icon`: (null — use the base Fireball icon)
- `cost`: 150.0

---

#### `Ability` — The Core Ability Definition

A single learnable or unlockable skill. Create one `.tres` file per ability.

| Property | Type | Description |
|----------|------|-------------|
| `name` | `String` | Display name shown in the ability tree UI. |
| `description` | `String` | Base description before any improvements are applied. |
| `icon` | `Ref<Texture2D>` | The ability's icon shown in the tree and HUD. Fallback for all `AbilityImprovement` icons that are `null`. |
| `enabled` | `bool` | Whether this ability is currently unlocked and active. Set to `true` by `AbilityTree.try_unlock()`. Read-only in the tree UI — do not manually set this in game code; use `try_unlock()` instead. |
| `cost` | `float` | The resource cost to **unlock** this ability (i.e., to set `enabled = true`). Separate from improvement costs. |
| `improvements` | `Array[AbilityImprovement]` | 10 pre-populated improvement slots (indices 0–9 = levels 1–10). Each slot is editable in the inspector. Slot 0 = Level 1, Slot 9 = Level 10. |
| `current_level` | `int` | The currently active improvement level (0 = no improvements purchased, 1–10 = that level active). |

**Icon resolution rule:**
When rendering improvement level `N`, check `improvements[N-1].icon`. If it is `null`, fall back to `ability.icon`. This lets designers set a single base icon and only override for specific milestone levels (e.g., level 5 and level 10 power thresholds).

**Cost interpretation:**
`Ability.cost` = cost to unlock the ability itself (prerequisites must be met first).
`AbilityImprovement.cost` = cost to purchase each individual upgrade tier.
The game system defines what currency these costs consume — they are intentionally unitless floats.

---

#### `AbilityNode` — One Node in the Prerequisite Tree

Wraps an `Ability` and defines its position in the unlock tree by listing which other nodes must be enabled first.

| Property | Type | Description |
|----------|------|-------------|
| `ability` | `Ref<Ability>` | The ability this node represents. Assign the `.tres` ability resource here. |
| `prerequisites` | `Array[AbilityNode]` | Other `AbilityNode` resources that must have their `ability.enabled == true` before this node can be unlocked. Leave empty for root abilities (no prerequisites). |

**Helper method — `can_unlock() → bool`:**
Returns `true` if **all** prerequisites have their `ability.enabled` set to `true`. This is a synchronous read — call it before displaying the unlock button in the UI.

```
can_unlock() pseudocode:
  for each node in prerequisites:
    if not node.ability.enabled:
      return false
  return true
```

**Prerequisite tree shape:**
The tree is implied by the `prerequisites` arrays — there is no explicit parent pointer. An `AbilityNode` can have **multiple prerequisites** (AND logic: all must be unlocked) and can be a prerequisite for **multiple other nodes** (fan-out). This forms a directed acyclic graph (DAG), not a strict binary tree.

```mermaid
flowchart LR
    A[Dodge Roll\n no prerequisites] --> B[Shadow Step\n requires Dodge Roll]
    A --> C[Evasion Mastery\n requires Dodge Roll]
    B --> D[Phase Shift\n requires Shadow Step\n + Evasion Mastery]
    C --> D
```

---

#### `AbilityTree` — The Full Collection

Groups all `AbilityNode` resources into one serializable asset. One `AbilityTree` `.tres` file per character, class, or skill category.

| Property | Type | Description |
|----------|------|-------------|
| `nodes` | `Array[AbilityNode]` | Flat list of every node in this tree. Order does not imply hierarchy — tree structure is defined by `AbilityNode.prerequisites`. |

**Helper methods:**

`get_root_nodes() → Array`
Returns all nodes that have an empty `prerequisites` array. These are the starting points of the tree — abilities available to unlock from the beginning.

`try_unlock(node: Ref<AbilityNode>) → bool`
Attempts to unlock the ability on the given node:
1. Calls `node.can_unlock()` — if `false`, returns `false` immediately.
2. Checks that the player has sufficient resources to cover `node.ability.cost` (game code must validate this externally or via a signal/callback).
3. Sets `node.ability.enabled = true`.
4. Returns `true` on success.

> [!DECISION] `try_unlock()` currently handles prerequisite checking inside the framework. Resource cost deduction (gems, XP, etc.) should be handled by game code before or after calling `try_unlock()`, since the framework has no knowledge of the game's economy. Define the handshake pattern: does game code check cost → call `try_unlock()`, or does `try_unlock()` emit a signal the game code consumes?

---

#### Designer Workflow

**Step 1 — Create ability resources**

For each ability, create a new `Ability` resource (`.tres`) in the Godot editor:
1. In the FileSystem dock, right-click → **New Resource** → select `Ability`.
2. Fill in `name`, `description`, `icon` (drag a Texture2D from the filesystem).
3. Set `cost` (the unlock cost in whatever currency the game uses).
4. The 10 `improvements` slots are pre-populated. Expand each to fill in `description` and `cost`. Optionally assign a unique `icon` for milestone levels.
5. Leave `enabled = false` and `current_level = 0` — these are managed at runtime by the tree.

**Step 2 — Build the node graph**

For each ability, create an `AbilityNode` resource:
1. Right-click → **New Resource** → select `AbilityNode`.
2. Assign the corresponding `Ability` to the `ability` slot.
3. In `prerequisites`, add references to any `AbilityNode` resources that must be unlocked first. Leave empty for root abilities.

**Step 3 — Assemble the tree**

Create one `AbilityTree` resource per character or skill category:
1. Right-click → **New Resource** → select `AbilityTree`.
2. In `nodes`, add all `AbilityNode` resources for this tree (the full flat list).
3. Save as a `.tres` file and assign it to the relevant game node/component.

**Step 4 — Runtime unlock flow**

```mermaid
flowchart TD
    UI[Player clicks Unlock\non ability node] --> Check{node.can_unlock?}
    Check -- No --> ShowLocked[Highlight missing prerequisites\nin the tree UI]
    Check -- Yes --> Cost{Player has enough\nresources?}
    Cost -- No --> ShowCost[Show insufficient\nresource feedback]
    Cost -- Yes --> Deduct[Deduct cost from\nplayer resources]
    Deduct --> Unlock[tree.try_unlock node\nsets ability.enabled = true]
    Unlock --> Refresh[Refresh tree UI\nshow newly available nodes]
```

**Step 5 — Upgrade improvements**

Once an ability is enabled, its improvement levels are purchased separately:
- Check `ability.current_level` to know the current tier.
- Read `ability.improvements[current_level].cost` for the next upgrade cost.
- After deducting cost in game code, increment `ability.current_level` (max 10).
- The active improvement description is `ability.improvements[ability.current_level - 1].description`.
- The displayed icon is `ability.improvements[ability.current_level - 1].icon ?? ability.icon`.

---

#### Improvement Level Icon Strategy

| Level | Recommended Icon Approach |
|-------|--------------------------|
| 1–4 | Use base `Ability.icon` (no override needed) |
| 5 | Assign a "mid-tier" icon variant (e.g., glowing version) |
| 6–9 | Use `null` (fall back to level 5 override or base) |
| 10 | Assign a "max tier" icon variant (distinct, prestigious look) |

This minimizes the number of unique icons needed while still giving visual weight to milestone levels.

---

## 5. World & Level Design

### 5.1 World Structure

The game world is divided into:

1. **The Living Rooms** — The main dungeon. Procedurally-arranged rooms with enemies, loot, and choices. Two door types:
   - **Normal Doors:** Lead to progressive-difficulty rooms.
   - **Marked Doors:** Lead to optional challenge rooms with greater loot.

2. **Deathy's Playground** — The afterlife micro-game arena. Triggered on death. A chaotic bedroom environment where Deathy sets the rules.

### 5.2 Level Design Principles

- **Rogue-like Room Selection:** At each room transition, the player chooses from 3 options (room types, paths, events).
- **Progressive Difficulty:** Normal path rooms scale in difficulty throughout a run.
- **Risk/Reward Branching:** Marked doors are always visible but optional — catering to both cautious and aggressive playstyles.
- **Decision Points:** Narrative and mechanical choices appear between rooms.

### 5.3 Environment Types

**Currently Available:**
- **Forest:** Kaykit forest environment with 8 color variants. MeshLib created for Color1. Trees, rocks, grass elements.
- **Prototyping:** Kenney prototype textures for blockout and testing.

> [!QUESTION] What other environment types are planned? (e.g., Dungeon, Graveyard, Deathy's Bedroom, Crystal Caves?)

---

## 6. Narrative

### 6.1 Story Overview

The Grim Reaper, overwhelmed with work, leaves soul collection duties to his young son, **Deathy**. Bored and lonely, Deathy doesn't just collect souls — he plays with them. He's turned his corner of the afterlife into a playground where souls must entertain him to earn their freedom (or at least, keep their shiny gems).

The player is the latest soul to catch Deathy's attention. Each run is a "play session" in Deathy's eyes. Win his games, and he might just let you go. Lose, and you're another toy in the box.

### 6.2 Characters

**The Player (Soul):**
- A recently deceased soul. Personality shaped by player decisions.
- Visual: Kaykit Protagonist B (medium rig, backpack-wearing adventurer).

**Deathy:**
- The Grim Reaper's son. Young, mischievous, lonely.
- Not evil — just a kid who doesn't understand the weight of death.
- His mood and behavior shift based on player choices and micro-game outcomes.

**The Grim Reaper (Mentioned/Background):**
- Absent father figure. Too busy to supervise Deathy.
- May appear in late-game narrative beats.

> [!QUESTION] Are there other NPCs the player encounters in rooms? (Merchants, trapped souls, mini-bosses with dialogue?)

### 6.3 Lore & Worldbuilding

> [!QUESTION] How deep should the lore go? Should there be collectible lore items that reveal backstory?

### 6.4 Dialogue System

> [!DECISION] Define dialogue approach. Consider a lightweight system:
> - **In rooms:** Minimal — short NPC text boxes, no branching mid-combat.
> - **Between rooms:** Decision prompts with 2-3 choices that feed into Decision Memory.
> - **Deathy's Playground:** Deathy speaks directly — taunts, rules, reactions to player choices. This is where personality shines.
> - **During runs (optional):** Deathy could commentate via floating text (like a bored kid narrating his toy's actions).

---

## 7. Art Direction

### 7.1 Visual Style

**Current Style:** Stylized 3D with Kaykit character and environment models. Colorful, chunky proportions. Not realistic — toy-like aesthetic that fits the "Deathy's Playground" theme.

### 7.2 Character Design

- **Player Character:** Kaykit Protagonist B — backpack adventurer with medium humanoid proportions.
- **Character Rig:** Medium skeleton rig with individual mesh parts (arms, legs, body, head, backpack).
- **Material:** StandardMaterial3D with albedo texture, 0.5 roughness.

**Available Character Models (Unused):** Knight, Mage, Ranger, Rogue, Warrior, Caveman, Druid, Engineer, Frostgolem, Vampire, Witch, Tiefling, Clanker, Combat Mech, Black Knight, Barbarians.

> [!QUESTION] Will the player unlock alternate character skins/models? Could these be permanent progression rewards?

### 7.3 Environment Art

- **Forest Biome:** Kaykit forest pack — 8 color variants with trees, rocks, grass.
- **Prototyping:** Kenney prototype textures for level blockout.

### 7.4 UI Art Style

> [!DECISION] Define UI visual style. The toy/playground theme suggests: hand-drawn borders, crayon-like fonts, slightly wobbly animations. Should the UI shift style between dungeon (clean/adventurous) and Deathy's Playground (chaotic/childlike)?

### 7.5 VFX

> [!DECISION] Define priority VFX:
> 1. Attack hit/charge effects (particle bursts, screen shake)
> 2. Death transition (screen warp/pull into Deathy's Playground)
> 3. Gem collection (sparkle trail to counter)
> 4. Micro-game visual language (Deathy's rules appearing as scribbled text?)
> 5. Marked door glow/aura (signals risk/reward)

---

## 8. Audio Design

### 8.1 Music Direction

> [!DECISION] Define the audio identity. Suggested direction:
> - **Dungeon rooms:** Tense, adventurous — builds intensity per room.
> - **Deathy's Playground:** Whimsical, toy-box — music-box melodies, xylophone hits.
> - **Transition moments:** Distinct audio stinger when pulled into micro-game.

### 8.2 Sound Effects

> [!DECISION] Prioritize SFX for the core loop first:
> 1. Attack impacts (melee hit, charged hit, miss)
> 2. Footsteps (per surface type)
> 3. Gem pickup / gem loss
> 4. Door open (normal vs. marked)
> 5. Death trigger + micro-game transition
> 6. Micro-game win/lose jingles

### 8.3 Ambient Audio

> [!DECISION] Define ambient layers per environment. Forest biome needs: wind, rustling leaves, distant creatures. Deathy's Playground needs: clock ticking, toy sounds, Deathy humming.

---

## 9. User Interface

### 9.1 HUD

> [!DECISION] Define minimal HUD layout. Suggested elements:
> - Health bar (top-left)
> - Gem counter (top-right)
> - Active buffs/modifiers (below health)
> - Room progress indicator (how deep into the run)
> - Attack charge indicator (near character, only visible when charging)

### 9.2 Menus

- **Pause Menu (ESC):** Pause, settings, quit.
- **Menu/Inventory (TAB):** Character status, collected items, permanent progression view.

> [!DECISION] Define menu navigation flow. Consider: should the permanent progression screen be accessible mid-run or only between runs?

### 9.3 UX Principles

- Minimal HUD clutter — information should be glanceable.
- Clear visual language for door types (normal vs. marked/challenge).
- Smooth transitions between dungeon rooms and Deathy's Playground.
- Rogue-like choice UI should be fast and readable (3 cards/options).

---

## 10. Technical Specifications

### 10.1 Engine & Tools

| Component | Technology |
|-----------|-----------|
| Engine | Godot 4.6 |
| Language | C++ (GDExtension) |
| Build System | CMake (C++20) |
| Physics | Jolt Physics 3D |
| Renderer | Forward Plus (D3D12 on Windows) |
| Dependencies | godot-cpp, vcpkg |

**Input Configuration (project.godot):**

| Action | Key | Purpose |
|--------|-----|---------|
| `move_forward` | W | Move forward (camera-relative) |
| `move_backward` | S | Move backward (camera-relative) |
| `move_left` | A | Move left (camera-relative) |
| `move_right` | D | Move right (camera-relative) |
| `attack_1` | F | Primary attack (charge-capable) |
| `attack_2` | G | Secondary attack (charge-capable) |
| `interact` | E | Context-sensitive interaction |
| `dodge` | Space | Dodge/evade |
| `pause` | Escape | Pause menu |
| `menu` | Tab | Inventory/character menu |

All input actions configured with 0.2 deadzone. Input action names are configurable properties in `PlayerTopDownCharacterBody3D` (can be remapped at runtime if needed).

### 10.2 Architecture Overview

```mermaid
graph TD
    A["Godot 4.6 Engine\n(Forward Plus, Jolt Physics)"]
    B["GDExtension Bridge\n(GemsAndSoulsGame)"]
    C["Game Code (C++)\n- HeroPlayer\n- register_types"]
    D["Rebel Framework (Static)\n- Character Controller Hierarchy\n- Camera Systems\n- Movement Logic"]
    E["godot-cpp Bindings"]

    A --> B --> C --> D --> E
```

**Character Controller Class Hierarchy:**

```mermaid
classDiagram
    CharacterBody3D <|-- BaseCharacterBody3D
    BaseCharacterBody3D <|-- TopDownCharacterBody3D
    BaseCharacterBody3D <|-- PlatformerCharacterBody3D
    TopDownCharacterBody3D <|-- PlayerTopDownCharacterBody3D
    PlatformerCharacterBody3D <|-- PlayerPlatformerCharacterBody3D
    PlayerTopDownCharacterBody3D <|-- HeroPlayer

    class BaseCharacterBody3D {
        <<abstract>>
        +float movingSpeed
        +float accelerationTime
        +float decelerationTime
        +float rotationSpeed
        +float attackChargeTime
        +bool useCustomGravity
        +ApplyGravity(delta)
        +ease_in(t, intensity)
        +ease_out(t, intensity)
    }

    class TopDownCharacterBody3D {
        +ApplyGravity(delta)
    }

    class PlayerTopDownCharacterBody3D {
        +SpringArm3D* cameraSpringArm
        +Camera3D* playerCamera
        +float springArmPitchAngle
        +float attack1ChargeElapsed
        +float attack2ChargeElapsed
        +apply_movement(delta)
        +activate_camera()
        +get_attack1_charge_level()
        +get_attack2_charge_level()
    }

    class HeroPlayer {
        +AnimationTree* animationTree
        +AnimationNodeStateMachinePlayback stateMachine
        +_on_player_movement_changed()
    }
```

**Key Design Decisions:**
- **Rebel Framework** is a reusable static library providing engine-level character and movement mechanics, game-agnostic.
  - `BaseCharacterBody3D`: Abstract base with shared movement, gravity, easing curves, rotation.
  - `TopDownCharacterBody3D`: Optimized for top-down gameplay (gravity disabled by default).
  - `PlayerTopDownCharacterBody3D`: Adds camera system (SpringArm3D + Camera3D), input handling, attack charging.
  - `PlatformerCharacterBody3D` / `PlayerPlatformerCharacterBody3D`: Alternative hierarchy for platformer games (not used in this project).
- **Game Code** extends the framework with game-specific behavior:
  - `GaS::HeroPlayer`: Integrates AnimationTree and state machine, handles animation transitions based on movement state.
- **GDExtension** enables hot-reload during development (`reloadable = true`).

### 10.3 GDExtension Classes

**Registered Classes (Available in Godot Editor):**

All classes are registered in `cpp/src/register_types.cpp` with entry point `gems_and_souls_library_init`. Minimum Godot compatibility: 4.5.

| Class | Namespace | Type | Purpose |
|-------|-----------|------|---------|
| `BaseCharacterBody3D` | `Rebel::CharacterBody` | Abstract | Base character movement and physics |
| `TopDownCharacterBody3D` | `Rebel::CharacterBody` | Concrete | Top-down optimized character body |
| `PlayerTopDownCharacterBody3D` | `Rebel::CharacterBody` | Concrete | Player-controlled top-down with camera |
| `PlatformerCharacterBody3D` | `Rebel::CharacterBody` | Concrete | Platformer character body (unused) |
| `PlayerPlatformerCharacterBody3D` | `Rebel::CharacterBody` | Concrete | Player platformer controller (unused) |
| `HeroPlayer` | `GaS` | Concrete | Game-specific hero with animations |

**Custom Icon:** `PlatformerCharacterBody3D` has custom editor icon defined in `.gdextension` file.

### 10.4 Build System

- **Build Directory:** `cpp/` (all build commands must run from this directory)
- **Build Command:** `cmake --build ./cmake-build-debug --target GemsAndSoulsGame -j 12`
- **Output Location:** `gameLib/` (at project root)
- **Hot Reload:** Enabled (`reloadable = true` in `.gdextension`)
- **CMake Targets:**
  - `GemsAndSoulsGame`: Main GDExtension library (links RebelFramework + godot-cpp)
  - `RebelFramework`: Static library (links godot-cpp)

### 10.5 Performance Targets

- **Target FPS:** 120 (configured in project.godot)
- **Physics Tick:** Default (60 Hz via Jolt)

### 10.6 Platform Requirements

**Primary:** Windows (x86_64)
**Export Targets:** Windows, Linux, Web (configured in `export_presets.cfg`)
**Full Build Support:** macOS (arm64), Android (x86_64, arm64), iOS (xcframework), Web (wasm32)

### 10.7 Code Structure

**Key Files:**

| Path | Purpose |
|------|---------|
| `cpp/CMakeLists.txt` | Main build configuration |
| `cpp/src/register_types.cpp` | GDExtension registration entry point |
| `cpp/src/Game/HeroPlayer.{hpp,cpp}` | Hero character implementation |
| `cpp/RebelFramework/include/Rebel/CharacterBody/` | Framework character controller headers |
| `cpp/RebelFramework/src/` | Framework implementation files |
| `GemsAndSoulsGame.gdextension` | GDExtension configuration (library paths, entry point) |
| `project.godot` | Godot project settings (input, physics, renderer) |
| `game/scenes/characters/hero/hero.tscn` | Hero character scene |
| `game/models/animations/` | Character animations and rigs |

**Development Workflow:**
1. Modify C++ code in `cpp/src/` or `cpp/RebelFramework/`
2. Build from `cpp/` directory: `cmake --build ./cmake-build-debug --target GemsAndSoulsGame -j 12`
3. Godot hot-reloads the extension automatically (no editor restart needed)

### 10.8 Networking

Not applicable — single-player experience.

---

## 11. Production

> **Full Roadmap:** See [`documentation/roadmap.md`](roadmap.md) for the detailed Mermaid gantt chart.

### 11.1 Development Phases

| Phase | Focus | Timeline | Status |
|-------|-------|----------|--------|
| Game Design | Scaffolding, GDD, repository | 2026-02-14 | Done |
| Core Gameplay | Movement, collisions, basic combat | 2026-02-14 to 2026-02-16 | Active |
| Systems | Health, altars, Deathy modifiers, death minigames | 2026-02-17 to 2026-02-21 | Pending |
| Content | Level design (3-4 levels), mechanic introduction | 2026-02-21 to 2026-02-25 | Pending |
| Polish & Export | Menus, music/SFX, bugfix, itch.io export | 2026-02-25 to 2026-02-27 | Pending |

### 11.2 Milestones

- [x] **M1: Project Setup** — Repository, build system, GDExtension working.
- [x] **M2: Core Movement** — Character movement, jumping, camera, collisions.
- [ ] **M3: Basic Combat** — Attack feel, hit detection, damage feedback.
- [ ] **M4: Core Systems** — Health, altars, Deathy modifiers, death micro-games.
- [ ] **M5: Playable Loop** — Full run from start to death to micro-game to restart.
- [ ] **M6: Content Complete** — 3-4 levels, progressive difficulty, challenge rooms.
- [ ] **M7: Polish & Ship** — Menus, audio, bug fixes, itch.io release.

### 11.3 Known Risks

- **Scope Creep:** The micro-game system (Deathy's Playground) could become a second game to build. Keep micro-games simple.
- **Combat Feel:** Getting top-down combat to feel satisfying in 3D with the charge system needs iteration.
- **Decision Tracking:** Remembering player choices across runs requires careful data architecture.

**Target Release:** 2026-02-27 on itch.io (per [roadmap](roadmap.md)).

---

## Appendix

### A. Glossary

| Term | Definition |
|------|-----------|
| **Deathy** | The Grim Reaper's son; the antagonist/game master who controls the afterlife playground. |
| **Deathy's Playground** | The micro-game arena triggered on player death. |
| **Gems** | Primary currency; collected during runs, at risk during death micro-games. |
| **Marked Door** | Optional door leading to a challenge room with greater rewards. |
| **Micro-Game** | Short, chaotic mini-game played in Deathy's Playground on death. |
| **Permanent Attributes** | Player stats that persist across runs, upgraded with XP. |
| **Run** | A single playthrough from start to final death/completion. |
| **Rebel Framework** | The reusable C++ framework providing base character mechanics. |

### B. Change Log

| Date | Change |
|------|--------|
| 2026-02-16 | Initial GDD creation. Populated overview, core loop, mechanics (from codebase), narrative hook, technical specs, and production timeline. Flagged open questions and decisions needed across all sections. |
| 2026-02-16 | Replaced ASCII art diagrams with Mermaid: primary gameplay loop (Section 2.1) and architecture overview (Section 10.2). |
| 2026-02-16 | Added roadmap link to Section 11. Updated development phases with status. Resolved release date question (2026-02-27). Reviewed all callouts: added specific suggestions and structured priorities to DECISION items across combat, interaction, audio, UI, VFX, dialogue, and save/load sections. |
| 2026-02-21 | Added Section 4.6 — Ability System. Full designer documentation for AbilityImprovement, Ability, AbilityNode, and AbilityTree resources: class overview with Mermaid diagram, property tables, icon fallback rules, cost interpretation, prerequisite DAG with example, designer step-by-step workflow, and runtime unlock flow diagram. |
| 2026-02-21 | Updated mechanics sections (3.1-3.5) to reflect actual code implementation: corrected from platformer to top-down movement, added camera system details (SpringArm3D with -55° pitch angle), updated combat system with actual implementation details (charge state tracking), removed jump mechanics, added animation system section (AnimationTree with state machine). Updated architecture overview (10.2) with complete character controller class hierarchy diagram showing BaseCharacterBody3D → TopDownCharacterBody3D → PlayerTopDownCharacterBody3D → HeroPlayer inheritance chain. All technical notes now reference correct class names and file paths. |
