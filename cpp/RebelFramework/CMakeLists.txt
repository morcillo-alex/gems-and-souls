cmake_minimum_required(VERSION 3.20)
project(RebelFramework LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

message(STATUS "Building the Rebel Framework by Alejandro Morcillo Montejo - All Rights Reserved")
add_library(${PROJECT_NAME} STATIC
        include/Rebel/Core.hpp
        src/Framework.cpp
        src/BaseCharacterBody3D.cpp
        include/Rebel/CharacterBody/BaseCharacterBody3D.hpp
        src/PlatformerCharacterBody3D.cpp
        include/Rebel/CharacterBody/PlatformerCharacterBody3D.hpp
        src/PlayerPlatformerCharacterBody3D.cpp
        include/Rebel/CharacterBody/PlayerPlatformerCharacterBody3D.hpp
        src/TopDownCharacterBody3D.cpp
        include/Rebel/CharacterBody/TopDownCharacterBody3D.hpp
        src/PlayerTopDownCharacterBody3D.cpp
        include/Rebel/CharacterBody/PlayerTopDownCharacterBody3D.hpp

        # Ability System
        include/Rebel/Ability/AbilityScriptContainerNode.hpp
        src/Ability/AbilityScriptContainerNode.cpp
        include/Rebel/Ability/AbilityImprovement.hpp
        src/Ability/AbilityImprovement.cpp
        include/Rebel/Ability/Ability.hpp
        src/Ability/Ability.cpp
        include/Rebel/Ability/AbilityNode.hpp
        src/Ability/AbilityNode.cpp
        include/Rebel/Ability/AbilityTree.hpp
        src/Ability/AbilityTree.cpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC godot-cpp)


target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_NAME=\"${PROJECT_NAME}\")
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_VERSION=\"${PROJECT_VERSION}\")
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_DESCRIPTION=\"${PROJECT_DESCRIPTION}\")

# Define export macro
target_compile_definitions(${PROJECT_NAME} PRIVATE REBEL_FRAMEWORK_EXPORTS)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME}${NAME_SUFFIX}"
        PREFIX ""  # Remove 'lib' prefix on Unix to match gdextension paths
)