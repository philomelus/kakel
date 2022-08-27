class_name AppPreferences
extends Node

const PREFS_VERSION = 1

# Sections.
const S_GLOBALS: String = "globals"

# Values.
const V_VERSION: String = "version"
const V_AUTO_LOAD: String = "auto_load"
const V_AUTO_PATH: String = "auto_path"
const V_AUTO_REMOVE_ON_WIN: String = "auto_remove_on_win"
const V_AUTO_SAVE: String = "auto_save"
const V_COLUMNS: String = "columns"
const V_DEFAULT_IMAGE: String = "default_image"
const V_DEFAULT_THEME: String = "default_theme"
const V_HILITE_BLANK: String = "hilite_blank"
const V_HILITE_BLANK_COLOR: String = "hilite_blank_color"
const V_KEEP_ASPECT: String = "keep_aspect"
const V_LAST_GAME: String = "last_game"
const V_LAST_IMAGE: String = "last_image"
const V_NUMBERS_COLOR: String = "numbers_color"
const V_NUMBERS_VISIBLE: String = "numbers_visible"
const V_OUTLINES_COLOR: String = "outlines_color"
const V_OUTLINES_VISIBLE: String = "outlines_visible"
const V_ROWS: String = "rows"

# Paths.
const P_DEFAULT_AUTO_PATH: String = "user://auto.kakel"
const P_DEFAULT_IMAGE: String = "res://default_image.png"
const P_DEFAULT_THEME = "res://theme.tres"
const P_PREFS: String = "user://prefs.cfg"

# Preferences.
export var auto_load: bool setget auto_load_set, auto_load_get
func auto_load_get() -> bool:
	return auto_load
func auto_load_set(newVal: bool) -> void:
	if auto_load != newVal:
		auto_load = newVal

export var auto_path: String setget auto_path_set, auto_path_get
func auto_path_get() -> String:
	return auto_path
func auto_path_set(newVal: String) -> void:
	if auto_path != newVal:
		auto_path = newVal

export var auto_remove_on_win: bool setget auto_remove_on_win_set, auto_remove_on_win_get
func auto_remove_on_win_get() -> bool:
	return auto_remove_on_win
func auto_remove_on_win_set(newVal: bool) -> void:
	if auto_remove_on_win != newVal:
		auto_remove_on_win = newVal

export var auto_save: bool setget auto_save_set, auto_save_get
func auto_save_get() -> bool:
	return auto_save
func auto_save_set(newVal: bool) -> void:
	if auto_save != newVal:
		auto_save = newVal

export var columns: int setget columns_set, columns_get
func columns_get() -> int:
	return columns
func columns_set(newVal: int) -> void:
	if columns != newVal:
		columns = newVal

export var default_image: String setget default_image_set, default_image_get
func default_image_get() -> String:
	return default_image
func default_image_set(newVal: String) -> void:
	if default_image != newVal:
		default_image = newVal

export var default_theme: String setget default_theme_set, default_theme_get
func default_theme_get() -> String:
	return default_theme
func default_theme_set(newVal: String) -> void:
	if default_theme != newVal:
		default_theme = newVal

export var hilite_blank: bool setget hilite_blank_set, hilite_blank_get
func hilite_blank_get() -> bool:
	return hilite_blank
func hilite_blank_set(newVal: bool) -> void:
	if hilite_blank != newVal:
		hilite_blank = newVal

export var hilite_blank_color: Color setget hilite_blank_color_set, hilite_blank_color_get
func hilite_blank_color_get() -> Color:
	return hilite_blank_color
func hilite_blank_color_set(newVal: Color) -> void:
	if hilite_blank_color != newVal:
		hilite_blank_color = newVal

export var keep_aspect: bool setget keep_aspect_set, keep_aspect_get
func keep_aspect_get() -> bool:
	return keep_aspect
func keep_aspect_set(newVal: bool) -> void:
	if keep_aspect != newVal:
		keep_aspect = newVal

