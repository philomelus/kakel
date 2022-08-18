#include "AppPreferences.hpp"
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
    const char* V_VERSION = "version";
    const char* V_AUTOLOAD = "auto_load";
    const char* V_AUTOPATH = "auto_path";
    const char* V_AUTOREMOVEONWIN = "auto_remove_on_win";
    const char* V_AUTOSAVE = "auto_save";
    const char* V_COLUMNS = "columns";
    const char* V_DEFAULTIMAGE = "default_image";
    const char* V_DEFAULTTHEME = "default_theme";
	const char* V_HILITEBLANK = "hilite_blank";
	const char* V_HILITEBLANKCOLOR = "hilite_blank_color";
	const char* V_KEEPASPECT = "keep_aspect";
    const char* V_LASTGAME = "last_game";
    const char* V_LASTIMAGE = "last_image";
    const char* V_NUMBERSCOLOR = "number_color";
    const char* V_NUMBERSVISIBLE = "numbers_visible";
    const char* V_OUTLINESCOLOR = "outline_color";
    const char* V_OUTLINESVISIBLE = "outlines_visible";
    const char* V_ROWS = "rows";
	
    // Paths.
    const char* P_DEFAULTAUTOPATH = "user://auto.kakel";
	const char* P_DEFAULTTHEME = "res://theme.tres";
    const char* P_DEFAULTIMAGE = "res://default_image.png";

	// Defaults
	const bool default_autoLoad = true;
	const char* default_autoPath = P_DEFAULTAUTOPATH;
	const bool default_autoRemoveOnWin = true;
	const bool default_autoSave = true;
	const int default_columns = 4;
	const char* default_defaultImage = P_DEFAULTIMAGE;
	const char* default_defaultTheme = P_DEFAULTTHEME;
	const bool default_hiliteBlank = false;
	const Color default_hiliteBlankColor = Color(0.8, 0.4, 0.4, 1);
	const bool default_keepAspect = false;
	const char* default_lastGame = "";
	const char* default_lastImage = "";
	const Color default_numbersColor = Color(0.8, 0.8, 0.8, 1);
	const bool default_numbersVisible = true;
	const Color default_outlinesColor = Color(0.5, 0.5, 0.5, 1);
	const bool default_outlinesVisible = false;
	const int default_rows = 4;
}

const char* AppPreferences::P_PREFS = "user://prefs.cfg";
	
