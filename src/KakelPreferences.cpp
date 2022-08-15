#include "KakelPreferences.hpp"
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
	static const char* V_KEEPASPECT = "keep_aspect";
    static const char* V_LASTGAME = "last_game";
    static const char* V_LASTIMAGE = "last_image";
    static const char* V_NUMBERSCOLOR = "number_color";
    static const char* V_NUMBERSVISIBLE = "numbers_visible";
    static const char* V_OUTLINESCOLOR = "outline_color";
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
	static const bool default_keepAspect = false;
	static const char* default_lastGame = "";
	static const char* default_lastImage = "";
	static const Color default_numbersColor = Color(0.8, 0.8, 0.8, 1);
	static const bool default_numbersVisible = true;
	static const Color default_outlinesColor = Color(0.5, 0.5, 0.5, 1);
	static const bool default_outlinesVisible = false;
	static const int default_rows = 4;
	static const bool default_useImage = true;
}

const char* KakelPreferences::P_PREFS = "user://prefs.cfg";
	
void KakelPreferences::_bind_methods()
{
	FUNC_("KakelPreferences::_bind_methods");

	// API
	ClassDB::bind_method(D_METHOD("load"), &KakelPreferences::load);
	ClassDB::bind_method(D_METHOD("save"), &KakelPreferences::save);

	// Non-api functions
	ClassDB::bind_method(D_METHOD("auto_load_get"), &KakelPreferences::auto_load_get);
	ClassDB::bind_method(D_METHOD("auto_load_set", "newVal"), &KakelPreferences::auto_load_set);
	ClassDB::bind_method(D_METHOD("auto_path_get"), &KakelPreferences::auto_path_get);
	ClassDB::bind_method(D_METHOD("auto_path_set", "newVal"), &KakelPreferences::auto_path_set);
	ClassDB::bind_method(D_METHOD("auto_remove_on_win_get"), &KakelPreferences::auto_remove_on_win_get);
	ClassDB::bind_method(D_METHOD("auto_remove_on_win_set", "newVal"), &KakelPreferences::auto_remove_on_win_set);
	ClassDB::bind_method(D_METHOD("auto_save_get"), &KakelPreferences::auto_save_get);
	ClassDB::bind_method(D_METHOD("auto_save_set", "newVal"), &KakelPreferences::auto_save_set);
	ClassDB::bind_method(D_METHOD("columns_get"), &KakelPreferences::columns_get);
	ClassDB::bind_method(D_METHOD("columns_set", "newVal"), &KakelPreferences::columns_set);
	ClassDB::bind_method(D_METHOD("default_image_get"), &KakelPreferences::default_image_get);
	ClassDB::bind_method(D_METHOD("default_image_set", "newVal"), &KakelPreferences::default_image_set);
	ClassDB::bind_method(D_METHOD("default_theme_get"), &KakelPreferences::default_theme_get);
	ClassDB::bind_method(D_METHOD("default_theme_set", "newVal"), &KakelPreferences::default_theme_set);
	ClassDB::bind_method(D_METHOD("keep_aspect_get"), &KakelPreferences::keep_aspect_get);
	ClassDB::bind_method(D_METHOD("keep_aspect_set", "newVal"), &KakelPreferences::keep_aspect_set);
	ClassDB::bind_method(D_METHOD("last_game_get"), &KakelPreferences::last_game_get);
	ClassDB::bind_method(D_METHOD("last_game_set", "newVal"), &KakelPreferences::last_game_set);
	ClassDB::bind_method(D_METHOD("last_image_get"), &KakelPreferences::last_image_get);
	ClassDB::bind_method(D_METHOD("last_image_set", "newVal"), &KakelPreferences::last_image_set);
	ClassDB::bind_method(D_METHOD("numbers_color_get"), &KakelPreferences::numbers_color_get);
	ClassDB::bind_method(D_METHOD("numbers_color_set", "newVal"), &KakelPreferences::numbers_color_set);
	ClassDB::bind_method(D_METHOD("numbers_visible_get"), &KakelPreferences::numbers_visible_get);
	ClassDB::bind_method(D_METHOD("numbers_visible_set", "newVal"), &KakelPreferences::numbers_visible_set);
	ClassDB::bind_method(D_METHOD("outlines_color_get"), &KakelPreferences::outlines_color_get);
	ClassDB::bind_method(D_METHOD("outlines_color_set", "newVal"), &KakelPreferences::outlines_color_set);
	ClassDB::bind_method(D_METHOD("outlines_visible_get"), &KakelPreferences::outlines_visible_get);
	ClassDB::bind_method(D_METHOD("outlines_visible_set", "newVal"), &KakelPreferences::outlines_visible_set);
	ClassDB::bind_method(D_METHOD("rows_get"), &KakelPreferences::rows_get);
	ClassDB::bind_method(D_METHOD("rows_set", "newVal"), &KakelPreferences::rows_set);
	ClassDB::bind_method(D_METHOD("use_image_get"), &KakelPreferences::use_image_get);
	ClassDB::bind_method(D_METHOD("use_image_set", "newVal"), &KakelPreferences::use_image_set);
	
	// Properties
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_load"), "auto_load_set",	"auto_load_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "auto_path"), "auto_path_set", "auto_path_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_remove_on_win"), "auto_remove_on_win_set", "auto_remove_on_win_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_save"), "auto_save_set", "auto_save_get");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "columns"), "columns_set", "columns_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "default_image"), "default_image_set", "default_image_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "default_theme"), "default_theme_set", "default_theme_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_aspect"), "keep_aspect_set",	"keep_aspect_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "last_game"), "last_game_set", "last_game_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "last_image"), "last_image_set", "last_image_get");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "numbers_color"), "numbers_color_set", "numbers_color_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "numbers_visible"), "numbers_visible_set", "numbers_visible_get");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "outlines_color"), "outlines_color_set", "outlines_color_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "outlines_visible"), "outlines_visible_set", "outlines_visible_get");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "rows"), "rows_set", "rows_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_image"), "use_image_set", "use_image_get");
}

