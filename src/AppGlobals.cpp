#include "Globals.hpp"
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
	void Globals::_register_methods()
	{
		FUNC_("Globals::_register_methods");
		
		// Properties
		register_property<Globals, bool>("auto_started", &Globals::auto_started_set, &Globals::auto_started_get, default_autoStarted);
		register_property<Globals, bool>("tiles_default_image", &Globals::tiles_default_image_set, &Globals::tiles_default_image_get, default_tilesDefaultImage);
		register_property<Globals, bool>("tiles_loading", &Globals::tiles_loading_set, &Globals::tiles_loading_get, default_tilesLoading);
		register_property<Globals, String>("tiles_load_path", &Globals::tiles_load_path_set, &Globals::tiles_load_path_get, default_tilesLoadPath);
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

		Godot::print("Globals::_init: auto_started = {0}", _autoStarted);
		Godot::print("Globals::_init: tiles_default_image = {0}", _tilesDefaultImage);
		Godot::print("Globals::_init: tiles_loading = {0}", _tilesLoading);
		Godot::print("Globals::_init: tiles_load_path = \"{0}\"", _tilesLoadPath);
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
}
