#include "Globals.hpp"
#include <ConfigFile.hpp>
#include <Directory.hpp>
#include <ResourceLoader.hpp>
#include "auto_free.hpp"

using namespace godot;

namespace
{
	static const bool default_autoStarted = true;
	static const Ref<Preferences> default_preferences;
	static const bool default_tilesDefaultImage = false;
	static const bool default_tilesLoading = false;
	static const char* default_tilesLoadPath;
	static const bool default_tilesUseImage = false;
}

namespace godot
{
	void Globals::_register_methods()
	{
		// Properties
		register_property<Globals, bool>("auto_started", &Globals::auto_started_set, &Globals::auto_started_get, default_autoStarted);
		register_property<Globals, Ref<Preferences>>("preferences", &Globals::preferences_set, &Globals::preferences_get, default_preferences);
		register_property<Globals, bool>("tiles_default_image", &Globals::tiles_default_image_set, &Globals::tiles_default_image_get, default_tilesDefaultImage);
		register_property<Globals, bool>("tiles_loading", &Globals::tiles_loading_set, &Globals::tiles_loading_get, default_tilesLoading);
		register_property<Globals, String>("tiles_load_path", &Globals::tiles_load_path_set, &Globals::tiles_load_path_get, default_tilesLoadPath);
		register_property<Globals, bool>("tiles_use_image", &Globals::tiles_use_image_set, &Globals::tiles_use_image_get, default_tilesUseImage);
	}

	Globals::Globals()
		: _preferences(Preferences::_new())
	{
		godot::Godot::print("Globals::Globals called");
	}

	Globals::~Globals()
	{
		godot::Godot::print("Globals::~Globals called");
	}

	void Globals::_init()
	{
		godot::Godot::print("Globals::_init called");

		// Load preferences if exists
		auto_free<Directory> d(Directory::_new());
		if (d->file_exists(_preferences->auto_load_get()))
			_preferences->load(_preferences->auto_load_get());
		
		// Load theme
		if (d->file_exists(_preferences->default_theme_get()))
			_theme = ResourceLoader::get_singleton()->load(_preferences->default_theme_get(), "Theme");
	}

	bool Globals::auto_started_get() const
	{
		return _autoStarted;
	}
	
	void Globals::auto_started_set(bool newVal)
	{
		if (_autoStarted != newVal)
			_autoStarted = newVal;
	}
	
	Ref<Preferences> Globals::preferences_get()
	{
		return _preferences;
	}
	
	void Globals::preferences_set(Ref<Preferences> newVal)
	{
		CRASH_NOW();
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
	
	void Globals::tiles_default_image_set(bool newVal)
	{
		if (_tilesDefaultImage != newVal)
			_tilesDefaultImage = newVal;
	}
	
	bool Globals::tiles_loading_get() const
	{
		return _tilesLoading;
	}
	
	void Globals::tiles_loading_set(bool newVal)
	{
		if (_tilesLoading != newVal)
			_tilesLoading = newVal;
	}

	String Globals::tiles_load_path_get()
	{
		return _tilesLoadPath;
	}
	
	void Globals::tiles_load_path_set(String newVal)
	{
		if (_tilesLoadPath != newVal)
			_tilesLoadPath = newVal;
	}
	
	bool Globals::tiles_use_image_get() const
	{
		return _tilesUseImage;
	}
	
	void Globals::tiles_use_image_set(bool newVal)
	{
		if (_tilesUseImage != newVal)
			_tilesUseImage = newVal;
	}
}
