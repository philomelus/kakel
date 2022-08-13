#include "Globals.hpp"
#include <godot_cpp/classes/config_file.hpp>
#include <godot_cpp/classes/directory.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "function.hpp"

using namespace godot;

namespace
{
	static const bool default_autoStarted = true;
	static const bool default_tilesDefaultImage = false;
	static const bool default_tilesLoading = false;
	static const char* default_tilesLoadPath;
}

void Globals::_bind_methods()
{
	FUNC_("Globals::_bind_methods");

	// Non-api functions
	ClassDB::bind_method(D_METHOD("auto_started_get"), &Globals::auto_started_get);
	ClassDB::bind_method(D_METHOD("auto_started_set", "newVal"), &Globals::auto_started_set);
	ClassDB::bind_method(D_METHOD("tiles_default_image_get"), &Globals::tiles_default_image_get);
	ClassDB::bind_method(D_METHOD("tiles_default_image_set", "newVal"), &Globals::tiles_default_image_set);
	ClassDB::bind_method(D_METHOD("tiles_loading_get"), &Globals::tiles_loading_get);
	ClassDB::bind_method(D_METHOD("tiles_loading_set", "newVal"), &Globals::tiles_loading_set);
	ClassDB::bind_method(D_METHOD("tiles_load_path_get"), &Globals::tiles_load_path_get);
	ClassDB::bind_method(D_METHOD("tiles_load_path_set", "newVal"), &Globals::tiles_load_path_set);
	
	// Properties
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_started"), "auto_started_set", "auto_started_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "tiles_default_image"), "tiles_default_image_set", "tiles_default_image_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "tiles_loading"), "tiles_loading_set", "tiles_loading_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "tiles_load_path"), "tiles_load_path_set", "tiles_load_path_get");
}

Globals::Globals() :
	_autoStarted(default_autoStarted),
	_tilesDefaultImage(default_tilesDefaultImage),
	_tilesLoading(default_tilesLoading),
	_tilesLoadPath(default_tilesLoadPath)
{
	FUNC_("Globals::Globals");
}

Globals::~Globals()
{
	FUNC_("Globals::~Globals");
}

void Globals::_init()
{
	FUNC_("Globals::_init");

	UtilityFunctions::print("Globals::_init: auto_started = {0}", _autoStarted);
	UtilityFunctions::print("Globals::_init: tiles_default_image = {0}", _tilesDefaultImage);
	UtilityFunctions::print("Globals::_init: tiles_loading = {0}", _tilesLoading);
	UtilityFunctions::print("Globals::_init: tiles_load_path = \"{0}\"", _tilesLoadPath);
}

bool Globals::auto_started_get() const
{
	return _autoStarted;
}
	
void Globals::auto_started_set(const bool newVal)
{
	if (_autoStarted != newVal)
		_autoStarted = newVal;
}
	
Ref<Theme> Globals::theme_get()
{
	return _theme;
}
	
void Globals::theme_set(Ref<Theme> newVal)
{
	_theme = newVal;
}
	
bool Globals::tiles_default_image_get() const
{
	return _tilesDefaultImage;
}
	
void Globals::tiles_default_image_set(const bool newVal)
{
	if (_tilesDefaultImage != newVal)
		_tilesDefaultImage = newVal;
}
	
bool Globals::tiles_loading_get() const
{
	return _tilesLoading;
}
	
void Globals::tiles_loading_set(const bool newVal)
{
	if (_tilesLoading != newVal)
		_tilesLoading = newVal;
}

String Globals::tiles_load_path_get()
{
	return _tilesLoadPath;
}
	
void Globals::tiles_load_path_set(const String newVal)
{
	if (_tilesLoadPath != newVal)
		_tilesLoadPath = newVal;
}

