#ifndef PREFERENCES_HPP
#define PREFERENCES_HPP

#include <godot_cpp/classes/node.hpp>

namespace godot
{
	class AppPreferences : public Node
	{
		GDCLASS(AppPreferences, Node);

	public:
		static const char* P_PREFS;
		
	private:
		bool _autoLoad;
		String _autoPath;
		bool _autoRemoveOnWin;
		bool _autoSave;
		int _columns;
		String _defaultImage;
		String _defaultTheme;
		bool _keepAspect;
		String _lastGame;
		String _lastImage;
		Color _numbersColor;
		bool _numbersVisible;
		Color _outlinesColor;
		bool _outlinesVisible;
		int _rows;
		bool _useImage;

	protected:
		static void _bind_methods();

	public:
		AppPreferences();
		~AppPreferences();

		void _ready() override;

		void load(const String path);
		void save(const String path);
		
		bool auto_load_get() const;
		void auto_load_set(const bool newVal);
		
		String auto_path_get() const;
		void auto_path_set(const String newVal);
		
		bool auto_remove_on_win_get() const;
		void auto_remove_on_win_set(const bool newVal);
		
		bool auto_save_get() const;
		void auto_save_set(const bool newVal);
		
		int columns_get() const;
		void columns_set(const int newVal);
		
		String default_theme_get() const;
		void default_theme_set(const String newVal);

		String default_image_get() const;
		void default_image_set(const String newVal);

		bool keep_aspect_get() const;
		void keep_aspect_set(const bool newVal);
		
		String last_game_get() const;
		void last_game_set(const String newVal);
		
		String last_image_get() const;
		void last_image_set(const String newVal);
		
		Color numbers_color_get() const;
		void numbers_color_set(const Color newVal);
		
		bool numbers_visible_get() const;
		void numbers_visible_set(const bool newVal);
		
		Color outlines_color_get() const;
		void outlines_color_set(const Color newVal);
		
		bool outlines_visible_get() const;
		void outlines_visible_set(const bool newVal);
		
		int rows_get() const;
		void rows_set(const int newVal);

		bool use_image_get() const;
		void use_image_set(const bool newVal);
	};
}

#endif	// KAKELPREFERENCES_HPP
