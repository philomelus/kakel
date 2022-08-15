#ifndef APPGLOBALS_HPP
#define APPGLOBALS_HPP

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/theme.hpp>

namespace godot
{
	class AppGlobals : public Node
	{
		GDCLASS(AppGlobals, Node);

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

	protected:
		static void _bind_methods();

	public:
		AppGlobals();
		virtual ~AppGlobals();

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

#endif	// KAKELGLOBALS_HPP
