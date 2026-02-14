cmake_minimum_required(VERSION 3.20)
project(GemsAndSoulsGame LANGUAGES CXX VERSION 0.1.0 DESCRIPTION TBD)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../gameLib/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../gameLib/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../gameLib/")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendors/godot-cpp)
get_target_property(NAME_SUFFIX godot-cpp GODOTCPP_SUFFIX)

# Include GodotCPPModule for documentation support
list(APPEND CMAKE_MODULE_PATH "${godot-cpp_SOURCE_DIR}/cmake")
include(GodotCPPModule)

add_library(${PROJECT_NAME} SHARED
        src/register_types.cpp
        src/register_types.hpp
        src/Game/HeroPlayer.cpp
        src/Game/HeroPlayer.hpp
)

add_subdirectory(RebelFramework)
target_link_libraries(${PROJECT_NAME} PRIVATE RebelFramework)



target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Debug>:DEBUG>")
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_NAME=\"${PROJECT_NAME}\")
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_VERSION=\"${PROJECT_VERSION}\")
string(TOLOWER "${PROJECT_NAME}" PROJECT_NAME_LOWER)
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_IDENTIFIER="com.rebel.games.${PROJECT_NAME_LOWER}")

target_link_libraries(${PROJECT_NAME} PRIVATE godot-cpp)


set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME}${NAME_SUFFIX}"
        PREFIX ""  # Remove 'lib' prefix on Unix to match gdextension paths
)

