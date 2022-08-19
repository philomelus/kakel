#include "AppPreferences.hpp"
#include <ConfigFile.hpp>
#include <Directory.hpp>
#include "auto_free.hpp"
#include "function.hpp"

using namespace godot;

namespace
{
    static int PREFS_VERSION = 1;

    // Sections.
    static const char* S_GLOBALS = "globals";

    // Values.
    static const char* V_VERSION = "version";
    static const char* V_AUTOLOAD = "auto_load";
    static const char* V_AUTOPATH = "auto_path";
    static const char* V_AUTOREMOVEONWIN = "auto_remove_on_win";
    static const char* V_AUTOSAVE = "auto_save";
    static const char* V_COLUMNS = "columns";
    static const char* V_DEFAULTIMAGE = "default_image";
    static const char* V_DEFAULTTHEME = "default_theme";
    static const char* V_LASTGAME = "last_game";
    static const char* V_LASTIMAGE = "last_image";
    static const char* V_NUMBERCOLOR = "number_color";
    static const char* V_NUMBERSVISIBLE = "numbers_visible";
    static const char* V_OUTLINECOLOR = "outline_color";
    static const char* V_OUTLINESVISIBLE = "outlines_visible";
    static const char* V_ROWS = "rows";
	static const char* V_USEIMAGE = "use_image";
	
    // Paths.
    static const char* P_DEFAULTAUTOPATH = "user://auto.kakel";
	static const char* P_DEFAULTTHEME = "res://theme.tres";
    static const char* P_DEFAULTIMAGE = "res://default_image.png";

	// Defaults
	static const bool default_autoLoad = true;
	static const char* default_autoPath = P_DEFAULTAUTOPATH;
	static const bool default_autoRemoveOnWin = true;
	static const bool default_autoSave = true;
	static const int default_columns = 4;
	static const char* default_defaultImage = P_DEFAULTIMAGE;
	static const char* default_defaultTheme = P_DEFAULTTHEME;
	static const char* default_lastGame = "";
	static const char* default_lastImage = "";
	static const Color default_numbersColor = Color(0.8, 0.8, 0.8, 1);
	static const bool default_numbersVisible = true;
	static const Color default_outlinesColor = Color(0.5, 0.5, 0.5, 1);
	static const bool default_outlinesVisible = false;
	static const int default_rows = 4;
	static const bool default_useImage = true;
}


namespace godot
{
    const char* AppPreferences::P_PREFS = "user://prefs.cfg";
	
	void AppPreferences::_register_methods()
	{
		FUNC_("AppPreferences::_register_methods");
		
		// API
		register_method("_ready", &AppPreferences::_ready);
		register_method("load", &AppPreferences::load);
		register_method("save", &AppPreferences::save);

		// Properties
		register_property<AppPreferences, bool>("auto_load", &AppPreferences::auto_load_set, &AppPreferences::auto_load_get, default_autoLoad);
		register_property<AppPreferences, String>("auto_path", &AppPreferences::auto_path_set, &AppPreferences::auto_path_get, default_autoPath);
		register_property<AppPreferences, bool>("auto_remove_on_win", &AppPreferences::auto_remove_on_win_set, &AppPreferences::auto_remove_on_win_get, default_autoRemoveOnWin);
		register_property<AppPreferences, bool>("auto_save", &AppPreferences::auto_save_set, &AppPreferences::auto_save_get, default_autoSave);
		register_property<AppPreferences, int>("columns", &AppPreferences::columns_set, &AppPreferences::columns_get, default_columns);
		register_property<AppPreferences, String>("default_theme", &AppPreferences::default_theme_set, &AppPreferences::default_theme_get, default_defaultTheme);
		register_property<AppPreferences, String>("default_image", &AppPreferences::default_image_set, &AppPreferences::default_image_get, default_defaultImage);
		register_property<AppPreferences, String>("last_game", &AppPreferences::last_game_set, &AppPreferences::last_game_get, default_lastGame);
		register_property<AppPreferences, String>("last_image", &AppPreferences::last_image_set, &AppPreferences::last_image_get, default_lastImage);
		register_property<AppPreferences, Color>("numbers_color", &AppPreferences::numbers_color_set, &AppPreferences::numbers_color_get, default_numbersColor);
		register_property<AppPreferences, bool>("numbers_visible", &AppPreferences::numbers_visible_set, &AppPreferences::numbers_visible_get, default_numbersVisible);
		register_property<AppPreferences, Color>("outlines_color", &AppPreferences::outlines_color_set, &AppPreferences::outlines_color_get, default_outlinesColor);
		register_property<AppPreferences, bool>("outlines_visible", &AppPreferences::outlines_visible_set, &AppPreferences::outlines_visible_get, default_outlinesVisible);
		register_property<AppPreferences, int>("rows", &AppPreferences::rows_set, &AppPreferences::rows_get, default_rows);
		register_property<AppPreferences, bool>("use_image", &AppPreferences::use_image_set, &AppPreferences::use_image_get, default_useImage);
	}

