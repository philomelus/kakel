class_name AppGlobals
extends Node

const default_auto_started: bool = true
const default_preferences_imported: bool = false
const default_tiles_default_image: bool = false
const default_tiles_hilite_blank: bool = false
const default_tiles_keep_aspect: bool = false
const default_tiles_loading: bool = false
const default_tiles_load_path: String = ""
const default_tiles_quit: bool = false
const default_tiles_use_image: bool = true

# When true, the game was started from AutoPath because AutoLoad was true
export var auto_started: bool = default_auto_started setget auto_started_set, auto_started_get
func auto_started_get() -> bool:
	return auto_started
func auto_started_set(newVal: bool) -> void:
	if auto_started != newVal:
		auto_started = newVal

# When true, globals have been initialized from preferences as needed.
# This should only happen once at app start.
var preferences_imported: bool = default_preferences_imported setget preferences_imported_set, preferences_imported_get
func preferences_imported_get() -> bool:
	return preferences_imported
func preferences_imported_set(newVal: bool) -> void:
	if preferences_imported != newVal:
		preferences_imported = newVal

# If true, use default image.
var tiles_default_image: bool = default_tiles_default_image setget tiles_default_image_set, tiles_default_image_get
func tiles_default_image_get() -> bool:
	return tiles_default_image
func tiles_default_image_set(newVal: bool) -> void:
	if tiles_default_image != newVal:
		tiles_default_image = newVal

# When true, enhance blank tiles display
var tiles_hilite_blank: bool = default_tiles_hilite_blank setget tiles_hilite_blank_set, tiles_hilite_blank_get
func tiles_hilite_blank_get() -> bool:
	return tiles_hilite_blank
func tiles_hilite_blank_set(newVal: bool) -> void:
	if tiles_hilite_blank != newVal:
		tiles_hilite_blank = newVal

# Initial state of keep aspect when starting game
# (way to communicate from NewGame.tscn to Game.tscn)
var tiles_keep_aspect: bool = default_tiles_keep_aspect setget tiles_keep_aspect_set, tiles_keep_aspect_get
func tiles_keep_aspect_get() -> bool:
	return tiles_keep_aspect
func tiles_keep_aspect_set(newVal: bool) -> void:
	if tiles_keep_aspect != newVal:
		tiles_keep_aspect = newVal

# When true, the game in `TilesLoadPath` is loaded and restarted
var tiles_loading: bool = default_tiles_loading setget tiles_loading_set, tiles_loading_get
func tiles_loading_get() -> bool:
	return tiles_loading
func tiles_loading_set(newVal: bool) -> void:
	if tiles_loading != newVal:
		tiles_loading = newVal

# When TilesLoading is true, path to game file to load.
var tiles_load_path:  String = default_tiles_load_path setget tiles_load_path_set, tiles_load_path_get
func tiles_load_path_get() -> String:
	return tiles_load_path
func tiles_load_path_set(newVal: String) -> void:
	if tiles_load_path != newVal:
		tiles_load_path = newVal

# When true, tiles has started then user quit
# (way to communicate from NewGame.tscn to Game.tscn)
var tiles_quit: bool = default_tiles_quit setget tiles_quit_set, tiles_quit_get
func tiles_quit_get() -> bool:
	return tiles_quit
func tiles_quit_set(newVal: bool) -> void:
	if tiles_quit != newVal:
		tiles_quit = newVal

# Wether to use an image for tiles
# (way to communicate from NewGame.tscn to Game.tscn)
var tiles_use_image: bool = default_tiles_use_image setget tiles_use_image_set, tiles_use_image_get
func tiles_use_image_get() -> bool:
	return tiles_use_image
func tiles_use_image_set(newVal: bool) -> void:
	if tiles_use_image != newVal:
		tiles_use_image = newVal