export var last_game: String setget last_game_set, last_game_get
func last_game_get() -> String:
	return last_game
func last_game_set(newVal: String) -> void:
	if last_game != newVal:
		last_game = newVal

export var last_image: String setget last_image_set, last_image_get
func last_image_get() -> String:
	return last_image
func last_image_set(newVal: String) -> void:
	if last_image != newVal:
		last_image = newVal

export var numbers_color: Color setget numbers_color_set, numbers_color_get
func numbers_color_get() -> Color:
	return numbers_color
func numbers_color_set(newVal: Color) -> void:
	if numbers_color != newVal:
		numbers_color = newVal

export var numbers_visible: bool setget numbers_visible_set, numbers_visible_get
func numbers_visible_get() -> bool:
	return numbers_visible
func numbers_visible_set(newVal: bool) -> void:
	if numbers_visible != newVal:
		numbers_visible = newVal

export var outlines_color: Color setget outlines_color_set, outlines_color_get
func outlines_color_get() -> Color:
	return outlines_color
func outlines_color_set(newVal: Color) -> void:
	if outlines_color != newVal:
		outlines_color = newVal

export var outlines_visible: bool setget outlines_visible_set, outlines_visible_get
func outlines_visible_get() -> bool:
	return outlines_visible
func outlines_visible_set(newVal: bool) -> void:
	if outlines_visible != newVal:
		outlines_visible = newVal

export var rows: int setget rows_set, rows_get
func rows_get() -> int:
	return rows
func rows_set(newVal: int) -> void:
	if rows != newVal:
		rows = newVal


# Preferences default values.
const default_auto_load: bool = true
const default_auto_path: String = P_DEFAULT_AUTO_PATH
const default_auto_remove_on_win: bool = true
const default_auto_save: bool = true
const default_columns: int = 4
const default_default_image: String = P_DEFAULT_IMAGE
const default_default_theme: String = P_DEFAULT_THEME
const default_hilite_blank: bool = false
const default_hilite_blank_color: Color = Color(0.8, 0.4, 0.4, 1)
const default_keep_aspect: bool = false
const default_last_game: String = ""
const default_last_image: String = ""
const default_numbers_color: Color = Color(0.8, 0.8, 0.8, 1)
const default_numbers_visible: bool = true
const default_outlines_color: Color = Color(0.5, 0.5, 0.5, 1)
const default_outlines_visible: bool = false
const default_rows: int = 4
const default_use_image: bool = true


func _ready() -> void:
	# Initialize all default values here
	self.auto_load = default_auto_load
	self.auto_path = default_auto_path
	self.auto_remove_on_win = default_auto_remove_on_win
	self.auto_save = default_auto_save
	self.columns = default_columns
	self.default_image = default_default_image
	self.default_theme = default_default_theme
	self.hilite_blank = default_hilite_blank
	self.hilite_blank_color = default_hilite_blank_color
	self.keep_aspect = default_keep_aspect
	self.last_game = default_last_game
	self.last_image = default_last_image
	self.numbers_color = default_numbers_color
	self.numbers_visible = default_numbers_visible
	self.outlines_color = default_outlines_color
	self.outlines_visible = default_outlines_visible
	self.rows = default_rows

	var d: Directory = Directory.new()
	if d.file_exists(P_PREFS):
		print("Loading preferences from \"%s\"" % [P_PREFS])
		load_(P_PREFS)


