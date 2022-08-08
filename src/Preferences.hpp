#ifndef PREFERENCES_HPP_INCLUDED
#define PREFERENCES_HPP_INCLUDED

#include <Godot.hpp>
#include <InputEvent.hpp>
#include <Resource.hpp>

namespace godot
{
	class Preferences : public Resource
	{
		GODOT_CLASS(Preferences, Resource)

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
		String _lastGame;
		String _lastImage;
		Color _numbersColor;
		bool _numbersVisible;
		Color _outlinesColor;
		bool _outlinesVisible;
		int _rows;
		
	public:
		Preferences();
		~Preferences();

		void _init();
		void _ready();

		void load(const String path);
		void save(const String path);
		
		static void _register_methods();

		bool auto_load_get() const;
		void auto_load_set(bool newVal);
		
		String auto_path_get() const;
		void auto_path_set(const String newVal);
		
		bool auto_remove_on_win_get() const;
		void auto_remove_on_win_set(bool newVal);
		
		bool auto_save_get() const;
		void auto_save_set(bool newVal);
		
		int columns_get() const;
		void columns_set(int newVal);
		
		String default_theme_get() const;
		void default_theme_set(const String newVal);

		String default_image_get() const;
		void default_image_set(const String newVal);
		
		String last_game_get() const;
		void last_game_set(const String newVal);
		
		String last_image_get() const;
		void last_image_set(String newVal);
		
		Color numbers_color_get() const;
		void numbers_color_set(Color newVal);
		
		bool numbers_visible_get() const;
		void numbers_visible_set(bool newVal);
		
		Color outlines_color_get() const;
		void outlines_color_set(Color newVal);
		
		bool outlines_visible_get() const;
		void outlines_visible_set(bool newVal);
		
		int rows_get() const;
		void rows_set(int newVal);
	};
}

#endif	// PREFERENCES_HPP_INCLUDED