void AppPreferences::_bind_methods()
{
	FUNC_("AppPreferences::_bind_methods");

	// API
	ClassDB::bind_method(D_METHOD("load"), &AppPreferences::load);
	ClassDB::bind_method(D_METHOD("save"), &AppPreferences::save);

	// Non-api functions
	ClassDB::bind_method(D_METHOD("auto_load_get"), &AppPreferences::auto_load_get);
	ClassDB::bind_method(D_METHOD("auto_load_set", "newVal"), &AppPreferences::auto_load_set);
	ClassDB::bind_method(D_METHOD("auto_path_get"), &AppPreferences::auto_path_get);
	ClassDB::bind_method(D_METHOD("auto_path_set", "newVal"), &AppPreferences::auto_path_set);
	ClassDB::bind_method(D_METHOD("auto_remove_on_win_get"), &AppPreferences::auto_remove_on_win_get);
	ClassDB::bind_method(D_METHOD("auto_remove_on_win_set", "newVal"), &AppPreferences::auto_remove_on_win_set);
	ClassDB::bind_method(D_METHOD("auto_save_get"), &AppPreferences::auto_save_get);
	ClassDB::bind_method(D_METHOD("auto_save_set", "newVal"), &AppPreferences::auto_save_set);
	ClassDB::bind_method(D_METHOD("columns_get"), &AppPreferences::columns_get);
	ClassDB::bind_method(D_METHOD("columns_set", "newVal"), &AppPreferences::columns_set);
	ClassDB::bind_method(D_METHOD("default_image_get"), &AppPreferences::default_image_get);
	ClassDB::bind_method(D_METHOD("default_image_set", "newVal"), &AppPreferences::default_image_set);
	ClassDB::bind_method(D_METHOD("default_theme_get"), &AppPreferences::default_theme_get);
	ClassDB::bind_method(D_METHOD("default_theme_set", "newVal"), &AppPreferences::default_theme_set);
	ClassDB::bind_method(D_METHOD("hilite_blank_get"), &AppPreferences::hilite_blank_get);
	ClassDB::bind_method(D_METHOD("hilite_blank_set", "newVal"), &AppPreferences::hilite_blank_set);
	ClassDB::bind_method(D_METHOD("hilite_blank_color_get"), &AppPreferences::hilite_blank_color_get);
	ClassDB::bind_method(D_METHOD("hilite_blank_color_set", "newVal"), &AppPreferences::hilite_blank_color_set);
	ClassDB::bind_method(D_METHOD("keep_aspect_get"), &AppPreferences::keep_aspect_get);
	ClassDB::bind_method(D_METHOD("keep_aspect_set", "newVal"), &AppPreferences::keep_aspect_set);
	ClassDB::bind_method(D_METHOD("last_game_get"), &AppPreferences::last_game_get);
	ClassDB::bind_method(D_METHOD("last_game_set", "newVal"), &AppPreferences::last_game_set);
	ClassDB::bind_method(D_METHOD("last_image_get"), &AppPreferences::last_image_get);
	ClassDB::bind_method(D_METHOD("last_image_set", "newVal"), &AppPreferences::last_image_set);
	ClassDB::bind_method(D_METHOD("numbers_color_get"), &AppPreferences::numbers_color_get);
	ClassDB::bind_method(D_METHOD("numbers_color_set", "newVal"), &AppPreferences::numbers_color_set);
	ClassDB::bind_method(D_METHOD("numbers_visible_get"), &AppPreferences::numbers_visible_get);
	ClassDB::bind_method(D_METHOD("numbers_visible_set", "newVal"), &AppPreferences::numbers_visible_set);
	ClassDB::bind_method(D_METHOD("outlines_color_get"), &AppPreferences::outlines_color_get);
	ClassDB::bind_method(D_METHOD("outlines_color_set", "newVal"), &AppPreferences::outlines_color_set);
	ClassDB::bind_method(D_METHOD("outlines_visible_get"), &AppPreferences::outlines_visible_get);
	ClassDB::bind_method(D_METHOD("outlines_visible_set", "newVal"), &AppPreferences::outlines_visible_set);
	ClassDB::bind_method(D_METHOD("rows_get"), &AppPreferences::rows_get);
	ClassDB::bind_method(D_METHOD("rows_set", "newVal"), &AppPreferences::rows_set);
	
	// Properties
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_load"), "auto_load_set",	"auto_load_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "auto_path"), "auto_path_set", "auto_path_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_remove_on_win"), "auto_remove_on_win_set", "auto_remove_on_win_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_save"), "auto_save_set", "auto_save_get");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "columns"), "columns_set", "columns_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "default_image"), "default_image_set", "default_image_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "default_theme"), "default_theme_set", "default_theme_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "hilite_blank"), "hilite_blank_set", "hilite_blank_get");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "hilite_blank_color"), "hilite_blank_color_set", "hilite_blank_color_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_aspect"), "keep_aspect_set",	"keep_aspect_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "last_game"), "last_game_set", "last_game_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "last_image"), "last_image_set", "last_image_get");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "numbers_color"), "numbers_color_set", "numbers_color_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "numbers_visible"), "numbers_visible_set", "numbers_visible_get");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "outlines_color"), "outlines_color_set", "outlines_color_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "outlines_visible"), "outlines_visible_set", "outlines_visible_get");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "rows"), "rows_set", "rows_get");
}

AppPreferences::AppPreferences()
{
	FUNC_("AppPreferences::AppPreferences");

	_autoLoad = default_autoLoad;
	_autoPath = default_autoPath;
	_autoRemoveOnWin = default_autoRemoveOnWin;
	_autoSave = default_autoSave;
	_columns = default_columns;
	_defaultTheme = default_defaultTheme;
	_defaultImage = default_defaultImage;
	_hiliteBlank = default_hiliteBlank;
	_hiliteBlankColor = default_hiliteBlankColor;
	_keepAspect = default_keepAspect;
	_lastGame = default_lastGame;
	_lastImage = default_lastImage;
	_numbersColor = default_numbersColor;
	_numbersVisible = default_numbersVisible;
	_outlinesColor = default_outlinesColor;
	_outlinesVisible = default_outlinesVisible;
	_rows = default_rows;
		
	// Load preferences if exists
	Ref<Directory> d = memnew(Directory);
	if (d->file_exists(P_PREFS))
	{
		FUNCPF_("Loading preferences from \"", P_PREFS, "\"");
		load(P_PREFS);
	}
}

AppPreferences::~AppPreferences()
{
	FUNC_("AppPreferences::~AppPreferences");
}

void AppPreferences::_ready()
{
	FUNC_("AppPreferences::_ready");
}
			
bool AppPreferences::auto_load_get() const
{
	return _autoLoad;
}
	
