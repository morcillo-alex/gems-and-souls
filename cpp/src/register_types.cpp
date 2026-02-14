

// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

#include "register_types.hpp"

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "Game/HeroPlayer.hpp"
#include "Rebel/CharacterBody/PlatformerCharacterBody3D.hpp"
#include "Rebel/CharacterBody/PlayerPlatformerCharacterBody3D.hpp"



using namespace godot;

void initialize_gems_and_souls_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_CLASS(Rebel::CharacterBody::PlatformerCharacterBody3D);
	GDREGISTER_CLASS(Rebel::CharacterBody::PlayerPlatformerCharacterBody3D);
	GDREGISTER_CLASS(GaS::HeroPlayer);

}

void uninitialize_gems_and_souls_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT gems_and_souls_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	const GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_gems_and_souls_module);
	init_obj.register_terminator(uninitialize_gems_and_souls_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
