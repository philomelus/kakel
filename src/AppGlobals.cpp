#include "AppGlobals.hpp"
#include <ConfigFile.hpp>
#include <Directory.hpp>
#include <ResourceLoader.hpp>
#include "auto_free.hpp"
#include "function.hpp"

using namespace godot;

namespace
{
	static const bool default_autoStarted = true;
	static const bool default_tilesDefaultImage = false;
	static const bool default_tilesLoading = false;
	static const char* default_tilesLoadPath;
}

namespace godot
{
	void AppGlobals::_register_methods()
	{
		FUNC_("AppGlobals::_register_methods");
		
		// Properties
		register_property<AppGlobals, bool>("auto_started", &AppGlobals::auto_started_set, &AppGlobals::auto_started_get, default_autoStarted);
		register_property<AppGlobals, bool>("tiles_default_image", &AppGlobals::tiles_default_image_set, &AppGlobals::tiles_default_image_get, default_tilesDefaultImage);
		register_property<AppGlobals, bool>("tiles_loading", &AppGlobals::tiles_loading_set, &AppGlobals::tiles_loading_get, default_tilesLoading);
		register_property<AppGlobals, String>("tiles_load_path", &AppGlobals::tiles_load_path_set, &AppGlobals::tiles_load_path_get, default_tilesLoadPath);
	}

	AppGlobals::AppGlobals() :
		_autoStarted(default_autoStarted),
		_tilesDefaultImage(default_tilesDefaultImage),
		_tilesLoading(default_tilesLoading),
		_tilesLoadPath(default_tilesLoadPath)
	{
		FUNC_("AppGlobals::AppGlobals");
	}

	AppGlobals::~AppGlobals()
	{
		FUNC_("AppGlobals::~AppGlobals");
	}

	void AppGlobals::_init()
	{
		FUNC_("AppGlobals::_init");

		FUNCP_("AppGlobals::_init: auto_started = {0}", _autoStarted);
		FUNCP_("AppGlobals::_init: tiles_default_image = {0}", _tilesDefaultImage);
		FUNCP_("AppGlobals::_init: tiles_loading = {0}", _tilesLoading);
		FUNCP_("AppGlobals::_init: tiles_load_path = \"{0}\"", _tilesLoadPath);
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
}
