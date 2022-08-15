#include "KakelGlobals.hpp"
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

void KakelGlobals::_bind_methods()
{
	FUNC_("KakelGlobals::_bind_methods");

	// Non-api functions
	ClassDB::bind_method(D_METHOD("auto_started_get"), &KakelGlobals::auto_started_get);
	ClassDB::bind_method(D_METHOD("auto_started_set", "newVal"), &KakelGlobals::auto_started_set);
	ClassDB::bind_method(D_METHOD("tiles_default_image_get"), &KakelGlobals::tiles_default_image_get);
	ClassDB::bind_method(D_METHOD("tiles_default_image_set", "newVal"), &KakelGlobals::tiles_default_image_set);
	ClassDB::bind_method(D_METHOD("tiles_loading_get"), &KakelGlobals::tiles_loading_get);
	ClassDB::bind_method(D_METHOD("tiles_loading_set", "newVal"), &KakelGlobals::tiles_loading_set);
	ClassDB::bind_method(D_METHOD("tiles_load_path_get"), &KakelGlobals::tiles_load_path_get);
	ClassDB::bind_method(D_METHOD("tiles_load_path_set", "newVal"), &KakelGlobals::tiles_load_path_set);
	
	// Properties
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_started"), "auto_started_set", "auto_started_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "tiles_default_image"), "tiles_default_image_set", "tiles_default_image_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "tiles_loading"), "tiles_loading_set", "tiles_loading_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "tiles_load_path"), "tiles_load_path_set", "tiles_load_path_get");
}

KakelGlobals::KakelGlobals() :
	_autoStarted(default_autoStarted),
	_tilesDefaultImage(default_tilesDefaultImage),
	_tilesLoading(default_tilesLoading),
	_tilesLoadPath(default_tilesLoadPath)
{
	FUNC_("KakelGlobals::KakelGlobals");
}

KakelGlobals::~KakelGlobals()
{
	FUNC_("KakelGlobals::~KakelGlobals");
}

bool KakelGlobals::auto_started_get() const
{
	return _autoStarted;
}
	
void KakelGlobals::auto_started_set(const bool newVal)
{
	if (_autoStarted != newVal)
		_autoStarted = newVal;
}
	
Ref<Theme> KakelGlobals::theme_get()
{
	return _theme;
}
	
void KakelGlobals::theme_set(Ref<Theme> newVal)
{
	_theme = newVal;
}
	
bool KakelGlobals::tiles_default_image_get() const
{
	return _tilesDefaultImage;
}
	
void KakelGlobals::tiles_default_image_set(const bool newVal)
{
	if (_tilesDefaultImage != newVal)
		_tilesDefaultImage = newVal;
}
	
bool KakelGlobals::tiles_loading_get() const
{
	return _tilesLoading;
}
	
void KakelGlobals::tiles_loading_set(const bool newVal)
{
	if (_tilesLoading != newVal)
		_tilesLoading = newVal;
}

String KakelGlobals::tiles_load_path_get()
{
	return _tilesLoadPath;
}
	
void KakelGlobals::tiles_load_path_set(const String newVal)
{
	if (_tilesLoadPath != newVal)
		_tilesLoadPath = newVal;
}

