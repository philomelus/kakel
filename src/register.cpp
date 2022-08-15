#include <godot/gdnative_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "Game.hpp"
#include "KakelGlobals.hpp"
#include "KakelPreferences.hpp"
#include "Main.hpp"
#include "NewGame.hpp"
#include "Prefs.hpp"
#include "TilesControl.hpp"

using namespace godot;

void initialize_module(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;

    ClassDB::register_class<Game>();
    ClassDB::register_class<KakelGlobals>();
    ClassDB::register_class<KakelPreferences>();
    ClassDB::register_class<Main>();
    ClassDB::register_class<NewGame>();
    ClassDB::register_class<Prefs>();
	ClassDB::register_class<TilesControl>();
}

void uninitialize_module(ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE)
		return;
}

// Initialization.
extern "C"
{
	GDNativeBool GDN_EXPORT kakel_init(const GDNativeInterface* p_interface,
									   const GDNativeExtensionClassLibraryPtr p_library,
									   GDNativeInitialization* r_initialization)
	{
		godot::GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

		init_obj.register_initializer(initialize_module);
		init_obj.register_terminator(uninitialize_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}
