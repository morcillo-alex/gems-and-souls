# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**Gems and Souls** is a top-down rogue-like game built with **Godot 4.6** using **C++ GDExtensions** for core gameplay logic. The project combines Godot's scene system and editor with the performance of native C++ code.

- **Engine:** Godot 4.6
- **Language:** C++20
- **Build System:** CMake
- **Physics Engine:** Jolt Physics
- **Rendering:** Forward+ (D3D12 on Windows)
- **Target Platform:** PC (Windows primary)

## Architecture

### High-Level Structure

```
gems-and-souls/
├── cpp/                          # C++ GDExtension source
│   ├── src/                      # Game-specific C++ code
│   │   ├── Game/                 # Game logic (HeroPlayer, etc.)
│   │   └── register_types.cpp    # GDExtension registration
│   ├── RebelFramework/           # Custom C++ framework library
│   │   ├── include/Rebel/        # Framework headers
│   │   │   └── CharacterBody/    # Character controller components
│   │   └── src/                  # Framework implementation
│   ├── vendors/                  # Third-party dependencies
│   │   ├── godot-cpp/            # Godot C++ bindings (submodule)
│   │   └── vcpkg/                # Package manager
│   └── CMakeLists.txt            # Main build configuration
├── game/                         # Godot assets
│   ├── scenes/                   # Scene files (.tscn)
│   ├── models/                   # 3D models and animations
│   ├── materials/                # Materials
│   └── meshlibs/                 # Mesh libraries
├── gameLib/                      # Build output (compiled .dll/.so files)
├── documentation/                # Project documentation
│   ├── game-design-document.md   # Complete GDD
│   └── roadmap.md                # Development roadmap
├── project.godot                 # Godot project configuration
└── GemsAndSoulsGame.gdextension  # GDExtension configuration
```

### GDExtension Architecture

The project uses a **two-layer architecture**:

1. **RebelFramework** (Static Library)
   - Reusable C++ framework for Godot game development
   - Base classes for character controllers:
     - `BaseCharacterBody3D` (abstract base)
     - `PlatformerCharacterBody3D` / `PlayerPlatformerCharacterBody3D`
     - `TopDownCharacterBody3D` / `PlayerTopDownCharacterBody3D`
   - Links to godot-cpp

2. **GemsAndSoulsGame** (Shared Library / GDExtension)
   - Game-specific logic (e.g., `GaS::HeroPlayer`)
   - Links to RebelFramework and godot-cpp
   - Registered classes are exposed to Godot editor

### Registered Classes

All registered classes (available in Godot):
- `Rebel::CharacterBody::BaseCharacterBody3D` (abstract)
- `Rebel::CharacterBody::PlatformerCharacterBody3D`
- `Rebel::CharacterBody::PlayerPlatformerCharacterBody3D`
- `Rebel::CharacterBody::TopDownCharacterBody3D`
- `Rebel::CharacterBody::PlayerTopDownCharacterBody3D`
- `GaS::HeroPlayer`

## Build Commands

### Prerequisites
- CMake 3.20+
- C++20-compatible compiler (MSVC, GCC, or Clang)
- Godot 4.6 installed

### Building the Project

**IMPORTANT:** Always run build commands from the `cpp/` directory.

```bash
# Navigate to cpp directory
cd cpp

# Build the project (Debug configuration)
cmake.exe --build ./cmake-build-debug --target GemsAndSoulsGame -j 12
```

Build outputs are placed in `gameLib/` at the project root, where Godot expects them based on the `.gdextension` configuration.

### Build Targets

- `GemsAndSoulsGame` — Main GDExtension library (game + framework)
- `RebelFramework` — Framework library (built automatically as dependency)

### CMake Configuration

```bash
# Configure with CMake (if needed)
cmake -B cmake-build-debug -S . -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build cmake-build-debug --target GemsAndSoulsGame -j 12
```

## Development Workflow

1. **Modify C++ code** in `cpp/src/` or `cpp/RebelFramework/`
2. **Build the extension** from `cpp/` directory
3. **Open Godot editor** and test changes
   - Godot reloads the extension automatically when `reloadable = true` is set
4. **Iterate** — changes require rebuild but not editor restart

### Hot Reloading

The `.gdextension` file has `reloadable = true`, allowing Godot to reload the extension without restarting the editor (Godot 4.6 feature).

## Input Actions

Defined in `project.godot`:
- **Movement:** WASD (`move_forward`, `move_backward`, `move_left`, `move_right`)
- **Combat:** F (`attack_1`), G (`attack_2`)
- **Interact:** E
- **Dodge:** Space
- **Pause:** Escape
- **Menu:** Tab

## Documentation

- **Game Design Document:** `documentation/game-design-document.md`
  - Comprehensive design specification for the game
  - Includes gameplay loop, mechanics, systems, narrative, and technical specs
  - Use the `/gdd` skill for structured GDD operations

- **Roadmap:** `documentation/roadmap.md`
  - Development phases and milestones
  - Current status and upcoming features

## Custom Skills

- **`/gdd`** — Create, update, review, or query the Game Design Document

## Code Conventions

### C++ Style
- **Classes:** PascalCase (matching Godot convention)
- **Public methods:** PascalCase
- **Private methods:** snake_case
- **Member variables:** Prefix with `m_` or use consistent project convention
- **Constants/Macros:** UPPER_SNAKE_CASE
- **Namespaces:** `Rebel::CharacterBody`, `GaS` (game-specific)

### GDExtension Patterns
- Use `GDCLASS` macro for all exported classes
- Implement `_bind_methods()` for property/method registration
- Use `GDREGISTER_CLASS()` in `register_types.cpp`
- Prefer `Ref<T>` for RefCounted-derived types
- Use raw pointers for Object-derived scene nodes
- Handle null checks and scene tree readiness

### File Organization
- Header guards or `#pragma once`
- Include godot-cpp headers before standard library
- Separate interface (.hpp) from implementation (.cpp)
- Register new classes in `src/register_types.cpp`

## Important Technical Details

### GDExtension Entry Point
- Entry symbol: `gems_and_souls_library_init` (defined in `register_types.cpp`)
- Initialization level: `MODULE_INITIALIZATION_LEVEL_SCENE`
- Minimum Godot compatibility: 4.5

### CMake Output Configuration
Build artifacts are placed in `gameLib/` via:
```cmake
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../gameLib/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../gameLib/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../gameLib/")
```

### Platform-Specific Libraries
The `.gdextension` file defines library paths for all platforms (Windows, macOS, Linux, Android, iOS, Web). Primary development is on Windows using D3D12 renderer.

## ARVIS Agent

For game development tasks, code reviews, GDD updates, and technical guidance, use the `game-dev-expert` agent (referred to as ARVIS). This agent specializes in:
- Godot 4.6 GDExtension C++ development
- Game design documentation
- Technical code reviews
- Performance and portability improvements
- Feature implementation

Launch the agent when you need expert guidance on game-specific C++ code, architecture decisions, or GDD management.
