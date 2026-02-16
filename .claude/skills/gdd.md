---
name: gdd
description: "Create, update, review, or query the Game Design Document (GDD) at docs/GDD.md. Use this skill when the user wants to work on their GDD."
---

# Game Design Document (GDD) Management Skill

You are managing the Game Design Document for this project. The GDD lives at `docs/GDD.md` as a single comprehensive Markdown file.

## Invocation

The user invokes this skill with `/gdd` followed by an optional action and arguments.

## Actions

Parse the user's intent from their message. Supported actions:

### `/gdd` (no arguments)
- If `docs/GDD.md` exists, read it and display a brief status summary: which sections exist, which are stubs/empty, and when it was last modified (via git log).
- If `docs/GDD.md` does not exist, ask the user if they'd like to create the initial GDD template.

### `/gdd init`
- Create the initial GDD template at `docs/GDD.md` with all standard sections.
- Ask the user for the game's name, genre, and a one-line pitch to populate the overview section.
- Create the `docs/` directory if it doesn't exist.

### `/gdd update <section>`
- Read the current GDD and the relevant section.
- Ask the user what changes they want to make, or accept inline descriptions.
- Update the specified section while preserving the rest of the document.
- Sections are matched case-insensitively and support partial matches (e.g., "combat" matches "Combat System").

### `/gdd add <section>`
- Add a new section to the GDD if it doesn't already exist.
- Place it in a logical position within the document structure.

### `/gdd review`
- Read the entire GDD and analyze it for:
  - Missing or stub sections that need content
  - Inconsistencies between sections (e.g., mechanics referenced but not defined)
  - Technical specifications that are outdated relative to the codebase
  - Sections that reference unimplemented features
- Provide a prioritized list of what needs attention.

### `/gdd sync`
- Compare the GDD against the current codebase.
- Identify implemented features not documented in the GDD.
- Identify documented features not yet implemented.
- Suggest updates to bring the GDD in sync with the code.

### `/gdd show <section>`
- Display a specific section of the GDD.

## GDD Template Structure

When creating a new GDD, use this structure:

```markdown
# [Game Name] - Game Design Document

> **Genre:** [Genre]
> **Platform:** PC (Godot 4.6 + C++/GDExtension)
> **Target Audience:** [Audience]
> **One-Line Pitch:** [Pitch]

---

## Table of Contents

## 1. Game Overview
### 1.1 Concept
### 1.2 Core Fantasy
### 1.3 Unique Selling Points
### 1.4 References & Inspirations

## 2. Core Gameplay Loop
### 2.1 Primary Loop
### 2.2 Secondary Loops
### 2.3 Session Structure

## 3. Game Mechanics
### 3.1 Player Mechanics
### 3.2 Combat System
### 3.3 Movement System
### 3.4 Interaction System

## 4. Systems Design
### 4.1 Progression System
### 4.2 Economy System
### 4.3 Inventory System
### 4.4 AI Systems
### 4.5 Save/Load System

## 5. World & Level Design
### 5.1 World Structure
### 5.2 Level Design Principles
### 5.3 Environment Types

## 6. Narrative
### 6.1 Story Overview
### 6.2 Characters
### 6.3 Lore & Worldbuilding
### 6.4 Dialogue System

## 7. Art Direction
### 7.1 Visual Style
### 7.2 Character Design
### 7.3 Environment Art
### 7.4 UI Art Style
### 7.5 VFX

## 8. Audio Design
### 8.1 Music Direction
### 8.2 Sound Effects
### 8.3 Ambient Audio

## 9. User Interface
### 9.1 HUD
### 9.2 Menus
### 9.3 UX Principles

## 10. Technical Specifications
### 10.1 Engine & Tools
### 10.2 Architecture Overview
### 10.3 Performance Targets
### 10.4 Platform Requirements
### 10.5 Networking (if applicable)

## 11. Production
### 11.1 Development Phases
### 11.2 Milestones
### 11.3 Known Risks

## Appendix
### A. Glossary
### B. Change Log
```

## Rules

1. **Never overwrite the entire GDD** without reading it first. Always use Edit to modify specific sections.
2. **Preserve existing content** when updating. Add to or refine sections, don't replace them unless explicitly asked.
3. **Use the Change Log** in the Appendix. Every significant update should add an entry with the date and a brief description of what changed.
4. **Cross-reference** between sections. When a mechanic affects multiple systems, note the connections.
5. **Include technical notes** where relevant — bridge design intent with implementation details (e.g., "This uses the component pattern via GDExtension, see `src/components/`").
6. **Keep language clear and accessible** — the team has varying experience levels.
7. **Flag open questions** with a `> [!QUESTION]` callout so they're visible and trackable.
8. **Flag decisions needed** with a `> [!DECISION]` callout.