KakelPreferences::KakelPreferences()
{
	FUNC_("KakelPreferences::KakelPreferences");

	_autoLoad = default_autoLoad;
	_autoPath = default_autoPath;
	_autoRemoveOnWin = default_autoRemoveOnWin;
	_autoSave = default_autoSave;
	_columns = default_columns;
	_defaultTheme = default_defaultTheme;
	_defaultImage = default_defaultImage;
	_keepAspect = default_keepAspect;
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
		FUNCPF_("KakelPreferences::Preferences: Loading preferences from \"", P_PREFS, "\"");
		load(P_PREFS);
	}
}

KakelPreferences::~KakelPreferences()
{
	FUNC_("KakelPreferences::~KakelPreferences");
}

void KakelPreferences::_ready()
{
	FUNC_("KakelPreferences::_ready");
}
			
bool KakelPreferences::auto_load_get() const
{
	return _autoLoad;
}
	
void KakelPreferences::auto_load_set(bool newVal)
{
	if (_autoLoad != newVal)
		_autoLoad = newVal;
}
	
String KakelPreferences::auto_path_get() const
{
	return _autoPath;
}
	
void KakelPreferences::auto_path_set(const String newVal)
{
	if (_autoPath != newVal)
		_autoPath = newVal;
}
	
bool KakelPreferences::auto_remove_on_win_get() const
{
	return _autoRemoveOnWin;
}
	
void KakelPreferences::auto_remove_on_win_set(bool newVal)
{
	if (_autoRemoveOnWin != newVal)
		_autoRemoveOnWin = newVal;
}
	
bool KakelPreferences::auto_save_get() const
{
	return _autoSave;
}
	
void KakelPreferences::auto_save_set(bool newVal)
{
	if (_autoSave != newVal)
		_autoSave = newVal;
}
	
int KakelPreferences::columns_get() const
{
	return _columns;
}
	
void KakelPreferences::columns_set(int newVal)
{
	if (_columns != newVal)
		_columns = newVal;
}
	
void KakelPreferences::default_image_set(const String newVal)
{
	if (_defaultImage != newVal)
		_defaultImage = newVal;
}
	
String KakelPreferences::last_game_get() const
{
	return _lastGame;
}
	
String KakelPreferences::default_theme_get() const
{
	return _defaultTheme;
}
	
void KakelPreferences::default_theme_set(const String newVal)
{
	if (_defaultTheme != newVal)
		_defaultTheme = newVal;
}
	
String KakelPreferences::default_image_get() const
{
	return _defaultImage;
}

bool KakelPreferences::keep_aspect_get() const
{
	return _keepAspect;
}

void KakelPreferences::keep_aspect_set(bool newVal)
{
	if (_keepAspect != newVal)
		_keepAspect = newVal;
}

void KakelPreferences::last_game_set(const String newVal)
{
	if (_lastGame != newVal)
		_lastGame = newVal;
}
	
