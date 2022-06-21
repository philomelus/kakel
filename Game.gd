extends Node2D

# Spacing between edge of canvas and tiles
const margin = Vector2(10, 10)

# Spacing between each tile
const spacing = Vector2(10, 10)

# Font for drawing numerical id's of tiles
var tile_font = load("res://tile_font.tres")

# All the valid tiles (1 less than total number)
var tiles = []

# For all tiles, their actual location in the screen
var tiles_order = []

# The index within tiles that can be moved in direction
# -1 means there isn't a movement in that direction
var mv_left = -1
var mv_right = -1
var mv_up = -1
var mv_down = -1

# Index within tiles of the empty tile
var empty

func _ready():
	# Calculate based on canvas size
	var width = 100
	var height = 100

	var xext = width + spacing.x
	var yext = height + spacing.y

	var tile
	for row in range(Globals.Rows):
		for col in range(Globals.Columns):
			tile = {}
			tile['area'] = Rect2(Vector2(margin.x + (col * xext), margin.y + (row * yext)), Vector2(width, height))
			tiles.append(tile)

	empty = Globals.Columns * Globals.Rows - 1

func _draw():
	var index = 0
	var id = 1
	var name
	var extent
	var area
	for t in tiles:
		if index != empty:
			name = "%02d" % id
			extent = tile_font.get_string_size(name)
			area = t["area"]
			draw_rect(area, Color(1, 1, 1, 1), false)
			draw_string(tile_font, Vector2(area.position.x + ((area.size.x - extent.x) / 2),
										   area.position.y + ((area.size.y - extent.y) / 2) + extent.y),
						name, Color(1, 1, 1, 1))
			id += 1
		index += 1


