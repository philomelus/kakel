extends Node

# Number of columns in tiles.
var TilesColumns: int = 4

# If true, use default image.
var TilesDefaultImage: bool = true

# Image to use for default image.
var TilesImageDefault: String = "res://default_image.png"

# If `TilesDefaultImage` is false and TilesUseImag is true, this
# contains the path of the image to use.
var TilesImagePath: String

# When true, the main display will load the game in `TilesLoadPath` rather
# than using TilesColumns/TilesRows/TilesImage/TilesDefaultImage, etc.
var TilesLoading: bool = false

# When TilesLoading is true, path to game file to load.
var TilesLoadPath: String

# Color used to draw tile numbers.
var TilesNumberColor: Color = Color.white

# Font used to draw tile numbers.
var TilesNumberFont: Font

# Color to draw tile outlines when TilesUseImage is false.
var TilesOutlineColor: Color = Color.white

# Number of rows in tiles.
var TilesRows: int = 4

# When true, show tile numbers on tiles.
var TilesShowNumbers: bool = true

# When true, use image for tiles.  Otherwise draw outlines with numbers.
var TilesUseImage: bool = true


# Load global preferences from file.
func Load(path: String) -> void:
	var inp: File = File.new()
	inp.open(path, File.Read)


# Save global preferences into file.
func Save(path: String) -> void:
	var outp: File = File.new()
	outp.open(path, File.Write)