String KakelPreferences::last_image_get() const
{
	return _lastImage;
}
	
void KakelPreferences::last_image_set(String newVal)
{
	if (_lastImage != newVal)
		_lastImage = newVal;
}
	
void KakelPreferences::load(const String path)
{
	FUNC_("KakelPreferences::load");
		
	Ref<ConfigFile> cf = memnew(ConfigFile);
	cf->load(path);
	int tmp = (int) cf->get_value(S_GLOBALS, V_VERSION, PREFS_VERSION);
	ERR_FAIL_COND(tmp != PREFS_VERSION);
	tmp = cf->get_value(S_GLOBALS, V_AUTOLOAD, default_autoLoad ? 1 : 0);
	// FUNCP_("KakelPreferences::load: _autoLoad = ", tmp);
	_autoLoad = tmp == 1;
	_autoPath = cf->get_value(S_GLOBALS, V_AUTOPATH, default_autoPath);
	// FUNCP_("KakelPreferences::load: _autoPath = \"", _autoPath, "\"");
	tmp = cf->get_value(S_GLOBALS, V_AUTOREMOVEONWIN, default_autoRemoveOnWin ? 1 : 0);
	// FUNCP_("KakelPreferences::load: _autoRemoveOnWin = ", tmp);
	_autoRemoveOnWin = tmp == 1;
	tmp = cf->get_value(S_GLOBALS, V_AUTOSAVE, default_autoSave ? 1 : 0);
	// FUNCP_("KakelPreferences::load: _autoSave = ", tmp);
	_autoSave = tmp == 1;
	_columns = cf->get_value(S_GLOBALS, V_COLUMNS, default_columns);
	// FUNCP_("KakelPreferences::load: _columns = ", _columns);
	_defaultTheme = cf->get_value(S_GLOBALS, V_DEFAULTTHEME, default_defaultTheme);
	// FUNCP_("KakelPreferences::load: _defaultTheme = \"", _defaultTheme, "\"");
	_defaultImage = cf->get_value(S_GLOBALS, V_DEFAULTIMAGE, default_defaultImage);
	// FUNCP_("KakelPreferences::load: _defaultImage = \"", _defaultImage, "\"");
	tmp = cf->get_value(S_GLOBALS, V_KEEPASPECT, default_keepAspect ? 1 : 0);
	// FUNCP_("KakelPreferences::load: _keepAspect = ", tmp);
	_lastGame = cf->get_value(S_GLOBALS, V_LASTGAME, default_lastGame);
	// FUNCP_("KakelPreferences::load: _lastGame = \"", _lastGame, "\"");
	_lastImage = cf->get_value(S_GLOBALS, V_LASTIMAGE, default_lastImage);
	// FUNCP_("KakelPreferences::load: _lastImage = \"", _lastImage, "\"");
	String stmp = cf->get_value(S_GLOBALS, V_NUMBERSCOLOR, default_numbersColor.to_html(true));
	// FUNCP_("KakelPreferences::load: _numbersColor = \"", stmp, "\"");
	_numbersColor = Color(stmp);
	tmp = cf->get_value(S_GLOBALS, V_NUMBERSVISIBLE, default_numbersVisible ? 1 : 0);
	// FUNCP_("KakelPreferences::load: _numbersVisible = ", tmp);
	_numbersVisible = tmp == 1;
	stmp = cf->get_value(S_GLOBALS, V_OUTLINESCOLOR, default_outlinesColor.to_html(true));
	// FUNCP_("KakelPreferences::load: _outlinesColor = \"", stmp, "\"");
	_outlinesColor = Color(stmp);
	tmp = cf->get_value(S_GLOBALS, V_OUTLINESVISIBLE, default_outlinesVisible ? 1 : 0);
	// FUNCP_("KakelPreferences::load: _outlinesVisible = ", tmp);
	_outlinesVisible = tmp == 1;
	_rows = cf->get_value(S_GLOBALS, V_ROWS, default_rows);
	// FUNCP_("KakelPreferences::load: _rows = ", _rows);
}

Color KakelPreferences::numbers_color_get() const
{
	return _numbersColor;
}
	
void KakelPreferences::numbers_color_set(const Color newVal)
{
	if (_numbersColor != newVal)
		_numbersColor = newVal;
}
	
bool KakelPreferences::numbers_visible_get() const
{
	return _numbersVisible;
}
	
