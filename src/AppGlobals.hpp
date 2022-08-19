#ifndef GLOBALS_HPP_INCLUDED
#define GLOBALS_HPP_INCLUDED

#include <Godot.hpp>
#include <Node.hpp>
#include <Theme.hpp>

namespace godot
{
	class Globals : public Node
	{
		GODOT_CLASS(Globals, Node)

	private:
		// When true, the game was started from AutoPath because AutoLoad was true
		bool _autoStarted;

		// Theme for all UI
		Ref<Theme> _theme;
		
		// If true, use default image.
		bool _tilesDefaultImage;

		// When true, the game in `TilesLoadPath` is loaded and restarted
		bool _tilesLoading;
		
		// When TilesLoading is true, path to game file to load.
		String _tilesLoadPath;
		
	public:
		Globals();
		~Globals();

		void _init();

		static void _register_methods();

		bool auto_started_get() const;
		void auto_started_set(const bool newVal);

		Ref<Theme> theme_get();
		void theme_set(Ref<Theme> newVal);
		
		bool tiles_default_image_get() const;
		void tiles_default_image_set(const bool newVal);

		bool tiles_loading_get() const;
		void tiles_loading_set(const bool newVal);

		String tiles_load_path_get();
		void tiles_load_path_set(const String newVal);
	};
}

#endif	// GLOBALS_HPP_INCLUDED
