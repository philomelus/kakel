#include "Preferences.hpp"
#include <ConfigFile.hpp>

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
    static const char* V_LASTGAME = "last_game";
    static const char* V_LASTIMAGE = "last_image";
    static const char* V_NUMBERCOLOR = "number_color";
    static const char* V_OUTLINECOLOR = "outline_color";
    static const char* V_ROWS = "rows";
    static const char* V_SHOWNUMBERS = "show_numbers";
    static const char* V_SHOWOUTLINES = "show_outlines";

    // Paths.
    static const char* P_DEFAULTAUTOPATH = "user://auto.kakel";
    static const char* P_DEFAULTIMAGE = "res://default_image.png";
    static const char* P_PREFS = "user://prefs.cfg";

	// Defaults
	static const bool default_autoLoad = true;
	static const char* default_autoPath = P_DEFAULTAUTOPATH;
	static const bool default_autoRemoveOnWin = true;
	static const bool default_autoSave = true;
	static const int default_columns = 4;
	static const char* default_defaultImage = P_DEFAULTIMAGE;
	static const char* default_lastGame = "";
	static const char* default_lastImage = "";
	static const Color default_numbersColor = Color(0.8, 0.8, 0.8, 1);
	static const bool default_numbersVisible = true;
	static const Color default_outlinesColor = Color(0.5, 0.5, 0.5, 1);
	static const bool default_outlinesVisible = false;
	static const int default_rows = 4;

	// Guaranteed memory release
	template <typename T>
	class auto_free
	{
	public:
		auto_free(T* v) { _v = v; }
		~auto_free() { godot::api->godot_free(_v); }
		T* operator->() { return _v; }
	private:
		T* _v;
	};
}

namespace godot
{
	void Preferences::_register_methods()
	{
		// API
		register_method("_ready", &Preferences::_ready);
		register_method("load", &Preferences::load);
		register_method("save", &Preferences::save);

		// Properties
		register_property<Preferences, bool>("auto_load", &Preferences::auto_load_set, &Preferences::auto_load_get, default_autoLoad);
		register_property<Preferences, String>("auto_path", &Preferences::auto_path_set, &Preferences::auto_path_get, default_autoPath);
		register_property<Preferences, bool>("auto_remove_on_win", &Preferences::auto_remove_on_win_set, &Preferences::auto_remove_on_win_get, default_autoRemoveOnWin);
		register_property<Preferences, bool>("auto_save", &Preferences::auto_save_set, &Preferences::auto_save_get, default_autoSave);
		register_property<Preferences, int>("columns", &Preferences::columns_set, &Preferences::columns_get, default_columns);
		register_property<Preferences, String>("default_image", &Preferences::default_image_set, &Preferences::default_image_get, default_defaultImage);
		register_property<Preferences, String>("last_game", &Preferences::last_game_set, &Preferences::last_game_get, default_lastGame);
		register_property<Preferences, String>("last_image", &Preferences::last_image_set, &Preferences::last_image_get, default_lastImage);
		register_property<Preferences, Color>("numbers_color", &Preferences::numbers_color_set, &Preferences::numbers_color_get, default_numbersColor);
		register_property<Preferences, bool>("numbers_visible", &Preferences::numbers_visible_set, &Preferences::numbers_visible_get, default_numbersVisible);
		register_property<Preferences, Color>("outlines_color", &Preferences::outlines_color_set, &Preferences::outlines_color_get, default_outlinesColor);
		register_property<Preferences, bool>("outlines_visible", &Preferences::outlines_visible_set, &Preferences::outlines_visible_get, default_outlinesVisible);
		register_property<Preferences, int>("rows", &Preferences::rows_set, &Preferences::rows_get, default_rows);
	}

	Preferences::Preferences()
	{
	}

	Preferences::~Preferences()
	{
	}

	void Preferences::_init()
	{
		godot::Godot::print(String("Preferences::_init called"));
	}

	void Preferences::_ready()
	{
		godot::Godot::print(String("Preferences::_ready called"));
	}
			
	bool Preferences::auto_load_get() const
	{
		return _autoLoad;
	}
	
	void Preferences::auto_load_set(bool newVal)
	{
		if (_autoLoad != newVal)
			_autoLoad = newVal;
	}
	
	String Preferences::auto_path_get() const
	{
		return _autoPath;
	}
	
	void Preferences::auto_path_set(const String newVal)
	{
		if (_autoPath != newVal)
			_autoPath = newVal;
	}
	
	bool Preferences::auto_remove_on_win_get() const
	{
		return _autoRemoveOnWin;
	}
	
	void Preferences::auto_remove_on_win_set(bool newVal)
	{
		if (_autoRemoveOnWin != newVal)
			_autoRemoveOnWin = newVal;
	}
	
	bool Preferences::auto_save_get() const
	{
		return _autoSave;
	}
	
