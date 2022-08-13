#include "Preferences.hpp"
#include <godot_cpp/classes/config_file.hpp>
#include <godot_cpp/classes/directory.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
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

const char* Preferences::P_PREFS = "user://prefs.cfg";
	
void Preferences::_bind_methods()
{
	FUNC_("Preferences::_bind_methods");
		
	// API
	ClassDB::bind_method(D_METHOD("load"), &Preferences::load);
	ClassDB::bind_method(D_METHOD("save"), &Preferences::save);

	// Non-api functions
	ClassDB::bind_method(D_METHOD("auto_load_get"), &Preferences::auto_load_get);
	ClassDB::bind_method(D_METHOD("auto_load_set", "newVal"), &Preferences::auto_load_set);
	ClassDB::bind_method(D_METHOD("auto_path_get"), &Preferences::auto_path_get);
	ClassDB::bind_method(D_METHOD("auto_path_set", "newVal"), &Preferences::auto_path_set);
	ClassDB::bind_method(D_METHOD("auto_remove_on_win_get"), &Preferences::auto_remove_on_win_get);
	ClassDB::bind_method(D_METHOD("auto_remove_on_win_set", "newVal"), &Preferences::auto_remove_on_win_set);
	ClassDB::bind_method(D_METHOD("auto_save_get"), &Preferences::auto_save_get);
	ClassDB::bind_method(D_METHOD("auto_save_set", "newVal"), &Preferences::auto_save_set);
	ClassDB::bind_method(D_METHOD("columns_get"), &Preferences::columns_get);
	ClassDB::bind_method(D_METHOD("columns_set", "newVal"), &Preferences::columns_set);
	ClassDB::bind_method(D_METHOD("default_image_get"), &Preferences::default_image_get);
	ClassDB::bind_method(D_METHOD("default_image_set", "newVal"), &Preferences::default_image_set);
	ClassDB::bind_method(D_METHOD("default_theme_get"), &Preferences::default_theme_get);
	ClassDB::bind_method(D_METHOD("default_theme_set", "newVal"), &Preferences::default_theme_set);
	ClassDB::bind_method(D_METHOD("last_game_get"), &Preferences::last_game_get);
	ClassDB::bind_method(D_METHOD("last_game_set", "newVal"), &Preferences::last_game_set);
	ClassDB::bind_method(D_METHOD("last_image_get"), &Preferences::last_image_get);
	ClassDB::bind_method(D_METHOD("last_image_set", "newVal"), &Preferences::last_image_set);
	ClassDB::bind_method(D_METHOD("numbers_color_get"), &Preferences::numbers_color_get);
	ClassDB::bind_method(D_METHOD("numbers_color_set", "newVal"), &Preferences::numbers_color_set);
	ClassDB::bind_method(D_METHOD("numbers_visible_get"), &Preferences::numbers_visible_get);
	ClassDB::bind_method(D_METHOD("numbers_visible_set", "newVal"), &Preferences::numbers_visible_set);
	ClassDB::bind_method(D_METHOD("outlines_color_get"), &Preferences::outlines_color_get);
	ClassDB::bind_method(D_METHOD("outlines_color_set", "newVal"), &Preferences::outlines_color_set);
	ClassDB::bind_method(D_METHOD("outlines_visible_get"), &Preferences::outlines_visible_get);
	ClassDB::bind_method(D_METHOD("outlines_visible_set", "newVal"), &Preferences::outlines_visible_set);
	ClassDB::bind_method(D_METHOD("rows_get"), &Preferences::rows_get);
	ClassDB::bind_method(D_METHOD("rows_set", "newVal"), &Preferences::rows_set);
	ClassDB::bind_method(D_METHOD("use_image_get"), &Preferences::use_image_get);
	ClassDB::bind_method(D_METHOD("use_image_set", "newVal"), &Preferences::use_image_set);
	
	// Properties
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_load"), "auto_load_set",	"auto_load_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "auto_path"), "auto_path_set", "auto_path_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_remove_on_win"), "auto_remove_on_win_set", "auto_remove_on_win_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_save"), "auto_save_set", "auto_save_get");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "columns"), "columns_set", "columns_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "default_image"), "default_image_set", "default_image_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "default_theme"), "default_theme_set", "default_theme_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "last_game"), "last_game_set", "last_game_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "last_image"), "last_image_set", "last_image_get");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "numbers_color"), "numbers_color_set", "numbers_color_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "numbers_visible"), "numbers_visible_set", "numbers_visible_get");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "outlines_color"), "outlines_color_set", "outlines_color_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "outlines_visible"), "outlines_visible_set", "outlines_visible_get");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "rows"), "rows_set", "rows_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_image"), "use_image_set", "use_image_get");
}

Preferences::Preferences()
{
	FUNC_("Preferences::Preferences");

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
	Ref<Directory> d = memnew(Directory);
	if (d->file_exists(P_PREFS))
	{
		UtilityFunctions::print("Preferences::Preferences: Loading preferences from \"{0}\"", P_PREFS);
		load(P_PREFS);
	}
}

Preferences::~Preferences()
{
	FUNC_("Preferences::~Preferences");
}

void Preferences::_ready()
{
	FUNC_("Preferences::_ready");
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
	
void Preferences::default_image_set(const String newVal)
{
	if (_defaultImage != newVal)
		_defaultImage = newVal;
}
	
String Preferences::last_game_get() const
{
	return _lastGame;
}
	
String Preferences::default_theme_get() const
{
	return _defaultTheme;
}
	
void Preferences::default_theme_set(const String newVal)
{
	if (_defaultTheme != newVal)
		_defaultTheme = newVal;
}
	
String Preferences::default_image_get() const
{
	return _defaultImage;
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
	FUNC_("Preferences::load");
		
	Ref<ConfigFile> cf = memnew(ConfigFile);
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

Color Preferences::numbers_color_get() const
{
	return _numbersColor;
}
	
void Preferences::numbers_color_set(const Color newVal)
{
	if (_numbersColor != newVal)
		_numbersColor = newVal;
}
	
bool Preferences::numbers_visible_get() const
{
	return _numbersVisible;
}
	
void Preferences::numbers_visible_set(const bool newVal)
{
	if (_numbersVisible != newVal)
		_numbersVisible = newVal;
}
	
Color Preferences::outlines_color_get() const
{
	return _outlinesColor;
}
	
void Preferences::outlines_color_set(const Color newVal)
{
	if (_outlinesColor != newVal)
		_outlinesColor = newVal;
}
		
bool Preferences::outlines_visible_get() const
{
	return _outlinesVisible;
}
	
void Preferences::outlines_visible_set(const bool newVal)
{
	if (_outlinesVisible != newVal)
		_outlinesVisible = newVal;
}
	
int Preferences::rows_get() const
{
	return _rows;
}
	
void Preferences::rows_set(const int newVal)
{
	if (_rows != newVal)
		_rows = newVal;
}

bool Preferences::use_image_get() const
{
	return _useImage;
}

void Preferences::use_image_set(const bool newVal)
{
	if (_useImage != newVal)
		_useImage = newVal;
}
	
void Preferences::save(const String path)
{
	FUNC_("Preferences::save");
		
	Ref<ConfigFile> cf = memnew(ConfigFile);
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
