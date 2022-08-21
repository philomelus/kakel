#ifndef GLOBALS_HPP_INCLUDED
#define GLOBALS_HPP_INCLUDED

#include <Godot.hpp>
#include <Node.hpp>
#include <Theme.hpp>

namespace godot
{
	class AppGlobals : public Node
	{
		GODOT_CLASS(AppGlobals, Node)

	private:
		// When true, the game was started from AutoPath because AutoLoad was true
		bool _autoStarted;

		// When true, globals have been initialized from preferences as needed.
		// This should only happen once at app start.
		bool _preferencesImported;
		
		// Theme for all UI
		Ref<Theme> _theme;
		
		// If true, use default image.
		bool _tilesDefaultImage;

		// When true, enhance blank tiles display
		bool _tilesHiliteBlank;
		
		// Initial state of keep aspect when starting game
		// (way to communicate from NewGame.tscn to Game.tscn)
		bool _tilesKeepAspect;
		
		// When true, the game in `TilesLoadPath` is loaded and restarted
		bool _tilesLoading;
		
		// When TilesLoading is true, path to game file to load.
		String _tilesLoadPath;
		
		// When true, tiles has started then user quit
		// (way to communicate from NewGame.tscn to Game.tscn)
		bool _tilesQuit;

		// Wether to use an image for tiles
		// (way to communicate from NewGame.tscn to Game.tscn)
		bool _tilesUseImage;

	public:
		AppGlobals();
		~AppGlobals();

		void _init();

		static void _register_methods();

		bool auto_started_get() const;
		void auto_started_set(const bool newVal);

		bool preferences_imported_get() const;
		void preferences_imported_set(const bool newVal);
		
		Ref<Theme> theme_get();
		void theme_set(Ref<Theme> newVal);
		
		bool tiles_default_image_get() const;
		void tiles_default_image_set(const bool newVal);

		bool tiles_hilite_blank_get() const;
		void tiles_hilite_blank_set(const bool newVal);

		bool tiles_keep_aspect_get() const;
		void tiles_keep_aspect_set(const bool newVal);
		
		bool tiles_loading_get() const;
		void tiles_loading_set(const bool newVal);

		String tiles_load_path_get();
		void tiles_load_path_set(const String newVal);
		
		bool tiles_quit_get() const;
		void tiles_quit_set(const bool newVal);

		bool tiles_use_image_get() const;
		void tiles_use_image_set(const bool newVal);
	};
}

#endif	// GLOBALS_HPP_INCLUDED
