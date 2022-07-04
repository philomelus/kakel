extends Node

# Size of the tile board
var TilesSize: Vector2 = Vector2(4, 4)

# Path of default image
var TilesImageDefault: String = "res://default_image.png"

# Color for tile edges
var TilesColor: Color = Color(0, 0, 1, 1)

# Color for tile fonts
var TilesFontColor: Color = Color(0, 1, 0, 1)

# Image used on tiles
var TilesImage: Image = null

# Default image for tiles (used as TilesImage when not overridden)
var TilesDefaultImage: Image = ResourceLoader.load("res://default_image.png", "Image", false)