void KakelPreferences::numbers_visible_set(const bool newVal)
{
	if (_numbersVisible != newVal)
		_numbersVisible = newVal;
}
	
Color KakelPreferences::outlines_color_get() const
{
	return _outlinesColor;
}
	
void KakelPreferences::outlines_color_set(const Color newVal)
{
	if (_outlinesColor != newVal)
		_outlinesColor = newVal;
}
		
bool KakelPreferences::outlines_visible_get() const
{
	return _outlinesVisible;
}
	
void KakelPreferences::outlines_visible_set(const bool newVal)
{
	if (_outlinesVisible != newVal)
		_outlinesVisible = newVal;
}
	
int KakelPreferences::rows_get() const
{
	return _rows;
}
	
void KakelPreferences::rows_set(const int newVal)
{
	if (_rows != newVal)
		_rows = newVal;
}

bool KakelPreferences::use_image_get() const
{
	return _useImage;
}

void KakelPreferences::use_image_set(const bool newVal)
{
	if (_useImage != newVal)
		_useImage = newVal;
}
	
void KakelPreferences::save(const String path)
{
	FUNC_("KakelPreferences::save");
		
	Ref<ConfigFile> cf = memnew(ConfigFile);
	cf->set_value(S_GLOBALS, V_VERSION, PREFS_VERSION);
	// FUNCP_("KakelPreferences::save: _autoLoad = ", _autoLoad ? 1 : 0);
	cf->set_value(S_GLOBALS, V_AUTOLOAD, _autoLoad ? 1 : 0);
	// FUNCP_("KakelPreferences::save: _autoPath = \"", _autoPath, "\"");
	cf->set_value(S_GLOBALS, V_AUTOPATH, _autoPath);
	// FUNCP_("KakelPreferences::save: _autoRemoveOnWin = ", _autoRemoveOnWin ? 1 : 0);
	cf->set_value(S_GLOBALS, V_AUTOREMOVEONWIN, _autoRemoveOnWin ? 1 : 0);
	// FUNCP_("KakelPreferences::save: _autoSave = ", _autoSave ? 1 : 0);
	cf->set_value(S_GLOBALS, V_AUTOSAVE, _autoSave ? 1 : 0);
	// FUNCP_("KakelPreferences::save: _columns = ", _columns);
	cf->set_value(S_GLOBALS, V_COLUMNS, _columns);
	// FUNCP_("KakelPreferences::save: _defaultTheme = \"", _defaultTheme, "\"");
	cf->set_value(S_GLOBALS, V_DEFAULTTHEME, _defaultTheme);
	// FUNCP_("KakelPreferences::save: _defaultImage = \"", _defaultImage, "\"");
	cf->set_value(S_GLOBALS, V_DEFAULTIMAGE, _defaultImage);
	// FUNCP_("KakelPreferences::save: _keepAspect = ", _keepAspect ? 1 : 0);
	cf->set_value(S_GLOBALS, V_KEEPASPECT, _keepAspect ? 1 : 0);
    // FUNCP_("KakelPreferences::save: _lastGame = \"", _lastGame, "\"");
	cf->set_value(S_GLOBALS, V_LASTGAME, _lastGame);
	// FUNCP_("KakelPreferences::save: _lastImage = \"", _lastImage, "\"");
	cf->set_value(S_GLOBALS, V_LASTIMAGE, _lastImage);
	String stmp = _numbersColor.to_html(true);
	// FUNCP_("KakelPreferences::save: _numbersColor = \"", stmp, "\"");
	cf->set_value(S_GLOBALS, V_NUMBERSCOLOR, stmp);
	// FUNCP_("KakelPreferences::save: _numbersVisible = ", _numbersVisible ? 1 : 0);
	cf->set_value(S_GLOBALS, V_NUMBERSVISIBLE, _numbersVisible ? 1 : 0);
	stmp = _outlinesColor.to_html(true);
	// FUNCP_("KakelPreferences::save: _outlinesColor = \"", stmp, "\"");
	cf->set_value(S_GLOBALS, V_OUTLINESCOLOR, stmp);
	// FUNCP_("KakelPreferences::save: _outlinesVisible = ", _outlinesVisible ? 1 : 0);
	cf->set_value(S_GLOBALS, V_OUTLINESVISIBLE, _outlinesVisible ? 1 : 0);
	// FUNCP_("KakelPreferences::save: _rows = ", _rows);
	cf->set_value(S_GLOBALS, V_ROWS, _rows);
	cf->save(path);
}