	AppPreferences::AppPreferences()
	{
		FUNC_("AppPreferences::AppPreferences");
	}

	AppPreferences::~AppPreferences()
	{
		FUNC_("AppPreferences::~AppPreferences");
	}

	void AppPreferences::_init()
	{
		FUNC_("AppPreferences::_init");
		
		_autoLoad = default_autoLoad;
		_autoPath = default_autoPath;
		_autoRemoveOnWin = default_autoRemoveOnWin;
		_autoSave = default_autoSave;
		_columns = default_columns;
		_defaultTheme = default_defaultTheme;
		_defaultImage = default_defaultImage;
		_lastGame = default_lastGame;
		_lastImage = default_lastImage;
		_numbersColor = default_numbersColor;
		_numbersVisible = default_numbersVisible;
		_outlinesColor = default_outlinesColor;
		_outlinesVisible = default_outlinesVisible;
		_rows = default_rows;
		_useImage = default_useImage;
		
		// Load preferences if exists
		Ref<Directory> d(Directory::_new());
		if (d->file_exists(P_PREFS))
		{
			FUNCP_("AppPreferences::_init: Loading preferences from \"{0}\"", P_PREFS);
			load(P_PREFS);
		}
	}

	void AppPreferences::_ready()
	{
		FUNC_("AppPreferences::_ready");
	}
			
	bool AppPreferences::auto_load_get() const
	{
		return _autoLoad;
	}
	
	void AppPreferences::auto_load_set(bool newVal)
	{
		if (_autoLoad != newVal)
			_autoLoad = newVal;
	}
	
	String AppPreferences::auto_path_get() const
	{
		return _autoPath;
	}
	
	void AppPreferences::auto_path_set(const String newVal)
	{
		if (_autoPath != newVal)
			_autoPath = newVal;
	}
	
	bool AppPreferences::auto_remove_on_win_get() const
	{
		return _autoRemoveOnWin;
	}
	
	void AppPreferences::auto_remove_on_win_set(bool newVal)
	{
		if (_autoRemoveOnWin != newVal)
			_autoRemoveOnWin = newVal;
	}
	
	bool AppPreferences::auto_save_get() const
	{
		return _autoSave;
	}
	
	void AppPreferences::auto_save_set(bool newVal)
	{
		if (_autoSave != newVal)
			_autoSave = newVal;
	}
	
	int AppPreferences::columns_get() const
	{
		return _columns;
	}
	
	void AppPreferences::columns_set(int newVal)
	{
		if (_columns != newVal)
			_columns = newVal;
	}
	
	String AppPreferences::default_theme_get() const
	{
		return _defaultTheme;
	}
	
	void AppPreferences::default_theme_set(const String newVal)
	{
		if (_defaultTheme != newVal)
			_defaultTheme = newVal;
	}
	
	String AppPreferences::default_image_get() const
	{
		return _defaultImage;
	}
	
	void AppPreferences::default_image_set(const String newVal)
	{
		if (_defaultImage != newVal)
			_defaultImage = newVal;
	}
	
	String AppPreferences::last_game_get() const
	{
		return _lastGame;
	}
	
	void AppPreferences::last_game_set(const String newVal)
	{
		if (_lastGame != newVal)
			_lastGame = newVal;
	}
	
	String AppPreferences::last_image_get() const
	{
		return _lastImage;
	}
	
	void AppPreferences::last_image_set(String newVal)
	{
		if (_lastImage != newVal)
			_lastImage = newVal;
	}
	