	void Preferences::auto_save_set(bool newVal)
	{
		if (_autoSave != newVal)
			_autoSave = newVal;
	}
	
	int Preferences::columns_get() const
	{
		return _columns;
	}
	
	void Preferences::columns_set(int newVal)
	{
		if (_columns != newVal)
			_columns = newVal;
	}
	
	String Preferences::default_image_get() const
	{
		return _defaultImage;
	}
	
	void Preferences::default_image_set(const String newVal)
	{
		if (_defaultImage != newVal)
			_defaultImage = newVal;
	}
	
	String Preferences::last_game_get() const
	{
		return _lastGame;
	}
	
	void Preferences::last_game_set(const String newVal)
	{
		if (_lastGame != newVal)
			_lastGame = newVal;
	}
	
	String Preferences::last_image_get() const
	{
		return _lastImage;
	}
	
	void Preferences::last_image_set(String newVal)
	{
		if (_lastImage != newVal)
			_lastImage = newVal;
	}
	
	void Preferences::load(const String path)
	{
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
        _defaultImage = cf->get_value(S_GLOBALS, V_DEFAULTIMAGE, default_defaultImage);
        _lastGame = cf->get_value(S_GLOBALS, V_LASTGAME, default_lastGame);
        _lastImage = cf->get_value(S_GLOBALS, V_LASTIMAGE, default_lastImage);
        _numbersColor = Color(cf->get_value(S_GLOBALS, V_NUMBERCOLOR, default_numbersColor.to_html(true)));
        tmp = cf->get_value(S_GLOBALS, V_SHOWNUMBERS, default_numbersVisible ? 1 : 0);
		_numbersVisible = tmp == 1;
        _outlinesColor = Color(cf->get_value(S_GLOBALS, V_OUTLINECOLOR, default_outlinesColor.to_html(true)));
        tmp = cf->get_value(S_GLOBALS, V_SHOWOUTLINES, default_outlinesVisible ? 1 : 0);
		_outlinesVisible = tmp == 1;
        _rows = cf->get_value(S_GLOBALS, V_ROWS, default_rows);
	}
	
	Color Preferences::numbers_color_get() const
	{
		return _numbersColor;
	}
	
	void Preferences::numbers_color_set(Color newVal)
	{
		if (_numbersColor != newVal)
			_numbersColor = newVal;
	}
	
	bool Preferences::numbers_visible_get() const
	{
		return _numbersVisible;
	}
	
	void Preferences::numbers_visible_set(bool newVal)
	{
		if (_numbersVisible != newVal)
			_numbersVisible = newVal;
	}
	
	Color Preferences::outlines_color_get() const
	{
		return _outlinesColor;
	}
	
	void Preferences::outlines_color_set(Color newVal)
	{
		if (_outlinesColor != newVal)
			_outlinesColor = newVal;
	}
		
	bool Preferences::outlines_visible_get() const
	{
		return _outlinesVisible;
	}
	
	void Preferences::outlines_visible_set(bool newVal)
	{
		if (_outlinesVisible != newVal)
			_outlinesVisible = newVal;
	}
	
	int Preferences::rows_get() const
	{
		return _rows;
	}
	
	void Preferences::rows_set(int newVal)
	{
		if (_rows != newVal)
			_rows = newVal;
	}
	
	void Preferences::save(const String path)
	{
        ConfigFile* cf = ConfigFile::_new();
        cf->set_value(S_GLOBALS, V_VERSION, PREFS_VERSION);
        cf->set_value(S_GLOBALS, V_AUTOLOAD, _autoLoad ? 1 : 0);
        cf->set_value(S_GLOBALS, V_AUTOPATH, _autoPath);
        cf->set_value(S_GLOBALS, V_AUTOREMOVEONWIN, _autoRemoveOnWin ? 1 : 0);
        cf->set_value(S_GLOBALS, V_AUTOSAVE, _autoSave ? 1 : 0);
        cf->set_value(S_GLOBALS, V_COLUMNS, _columns);
        cf->set_value(S_GLOBALS, V_DEFAULTIMAGE, _defaultImage);
        cf->set_value(S_GLOBALS, V_LASTGAME, _lastGame);
        cf->set_value(S_GLOBALS, V_LASTIMAGE, _lastImage);
        cf->set_value(S_GLOBALS, V_NUMBERCOLOR, _numbersColor.to_html(true));
        cf->set_value(S_GLOBALS, V_OUTLINECOLOR, _outlinesColor.to_html(true));
        cf->set_value(S_GLOBALS, V_ROWS, _rows);
        cf->set_value(S_GLOBALS, V_SHOWOUTLINES, _outlinesVisible ? 1 : 0);
        cf->set_value(S_GLOBALS, V_SHOWNUMBERS, _numbersVisible ? 1 : 0);
        cf->save(path);
	}
}
