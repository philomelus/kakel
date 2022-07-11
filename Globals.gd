extends Node

# Color for tile edges
var TilesColor: Color = Color(0, 0, 1, 1)

# Color for tile fonts
var TilesFontColor: Color = Color(0, 1, 0, 1)

# Image used on tiles
var TilesImage: Image = null
var TilesImagePath: String = ""

# Default image for tiles (used as TilesImage when not overridden)
var TilesImageDefault: Image = ResourceLoader.load("res://default_image.png", "Image", false)

# Size of the tile board
var TilesSize: Vector2 = Vector2(4, 4)

# When false, draw rect for tiles instead of image
var TilesUseImage: bool = true

# When true, game is loading rather than starting new so
# TileControl should skip initialization as load will do
# it after construction.
var TilesLoading: bool = false
var TilesLoadPath: String = ""
