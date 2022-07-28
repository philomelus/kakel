extends Node

# When true, the game was started from AutoPath because AutoLoad was true
var auto_started: bool = false

# Preferences that are saved/loade
var preferences: Preferences = Preferences.new()

# If true, use default image.
var tiles_default_image: bool = true

# When true, the game in `TilesLoadPath` is loaded and restarted
var tiles_loading: bool = false

# When TilesLoading is true, path to game file to load.
var tiles_load_path: String

# When true, use image for tiles.  Otherwise draw outlines with numbers.
var tiles_use_image: bool = true

