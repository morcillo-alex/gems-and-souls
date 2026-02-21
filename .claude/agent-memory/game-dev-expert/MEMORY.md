# ARVIS Agent Memory — Gems and Souls

## Project Stack
- Godot 4.6, C++20, CMake, Jolt Physics, Forward+ / D3D12 (Windows)
- Two-layer GDExtension: RebelFramework (static lib) + GemsAndSoulsGame (shared lib)
- Build: `cmake.exe --build ./cmake-build-debug --target GemsAndSoulsGame -j 12` from `cpp/`

## Key Paths
- Framework headers: `cpp/RebelFramework/include/Rebel/`
- Framework source: `cpp/RebelFramework/src/`
- Game source: `cpp/src/`
- Registration entry: `cpp/src/register_types.cpp`
- CMake source list: `cpp/RebelFramework/CMakeLists.txt` (explicit file list, NOT globbed)
- GDD: `documentation/game-design-document.md`

## Established Conventions
- Namespaces: `Rebel::CharacterBody`, `Rebel::Ability`, `GaS`
- Member prefix: `m_` (e.g., `m_nodes`, `m_behavior_scene`)
- Export macro: `REBEL_FRAMEWORK` from `Rebel/Core.hpp`
- Public methods: PascalCase; private methods: snake_case
- Headers use `#pragma once`; godot-cpp includes before stdlib
- Abstract base classes use `GDREGISTER_ABSTRACT_CLASS`; concrete use `GDREGISTER_CLASS`
- Registration order: leaf/base types before container types

## Ability System (as of 2026-02-21)
Classes: AbilityImprovement -> Ability -> AbilityNode -> AbilityTree + AbilityScriptContainerNode
- `Ability` resource: name, description, icon, enabled, cost, improvements[10], current_level (NO behavior_scene)
- `AbilityTree`: flat node list; try_unlock(), try_activate(node, character), deactivate(behavior_node)
- `AbilityScriptContainerNode`: Node-derived; lives permanently in character scene; disabled by default
  (PROCESS_MODE_DISABLED); on_activated() enables it, on_deactivated() disables it (never queue_free'd)
- try_activate searches character's direct children for container where get_ability() == node ability
- deactivate calls on_deactivated() only — does NOT queue_free
- CMakeLists.txt must be updated manually when new .cpp files are added

## CMakeLists Pattern
Files are listed explicitly in `cpp/RebelFramework/CMakeLists.txt` under `add_library()`.
Always add both header and .cpp when creating new framework classes.
