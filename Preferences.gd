class_name Preferences
extends Resource

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
const V_LAST_GAME: String = "last_game"
const V_LAST_IMAGE: String = "last_image"
const V_NUMBER_COLOR: String = "number_color"
const V_OUTLINE_COLOR: String = "outline_color"
const V_ROWS: String = "rows"
const V_SHOW_NUMBERS: String = "show_numbers"
const V_SHOW_OUTLINES: String = "show_outlines"

# Paths.
const P_DEFAULT_AUTO_PATH: String = "user://auto.kakel"
const P_DEFAULT_IMAGE: String = "res://default_image.png"
const P_PREFS: String = "user://prefs.cfg"

# Preferences.
export var auto_load: bool setget auto_load_set, auto_load_get
func auto_load_get() -> bool:
	return auto_load
func auto_load_set(newVal: bool) -> void:
	auto_load = newVal

export var auto_path: String setget auto_path_set, auto_path_get
func auto_path_get() -> String:
	return auto_path
func auto_path_set(newVal: String) -> void:
	auto_path = newVal

export var auto_remove_on_win: bool setget auto_remove_on_win_set, auto_remove_on_win_get
func auto_remove_on_win_get() -> bool:
	return auto_remove_on_win
func auto_remove_on_win_set(newVal: bool) -> void:
	auto_remove_on_win = newVal

export var auto_save: bool setget auto_save_set, auto_save_get
func auto_save_get() -> bool:
	return auto_save
func auto_save_set(newVal: bool) -> void:
	auto_save = newVal

export var columns: int setget columns_set, columns_get
func columns_get() -> int:
	return columns
func columns_set(newVal: int) -> void:
	columns = newVal

export var default_image: String setget default_image_set, default_image_get
func default_image_get() -> String:
	return default_image
func default_image_set(newVal: String) -> void:
	default_image = newVal

export var last_game: String setget last_game_set, last_game_get
func last_game_get() -> String:
	return last_game
func last_game_set(newVal: String) -> void:
	last_game = newVal

export var last_image: String setget last_image_set, last_image_get
func last_image_get() -> String:
	return last_image
func last_image_set(newVal: String) -> void:
	last_image = newVal

export var number_color: Color setget number_color_set, number_color_get
func number_color_get() -> Color:
	return number_color
func number_color_set(newVal: Color) -> void:
	number_color = newVal

export var outline_color: Color setget outline_color_set, outline_color_get
func outline_color_get() -> Color:
	return outline_color
func outline_color_set(newVal: Color) -> void:
	outline_color = newVal

export var rows: int setget rows_set, rows_get
func rows_get() -> int:
	return rows
func rows_set(newVal: int) -> void:
	rows = newVal

export var show_outlines: bool setget show_outlines_set, show_outlines_get
func show_outlines_get() -> bool:
	return show_outlines
func show_outlines_set(newVal: bool) -> void:
	show_outlines = newVal

export var show_numbers: bool setget show_numbers_set, show_numbers_get
func show_numbers_get() -> bool:
	return show_numbers
func show_numbers_set(newVal: bool) -> void:
	show_numbers = newVal

# Preferences default values.
const default_auto_load: bool = true
const default_auto_path: String = P_DEFAULT_AUTO_PATH
const default_auto_remove_on_win: bool = true
const default_auto_save: bool = true
const default_columns: int = 4;
const default_default_image: String = P_DEFAULT_IMAGE
const default_last_game: String = ""
const default_last_image: String = ""
const default_number_color: Color = Color.lightgray
const default_outline_color: Color = Color.gray
const default_rows: int = 4
const default_show_numbers: bool = true
const default_show_outlines: bool = false


func _ready() -> void:
	# Initialize all default values here
	self.auto_load = default_auto_load
	self.auto_path = default_auto_path
	self.auto_remove_on_win = default_auto_remove_on_win
	self.auto_save = default_auto_save
	self.columns = default_columns
	self.default_image = default_default_image
	self.last_game = default_last_game
	self.last_image = default_last_image
	self.number_color = default_number_color
	self.outline_color = default_outline_color
	self.rows = default_rows
	self.show_outlines = default_show_outlines
	self.show_numbers = default_show_numbers


# Load preferences from file
func load_(path: String) -> void:
	var cf: ConfigFile = ConfigFile.new()
	cf.load(path)
	assert(cf.get_value(S_GLOBALS, V_VERSION, PREFS_VERSION) == PREFS_VERSION)
	self.auto_load = cf.get_value(S_GLOBALS, V_AUTO_LOAD, 1 if default_auto_load else 0) == 1
	self.auto_path = cf.get_value(S_GLOBALS, V_AUTO_PATH, default_auto_path)
	self.auto_remove_on_win = cf.get_value(S_GLOBALS, V_AUTO_REMOVE_ON_WIN, 1 if default_auto_remove_on_win else 0) == 1
	self.auto_save = cf.get_value(S_GLOBALS, V_AUTO_SAVE, 1 if default_auto_save else 0) == 1
	self.columns = cf.get_value(S_GLOBALS, V_COLUMNS, default_columns)
	self.default_image = cf.get_value(S_GLOBALS, V_DEFAULT_IMAGE, default_default_image)
	self.last_game = cf.get_value(S_GLOBALS, V_LAST_GAME, default_last_game)
	self.last_image = cf.get_value(S_GLOBALS, V_LAST_IMAGE, default_last_image)
	self.number_color = Color(cf.get_value(S_GLOBALS, V_NUMBER_COLOR, default_number_color.to_html(true)))
	self.outline_color = Color(cf.get_value(S_GLOBALS, V_OUTLINE_COLOR, default_outline_color.to_html(true)))
	self.rows = cf.get_value(S_GLOBALS, V_ROWS, default_rows)
	self.show_outlines = cf.get_value(S_GLOBALS, V_SHOW_OUTLINES, 1 if default_show_outlines else 0) == 1
	self.show_numbers = cf.get_value(S_GLOBALS, V_SHOW_NUMBERS, 1 if default_show_numbers else 0) == 1


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
	cf.set_value(S_GLOBALS, V_LAST_GAME, self.last_game)
	cf.set_value(S_GLOBALS, V_LAST_IMAGE, self.last_image)
	cf.set_value(S_GLOBALS, V_NUMBER_COLOR, self.number_color.to_html(true))
	cf.set_value(S_GLOBALS, V_OUTLINE_COLOR, self.outline_color.to_html(true))
	cf.set_value(S_GLOBALS, V_ROWS, rows)
	cf.set_value(S_GLOBALS, V_SHOW_OUTLINES, 1 if self.show_outlines else 0)
	cf.set_value(S_GLOBALS, V_SHOW_NUMBERS, 1 if self.show_numbers else 0)
	cf.save(path)