# Load preferences from file
func load_(path: String) -> void:
	var cf: ConfigFile = ConfigFile.new()
	var _r = cf.load(path)
	assert(_r == OK)
	assert(cf.get_value(S_GLOBALS, V_VERSION, PREFS_VERSION) == PREFS_VERSION)
	self.auto_load = cf.get_value(S_GLOBALS, V_AUTO_LOAD, 1 if default_auto_load else 0) == 1
	self.auto_path = cf.get_value(S_GLOBALS, V_AUTO_PATH, default_auto_path)
	self.auto_remove_on_win = cf.get_value(S_GLOBALS, V_AUTO_REMOVE_ON_WIN, 1 if default_auto_remove_on_win else 0) == 1
	self.auto_save = cf.get_value(S_GLOBALS, V_AUTO_SAVE, 1 if default_auto_save else 0) == 1
	self.columns = cf.get_value(S_GLOBALS, V_COLUMNS, default_columns)
	self.default_image = cf.get_value(S_GLOBALS, V_DEFAULT_IMAGE, default_default_image)
	self.default_theme = cf.get_value(S_GLOBALS, V_DEFAULT_THEME, default_default_theme)
	self.hilite_blank = cf.get_value(S_GLOBALS, V_HILITE_BLANK, 1 if default_hilite_blank else 0) == 1
	self.hilite_blank_color = Color(cf.get_value(S_GLOBALS, V_HILITE_BLANK_COLOR, default_hilite_blank_color.to_html(true)))
	self.keep_aspect = cf.get_value(S_GLOBALS, V_KEEP_ASPECT, 1 if default_keep_aspect else 0) == 1
	self.last_game = cf.get_value(S_GLOBALS, V_LAST_GAME, default_last_game)
	self.last_image = cf.get_value(S_GLOBALS, V_LAST_IMAGE, default_last_image)
	self.numbers_color = Color(cf.get_value(S_GLOBALS, V_NUMBERS_COLOR, default_numbers_color.to_html(true)))
	self.numbers_visible = cf.get_value(S_GLOBALS, V_NUMBERS_VISIBLE, 1 if default_numbers_visible else 0) == 1
	self.outlines_color = Color(cf.get_value(S_GLOBALS, V_OUTLINES_COLOR, default_outlines_color.to_html(true)))
	self.outlines_visible = cf.get_value(S_GLOBALS, V_OUTLINES_VISIBLE, 1 if default_outlines_visible else 0) == 1
	self.rows = cf.get_value(S_GLOBALS, V_ROWS, default_rows)


# Save preferences to file
func save(path: String) -> void:
	var cf: ConfigFile = ConfigFile.new()
	cf.set_value(S_GLOBALS, V_VERSION, PREFS_VERSION)
	cf.set_value(S_GLOBALS, V_AUTO_LOAD, 1 if self.auto_load else 0)
	cf.set_value(S_GLOBALS, V_AUTO_PATH, self.auto_path)
	cf.set_value(S_GLOBALS, V_AUTO_REMOVE_ON_WIN, 1 if self.auto_remove_on_win else 0)
	cf.set_value(S_GLOBALS, V_AUTO_SAVE, 1 if self.auto_save else 0)
	cf.set_value(S_GLOBALS, V_COLUMNS, columns)
	cf.set_value(S_GLOBALS, V_DEFAULT_IMAGE, self.default_image)
	cf.set_value(S_GLOBALS, V_DEFAULT_THEME, self.default_theme)
	cf.set_value(S_GLOBALS, V_HILITE_BLANK, 1 if self.hilite_blank else 0)
	cf.set_value(S_GLOBALS, V_HILITE_BLANK_COLOR, self.hilite_blank_color.to_html(true))
	cf.set_value(S_GLOBALS, V_KEEP_ASPECT, 1 if self.keep_aspect else 0)
	cf.set_value(S_GLOBALS, V_LAST_GAME, self.last_game)
	cf.set_value(S_GLOBALS, V_LAST_IMAGE, self.last_image)
	cf.set_value(S_GLOBALS, V_NUMBERS_COLOR, self.numbers_color.to_html(true))
	cf.set_value(S_GLOBALS, V_NUMBERS_VISIBLE, 1 if self.numbers_visible else 0)
	cf.set_value(S_GLOBALS, V_OUTLINES_COLOR, self.outlines_color.to_html(true))
	cf.set_value(S_GLOBALS, V_OUTLINES_VISIBLE, 1 if self.outlines_visible else 0)
	cf.set_value(S_GLOBALS, V_ROWS, rows)
	var _r = cf.save(path)
	assert(_r == OK)
