#include "AppGlobals.hpp"
#include <ConfigFile.hpp>
#include <Directory.hpp>
#include <ResourceLoader.hpp>
#include "auto_free.hpp"
#include "function.hpp"

using namespace godot;

namespace
{
	const bool default_autoStarted = true;
	const bool default_preferencesImported = false;
	const bool default_tilesDefaultImage = false;
	const bool default_tilesHiliteBlank = false;
	const bool default_tilesKeepAspect = false;
	const bool default_tilesLoading = false;
	const char* default_tilesLoadPath;
	const bool default_tilesQuit = false;
	const bool default_tilesUseImage = true;
}

namespace godot
{
	void AppGlobals::_register_methods()
	{
		FUNC_("AppGlobals::_register_methods");
		
		// Properties
		register_property<AppGlobals, bool>("auto_started", &AppGlobals::auto_started_set, &AppGlobals::auto_started_get, default_autoStarted);
		register_property<AppGlobals, bool>("preferences_imported", &AppGlobals::preferences_imported_set, &AppGlobals::preferences_imported_get, default_preferencesImported);
		register_property<AppGlobals, bool>("tiles_default_image", &AppGlobals::tiles_default_image_set, &AppGlobals::tiles_default_image_get, default_tilesDefaultImage);
		register_property<AppGlobals, bool>("tiles_hilite_blank", &AppGlobals::tiles_hilite_blank_set, &AppGlobals::tiles_hilite_blank_get, default_tilesHiliteBlank);
		register_property<AppGlobals, bool>("tiles_keep_aspect", &AppGlobals::tiles_keep_aspect_set, &AppGlobals::tiles_keep_aspect_get, default_tilesKeepAspect);
		register_property<AppGlobals, bool>("tiles_loading", &AppGlobals::tiles_loading_set, &AppGlobals::tiles_loading_get, default_tilesLoading);
		register_property<AppGlobals, String>("tiles_load_path", &AppGlobals::tiles_load_path_set, &AppGlobals::tiles_load_path_get, default_tilesLoadPath);
		register_property<AppGlobals, bool>("tiles_quit", &AppGlobals::tiles_quit_set, &AppGlobals::tiles_quit_get, default_tilesQuit);
		register_property<AppGlobals, bool>("tiles_use_image", &AppGlobals::tiles_use_image_set, &AppGlobals::tiles_use_image_get, default_tilesUseImage);
	}

	AppGlobals::AppGlobals() :
		_autoStarted(default_autoStarted),
		_preferencesImported(default_preferencesImported),
		_tilesDefaultImage(default_tilesDefaultImage),
		_tilesHiliteBlank(default_tilesHiliteBlank),
		_tilesKeepAspect(default_tilesKeepAspect),
		_tilesLoading(default_tilesLoading),
		_tilesLoadPath(default_tilesLoadPath),
		_tilesQuit(default_tilesQuit),
		_tilesUseImage(default_tilesUseImage)
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
	
}