void AppPreferences::auto_load_set(const bool newVal)
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
	
void AppPreferences::auto_remove_on_win_set(const bool newVal)
{
	if (_autoRemoveOnWin != newVal)
		_autoRemoveOnWin = newVal;
}
	
bool AppPreferences::auto_save_get() const
{
	return _autoSave;
}
	
void AppPreferences::auto_save_set(const bool newVal)
{
	if (_autoSave != newVal)
		_autoSave = newVal;
}
	
int AppPreferences::columns_get() const
{
	return _columns;
}
	
void AppPreferences::columns_set(const int newVal)
{
	if (_columns != newVal)
		_columns = newVal;
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

bool AppPreferences::hilite_blank_get() const
{
	return _hiliteBlank;
}

void AppPreferences::hilite_blank_set(const bool newVal)
{
	if (_hiliteBlank != newVal)
		_hiliteBlank = newVal;
}

Color AppPreferences::hilite_blank_color_get() const
{
	return _hiliteBlankColor;
}

void AppPreferences::hilite_blank_color_set(const Color newVal)
{
	if (_hiliteBlankColor != newVal)
		_hiliteBlankColor = newVal;
}

bool AppPreferences::keep_aspect_get() const
{
	return _keepAspect;
}

void AppPreferences::keep_aspect_set(bool newVal)
{
	if (_keepAspect != newVal)
		_keepAspect = newVal;
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
	
void AppPreferences::last_image_set(const String newVal)
{
	if (_lastImage != newVal)
		_lastImage = newVal;
}
	
void AppPreferences::load(const String path)
{
	FUNC_("AppPreferences::load");

	int tmp;
	String stmp;
	
	Ref<ConfigFile> cf = memnew(ConfigFile);
	cf->load(path);
	tmp = (int) cf->get_value(S_GLOBALS, V_VERSION, PREFS_VERSION);
	ERR_FAIL_COND(tmp != PREFS_VERSION);
	tmp = cf->get_value(S_GLOBALS, V_AUTOLOAD, default_autoLoad ? 1 : 0);
	// FUNCP_("_autoLoad = ", tmp);
	_autoLoad = tmp == 1;
	_autoPath = cf->get_value(S_GLOBALS, V_AUTOPATH, default_autoPath);
	// FUNCP_("_autoPath = \"", _autoPath, "\"");
	tmp = cf->get_value(S_GLOBALS, V_AUTOREMOVEONWIN, default_autoRemoveOnWin ? 1 : 0);
	// FUNCP_("_autoRemoveOnWin = ", tmp);
	_autoRemoveOnWin = tmp == 1;
	tmp = cf->get_value(S_GLOBALS, V_AUTOSAVE, default_autoSave ? 1 : 0);
	// FUNCP_("_autoSave = ", tmp);
	_autoSave = tmp == 1;
	_columns = cf->get_value(S_GLOBALS, V_COLUMNS, default_columns);
	// FUNCP_("_columns = ", _columns);
	_defaultTheme = cf->get_value(S_GLOBALS, V_DEFAULTTHEME, default_defaultTheme);
	// FUNCP_("_defaultTheme = \"", _defaultTheme, "\"");
	_defaultImage = cf->get_value(S_GLOBALS, V_DEFAULTIMAGE, default_defaultImage);
	// FUNCP_("_defaultImage = \"", _defaultImage, "\"");
	tmp = cf->get_value(S_GLOBALS, V_HILITEBLANK, default_hiliteBlank ? 1 : 0);
	// FUNCP_("_hiliteBlank = ", tmp);
	_hiliteBlank = tmp == 1;
	stmp = cf->get_value(S_GLOBALS, V_HILITEBLANKCOLOR, default_hiliteBlankColor.to_html(false));
	// FUNCPF_("_hiliteBlankColor = \"", stmp, "\"");
	_hiliteBlankColor = Color::html(stmp.substr(0, 6));
	tmp = cf->get_value(S_GLOBALS, V_KEEPASPECT, default_keepAspect ? 1 : 0);
	// FUNCP_("_keepAspect = ", tmp);
	_keepAspect = tmp == 1;	
	_lastGame = cf->get_value(S_GLOBALS, V_LASTGAME, default_lastGame);
	// FUNCP_("_lastGame = \"", _lastGame, "\"");
	_lastImage = cf->get_value(S_GLOBALS, V_LASTIMAGE, default_lastImage);
	// FUNCP_("_lastImage = \"", _lastImage, "\"");
	stmp = cf->get_value(S_GLOBALS, V_NUMBERSCOLOR, default_numbersColor.to_html(false));
	// FUNCP_("_numbersColor = \"", stmp, "\"");
	_numbersColor = Color::html(stmp.substr(0, 6));
	tmp = cf->get_value(S_GLOBALS, V_NUMBERSVISIBLE, default_numbersVisible ? 1 : 0);
	// FUNCP_("_numbersVisible = ", tmp);
	_numbersVisible = tmp == 1;
	stmp = cf->get_value(S_GLOBALS, V_OUTLINESCOLOR, default_outlinesColor.to_html(false));
	// FUNCPF_("_outlinesColor = \"", stmp, "\"");
	_outlinesColor = Color::html(stmp.substr(0, 6));
	tmp = cf->get_value(S_GLOBALS, V_OUTLINESVISIBLE, default_outlinesVisible ? 1 : 0);
	// FUNCP_("_outlinesVisible = ", tmp);
	_outlinesVisible = tmp == 1;
	_rows = cf->get_value(S_GLOBALS, V_ROWS, default_rows);
	// FUNCP_("_rows = ", _rows);
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

void AppPreferences::save(const String path)
{
	FUNC_("AppPreferences::save");

	int tmp;
	String stmp;
	
	Ref<ConfigFile> cf = memnew(ConfigFile);
	cf->set_value(S_GLOBALS, V_VERSION, PREFS_VERSION);
	tmp = _autoLoad ? 1 : 0;
	// FUNCP_("_autoLoad = ", tmp);
	cf->set_value(S_GLOBALS, V_AUTOLOAD, tmp);
	// FUNCP_("_autoPath = \"", _autoPath, "\"");
	cf->set_value(S_GLOBALS, V_AUTOPATH, _autoPath);
	tmp = _autoRemoveOnWin ? 1 : 0;
	// FUNCP_("_autoRemoveOnWin = ", tmp);
	cf->set_value(S_GLOBALS, V_AUTOREMOVEONWIN, tmp);
	tmp = _autoSave ? 1 : 0;
	// FUNCP_("_autoSave = ", tmp);
	cf->set_value(S_GLOBALS, V_AUTOSAVE, tmp);
	// FUNCP_("_columns = ", _columns);
	cf->set_value(S_GLOBALS, V_COLUMNS, _columns);
	// FUNCP_("_defaultTheme = \"", _defaultTheme, "\"");
	cf->set_value(S_GLOBALS, V_DEFAULTTHEME, _defaultTheme);
	// FUNCP_("_defaultImage = \"", _defaultImage, "\"");
	cf->set_value(S_GLOBALS, V_DEFAULTIMAGE, _defaultImage);
	tmp = _hiliteBlank ? 1 : 0;
	// FUNCP_("_hiliteBlank = ", tmp);
	cf->set_value(S_GLOBALS, V_HILITEBLANK, tmp);
	stmp = _hiliteBlankColor.to_html(true);
	// FUNCP_("_hiliteBlankColor = \"", stmp, "\"");
	cf->set_value(S_GLOBALS, V_HILITEBLANKCOLOR, stmp.substr(0, 6));
	tmp = _keepAspect ? 1 : 0;
	// FUNCP_("_keepAspect = ", tmp);
	cf->set_value(S_GLOBALS, V_KEEPASPECT, tmp);
    // FUNCP_("_lastGame = \"", _lastGame, "\"");
	cf->set_value(S_GLOBALS, V_LASTGAME, _lastGame);
	// FUNCP_("_lastImage = \"", _lastImage, "\"");
	cf->set_value(S_GLOBALS, V_LASTIMAGE, _lastImage);
	stmp = _numbersColor.to_html(true);
	// FUNCP_("_numbersColor = \"", stmp, "\"");
	cf->set_value(S_GLOBALS, V_NUMBERSCOLOR, stmp.substr(0, 6));
	tmp = _numbersVisible ? 1 : 0;
	// FUNCP_("_numbersVisible = ", tmp);
	cf->set_value(S_GLOBALS, V_NUMBERSVISIBLE, tmp);
	stmp = _outlinesColor.to_html(true);
	// FUNCPF_("_outlinesColor = \"", stmp, "\"");
	cf->set_value(S_GLOBALS, V_OUTLINESCOLOR, stmp.substr(0, 6));
	tmp = _outlinesVisible ? 1 : 0;
	// FUNCP_("_outlinesVisible = ", tmp);
	cf->set_value(S_GLOBALS, V_OUTLINESVISIBLE, tmp);
	// FUNCP_("_rows = ", _rows);
	cf->set_value(S_GLOBALS, V_ROWS, _rows);
	cf->save(path);
}
