#ifndef GLOBALS_HPP_INCLUDED
#define GLOBALS_HPP_INCLUDED

#include <Godot.hpp>
#include <Resource.hpp>
#include "Preferences.hpp"

namespace godot
{
	class Globals : public Resource
	{
		GODOT_CLASS(Globals, Resource)

	private:
		// When true, the game was started from AutoPath because AutoLoad was true
		bool _autoStarted = false;

		// Preferences that are saved/loaded
		Ref<Preferences> _preferences;

		// If true, use default image.
		bool _tilesDefaultImage = true;

		// When true, the game in `TilesLoadPath` is loaded and restarted
		bool _tilesLoading = false;
		
		// When TilesLoading is true, path to game file to load.
		String _tilesLoadPath;
		
		// When true, use image for tiles.  Otherwise draw outlines with numbers.
		bool _tilesUseImage = true;
		
	public:
		Globals();
		~Globals();

		void _init();

		static void _register_methods();

		bool auto_started_get() const;
		void auto_started_set(bool newVal);

		Ref<Preferences> preferences_get();

		bool tiles_default_image_get() const;
		void tiles_default_image_set(bool newVal);

		bool tiles_loading_get() const;
		void tiles_loading_set(bool newVal);

		String tiles_load_path_get();
		void tiles_load_path_set(String newVal);

		bool tiles_use_image_get() const;
		void tiles_use_image_set(bool newVal);

	private:
		void preferences_set(Ref<Preferences> newVal);
	};
}

#endif	// GLOBALS_HPP_INCLUDED
