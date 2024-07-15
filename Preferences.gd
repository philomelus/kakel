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
@export var auto_load: bool:
	get:
		return auto_load
	set(newVal):
		auto_load = newVal

@export var auto_path: String:
	get:
		return auto_path
	set(newVal):
		auto_path = newVal

@export var auto_remove_on_win: bool:
	get:
		return auto_remove_on_win
	set(newVal):
		auto_remove_on_win = newVal

@export var auto_save: bool:
	get:
		return auto_save
	set(newVal):
		auto_save = newVal

@export var columns: int:
	get:
		return columns
	set(newVal):
		columns = newVal

@export var default_image: String:
	get:
		return default_image
	set(newVal):
		default_image = newVal

@export var last_game: String:
	get:
		return last_game
	set(newVal):
		last_game = newVal

@export var last_image: String:
	get:
		return last_image
	set(newVal):
		last_image = newVal

@export var number_color: Color:
	get:
		return number_color
	set(newVal):
		number_color = newVal

@export var outline_color: Color:
	get:
		return outline_color
	set(newVal):
		outline_color = newVal

@export var rows: int:
	get:
		return rows
	set(newVal):
		rows = newVal

@export var show_outlines: bool:
	get:
		return show_outlines
	set(newVal):
		show_outlines = newVal

@export var show_numbers: bool:
	get:
		return show_numbers
	set(newVal):
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
const default_number_color: Color = Color.LIGHT_GRAY
const default_outline_color: Color = Color.GRAY
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