	void AppPreferences::load(const String path)
	{
		FUNC_("AppPreferences::load");
		
		auto_free<ConfigFile> cf(ConfigFile::_new());
        cf->load(path);
        int tmp = (int) cf->get_value(S_GLOBALS, V_VERSION, PREFS_VERSION);
		ERR_FAIL_COND(tmp != PREFS_VERSION);
        tmp = cf->get_value(S_GLOBALS, V_AUTOLOAD, default_autoLoad ? 1 : 0);
		_autoLoad = tmp == 1;
        _autoPath = cf->get_value(S_GLOBALS, V_AUTOPATH, default_autoPath);
        tmp = cf->get_value(S_GLOBALS, V_AUTOREMOVEONWIN, default_autoRemoveOnWin ? 1 : 0);
		_autoRemoveOnWin = tmp == 1;
        tmp = cf->get_value(S_GLOBALS, V_AUTOSAVE, default_autoSave ? 1 : 0);
		_autoSave = tmp == 1;
        _columns = cf->get_value(S_GLOBALS, V_COLUMNS, default_columns);
        _defaultTheme = cf->get_value(S_GLOBALS, V_DEFAULTTHEME, default_defaultTheme);
        _defaultImage = cf->get_value(S_GLOBALS, V_DEFAULTIMAGE, default_defaultImage);
        _lastGame = cf->get_value(S_GLOBALS, V_LASTGAME, default_lastGame);
        _lastImage = cf->get_value(S_GLOBALS, V_LASTIMAGE, default_lastImage);
        _numbersColor = Color(cf->get_value(S_GLOBALS, V_NUMBERCOLOR, default_numbersColor.to_html(true)));
        tmp = cf->get_value(S_GLOBALS, V_NUMBERSVISIBLE, default_numbersVisible ? 1 : 0);
		_numbersVisible = tmp == 1;
        _outlinesColor = Color(cf->get_value(S_GLOBALS, V_OUTLINECOLOR, default_outlinesColor.to_html(true)));
        tmp = cf->get_value(S_GLOBALS, V_OUTLINESVISIBLE, default_outlinesVisible ? 1 : 0);
		_outlinesVisible = tmp == 1;
        _rows = cf->get_value(S_GLOBALS, V_ROWS, default_rows);
	}
	
	Color AppPreferences::numbers_color_get() const
	{
		return _numbersColor;
	}
	
	void AppPreferences::numbers_color_set(const Color newVal)
	{
		if (_numbersColor != newVal)
			_numbersColor = newVal;
	}
	
	bool AppPreferences::numbers_visible_get() const
	{
		return _numbersVisible;
	}
	
	void AppPreferences::numbers_visible_set(const bool newVal)
	{
		if (_numbersVisible != newVal)
			_numbersVisible = newVal;
	}
	
	Color AppPreferences::outlines_color_get() const
	{
		return _outlinesColor;
	}
	
	void AppPreferences::outlines_color_set(const Color newVal)
	{
		if (_outlinesColor != newVal)
			_outlinesColor = newVal;
	}
		
	bool AppPreferences::outlines_visible_get() const
	{
		return _outlinesVisible;
	}
	
	void AppPreferences::outlines_visible_set(const bool newVal)
	{
		if (_outlinesVisible != newVal)
			_outlinesVisible = newVal;
	}
	
	int AppPreferences::rows_get() const
	{
		return _rows;
	}
	
	void AppPreferences::rows_set(const int newVal)
	{
		if (_rows != newVal)
			_rows = newVal;
	}

	bool AppPreferences::use_image_get() const
	{
		return _useImage;
	}

	void AppPreferences::use_image_set(const bool newVal)
	{
		if (_useImage != newVal)
			_useImage = newVal;
	}
	
	void AppPreferences::save(const String path)
	{
		FUNC_("AppPreferences::save");
		
        auto_free<ConfigFile> cf(ConfigFile::_new());
        cf->set_value(S_GLOBALS, V_VERSION, PREFS_VERSION);
        cf->set_value(S_GLOBALS, V_AUTOLOAD, _autoLoad ? 1 : 0);
        cf->set_value(S_GLOBALS, V_AUTOPATH, _autoPath);
        cf->set_value(S_GLOBALS, V_AUTOREMOVEONWIN, _autoRemoveOnWin ? 1 : 0);
        cf->set_value(S_GLOBALS, V_AUTOSAVE, _autoSave ? 1 : 0);
        cf->set_value(S_GLOBALS, V_COLUMNS, _columns);
        cf->set_value(S_GLOBALS, V_DEFAULTTHEME, _defaultTheme);
        cf->set_value(S_GLOBALS, V_DEFAULTIMAGE, _defaultImage);
        cf->set_value(S_GLOBALS, V_LASTGAME, _lastGame);
        cf->set_value(S_GLOBALS, V_LASTIMAGE, _lastImage);
        cf->set_value(S_GLOBALS, V_NUMBERCOLOR, _numbersColor.to_html(true));
        cf->set_value(S_GLOBALS, V_NUMBERSVISIBLE, _numbersVisible ? 1 : 0);
        cf->set_value(S_GLOBALS, V_OUTLINECOLOR, _outlinesColor.to_html(true));
        cf->set_value(S_GLOBALS, V_OUTLINESVISIBLE, _outlinesVisible ? 1 : 0);
        cf->set_value(S_GLOBALS, V_ROWS, _rows);
        cf->save(path);
	}
}
