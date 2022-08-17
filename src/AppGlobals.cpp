#include "AppGlobals.hpp"
#include <godot_cpp/classes/config_file.hpp>
#include <godot_cpp/classes/directory.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "function.hpp"

using namespace godot;

namespace
{
	const bool default_autoStarted = true;
	const bool default_preferencesImported = false;
	const bool default_tilesDefaultImage = true;
	const bool default_tilesHiliteBlank = false;
	const bool default_tilesKeepAspect = false;
	const bool default_tilesLoading = false;
	const char* default_tilesLoadPath;
	const bool default_tilesUseImage = true;
	const bool default_tilesQuit = true;
}

void AppGlobals::_bind_methods()
{
	FUNC_("AppGlobals::_bind_methods");

	// Non-api functions
	ClassDB::bind_method(D_METHOD("auto_started_get"), &AppGlobals::auto_started_get);
	ClassDB::bind_method(D_METHOD("auto_started_set", "newVal"), &AppGlobals::auto_started_set);
	ClassDB::bind_method(D_METHOD("preferences_imported_get"), &AppGlobals::preferences_imported_get);
	ClassDB::bind_method(D_METHOD("preferences_imported_set", "newVal"), &AppGlobals::preferences_imported_set);
	ClassDB::bind_method(D_METHOD("tiles_default_image_get"), &AppGlobals::tiles_default_image_get);
	ClassDB::bind_method(D_METHOD("tiles_default_image_set", "newVal"), &AppGlobals::tiles_default_image_set);
	ClassDB::bind_method(D_METHOD("tiles_hilite_blank_get"), &AppGlobals::tiles_hilite_blank_get);
	ClassDB::bind_method(D_METHOD("tiles_hilite_blank_set", "newVal"), &AppGlobals::tiles_hilite_blank_set);
	ClassDB::bind_method(D_METHOD("tiles_keep_aspect_get"), &AppGlobals::tiles_keep_aspect_get);
	ClassDB::bind_method(D_METHOD("tiles_keep_aspect_set", "newVal"), &AppGlobals::tiles_keep_aspect_set);
	ClassDB::bind_method(D_METHOD("tiles_loading_get"), &AppGlobals::tiles_loading_get);
	ClassDB::bind_method(D_METHOD("tiles_loading_set", "newVal"), &AppGlobals::tiles_loading_set);
	ClassDB::bind_method(D_METHOD("tiles_load_path_get"), &AppGlobals::tiles_load_path_get);
	ClassDB::bind_method(D_METHOD("tiles_load_path_set", "newVal"), &AppGlobals::tiles_load_path_set);
	ClassDB::bind_method(D_METHOD("tiles_use_image_get"), &AppGlobals::tiles_use_image_get);
	ClassDB::bind_method(D_METHOD("tiles_use_image_set", "newVal"), &AppGlobals::tiles_use_image_set);
	ClassDB::bind_method(D_METHOD("tiles_quit_get"), &AppGlobals::tiles_quit_get);
	ClassDB::bind_method(D_METHOD("tiles_quit_set", "newVal"), &AppGlobals::tiles_quit_set);
	
	// Properties
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_started"), "auto_started_set", "auto_started_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "preferences_imported"), "preferences_imported_set", "preferences_imported_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "tiles_keep_aspect"), "tiles_keep_aspect_set", "tiles_keep_aspect_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "tiles_default_image"), "tiles_default_image_set", "tiles_default_image_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "tiles_hilite_blank"), "tiles_hilite_blank_set", "tiles_hilite_blank_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "tiles_loading"), "tiles_loading_set", "tiles_loading_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "tiles_load_path"), "tiles_load_path_set", "tiles_load_path_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "tiles_quit"), "tiles_quit_set", "tiles_quit_get");
}

AppGlobals::AppGlobals() :
	_autoStarted(default_autoStarted),
	_preferencesImported(default_preferencesImported),
	_tilesDefaultImage(default_tilesDefaultImage),
	_tilesKeepAspect(default_tilesKeepAspect),
	_tilesLoading(default_tilesLoading),
	_tilesLoadPath(default_tilesLoadPath),
	_tilesUseImage(default_tilesUseImage)
{
	FUNC_("AppGlobals::AppGlobals");
}

AppGlobals::~AppGlobals()
{
	FUNC_("AppGlobals::~AppGlobals");
}

bool AppGlobals::auto_started_get() const
{
	return _autoStarted;
}
	
void AppGlobals::auto_started_set(const bool newVal)
{
	if (_autoStarted != newVal)
		_autoStarted = newVal;
}
	
bool AppGlobals::preferences_imported_get() const
{
	return _preferencesImported;
}
	
void AppGlobals::preferences_imported_set(const bool newVal)
{
	if (_preferencesImported != newVal)
		_preferencesImported = newVal;
}
	
Ref<Theme> AppGlobals::theme_get()
{
	return _theme;
}
	
void AppGlobals::theme_set(Ref<Theme> newVal)
{
	_theme = newVal;
}
	
bool AppGlobals::tiles_default_image_get() const
{
	return _tilesDefaultImage;
}
	
void AppGlobals::tiles_default_image_set(const bool newVal)
{
	if (_tilesDefaultImage != newVal)
		_tilesDefaultImage = newVal;
}
	
bool AppGlobals::tiles_hilite_blank_get() const
{
	return _tilesHiliteBlank;
}

void AppGlobals::tiles_hilite_blank_set(const bool newVal)
{
	if (_tilesHiliteBlank != newVal)
		_tilesHiliteBlank = newVal;
}

bool AppGlobals::tiles_keep_aspect_get() const
{
	return _tilesKeepAspect;
}
	
void AppGlobals::tiles_keep_aspect_set(const bool newVal)
{
	if (_tilesKeepAspect != newVal)
		_tilesKeepAspect = newVal;
}
	
bool AppGlobals::tiles_loading_get() const
{
	return _tilesLoading;
}
	
void AppGlobals::tiles_loading_set(const bool newVal)
{
	if (_tilesLoading != newVal)
		_tilesLoading = newVal;
}

String AppGlobals::tiles_load_path_get()
{
	return _tilesLoadPath;
}
	
void AppGlobals::tiles_load_path_set(const String newVal)
{
	if (_tilesLoadPath != newVal)
		_tilesLoadPath = newVal;
}

bool AppGlobals::tiles_quit_get() const
{
	return _tilesQuit;
}
	
void AppGlobals::tiles_quit_set(const bool newVal)
{
	if (_tilesQuit != newVal)
		_tilesQuit = newVal;
}

bool AppGlobals::tiles_use_image_get() const
{
	return _tilesUseImage;
}
	
void AppGlobals::tiles_use_image_set(const bool newVal)
{
	if (_tilesUseImage != newVal)
		_tilesUseImage = newVal;
}
