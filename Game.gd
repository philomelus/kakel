extends Node2D

# Spacing between edge of canvas and tiles
const margin = Vector2(10, 10)

# Spacing between each tile
const spacing = Vector2(10, 10)

# Font for drawing numerical id's of tiles
var tile_font: DynamicFont = load("res://tile_font.tres")

# All the valid tiles (1 less than total number)
var tiles: Array = []

# For all tiles, their actual location in the screen
var tiles_order: Array = []

# When true, move_left_index is index of left movement tile within tiles
var can_move_left: bool = false
var move_left_index: int = -1

# When true, move_right_index is index of right movement tile within tiles
var can_move_right: bool = false
var move_right_index: int = -1

# When true, move_up_index is index of up movement tile within tiles
var can_move_up: bool = false
var move_up_index: int = -1

# When true, move_down_index is index of down movement tile within tiles
var can_move_down: bool = false
var move_down_index: int = -1

# Index within tiles of the empty tile
var empty: int

func _ready():
	# Determine width and height of tiles from viewport size
	var screen = get_viewport_rect()
	var width = (screen.size.x - (2 * margin.x) - ((Globals.Columns - 1) * spacing.x)) / Globals.Columns
	var height = (screen.size.y - (2 * margin.y) - ((Globals.Rows - 1) * spacing.y)) / Globals.Rows

	# Total size of each tile with spacing between tiles
	var xext = width + spacing.x
	var yext = height + spacing.y

	# Need some random numbers
	var rng = RandomNumberGenerator.new()
	rng.randomize()

	# Pre-calculate the bounding boxes for each tile
	var tile
	for row in range(Globals.Rows):
		for col in range(Globals.Columns):
			tile = {}
			tile['area'] = Rect2(Vector2(margin.x + (col * xext), margin.y + (row * yext)), Vector2(width, height))
			tiles.append(tile)

	# Total number of tiles
	var num_tiles = Globals.Columns * Globals.Rows

	# Initial blank tiles is last tile
	empty = num_tiles - 1
	#empty = rng.randi_range(0, num_tiles - 1)
	#empty = 15

	# Determine initial random tile order
	var order
	for i in range(num_tiles):
		if i == empty:
			assert(not empty in tiles_order)
			tiles_order.append(empty)
		else:
			order = rng.randi_range(0, num_tiles - 1)
			if len(tiles_order) > 0:
				order = tiles_order[0]
				while order in tiles_order or order == empty:
					order = rng.randi_range(0, num_tiles - 1)
			else:
				if order == empty:
					while order == empty:
						order = rng.randi_range(0, num_tiles - 1)
			assert(not order in tiles_order)
			assert(order != empty)
			tiles_order.append(order)

	# Determine valid moves
	calc_movables()


func calc_movables():
	#print(tiles_order)
	#print("Globals.Columns = %d" % Globals.Columns)
	#print("Globals.Rows = %d" % Globals.Rows)
	#print("empty = %d" % empty)

	# These calcs are done more than once (_mo = minus one)
	var cols_mo: int = (Globals.Columns - 1)
	var rows_mo: int = (Globals.Rows - 1)

	# Blank tile column and row
	var row: int = empty / Globals.Columns
	var column: int = empty % Globals.Columns

	#print("row = %d" % row)
	#print("column = %d" % column)

	# Determine if empty is on any edges
	#
	#				L R U D		Condition
	# ============  =======     =====================================
	# Left Edge:    F T - -		column == 0
	# Right edge:   F T - -		column == (Globals.Columns - 1)
	# Top Edge:     - - F T		row == 0
	# Bottom Edge:  - - T F		row == (Globlas.Rows - 1)
	# Inside H:     T T - -		column > 0 and column < (Globlas.Columns - 1)
	# Inside V:		- - T T		row > 0 and row < (Globals.Rows - 1)

	# Determine tiles that can move horizontally
	var left_side: bool = (column == 0)
	var right_side: bool = (column == cols_mo)
	var inside_h: bool = (column > 0 and column < cols_mo)

	assert((left_side and !right_side and !inside_h)
		   or (right_side and !left_side and !inside_h)
		   or (inside_h and !left_side and !right_side))

	#print("left_side = %s" % left_side)
	#print("right_side = %s" % right_side)
	#print("inside_h = %s" % inside_h)

	var can_move_left_index = ((row * Globals.Columns) + column + 1)
	var can_move_right_index = ((row * Globals.Columns) + column - 1)

	if left_side:
		can_move_left = false
		can_move_right = true
	elif right_side:
		can_move_left = false
		can_move_right = true
	else:
		assert(inside_h)
		can_move_left = true
		can_move_right = true

	if can_move_left:
		move_left_index = tiles_order[can_move_left_index]
	if can_move_right:
		move_right_index = tiles_order[can_move_right_index]

	#print("can_move_left = %s / %d" % [can_move_left, move_left_index])
	#print("can_move_right = %s / %d" % [can_move_right, move_right_index])

	# Determine tiles that can move vertically
	var top_side: bool = (row == 0)
	var bottom_side: bool = (row == rows_mo)
	var inside_v: bool = (row > 0 and row < rows_mo)

	assert((top_side and !bottom_side and !inside_v)
		   or (bottom_side and !top_side and !inside_v)
		   or (inside_v and !top_side and !bottom_side))

	#print("top_side = %s" % top_side)
	#print("bottom_side = %s" % bottom_side)
	#print("inside_v = %s" % inside_v)

	var can_move_up_index = (((row + 1) * Globals.Columns) + column)
	var can_move_down_index = (((row - 1) * Globals.Columns) + column)

	#print("can_move_up_index = %d" % can_move_up_index)
	#print("can_move_down_index = %d" % can_move_down_index)

	if top_side:
		can_move_up = true
		can_move_down = false
	elif bottom_side:
		can_move_up = false
		can_move_down = true
	else:
		assert(inside_v)
		can_move_up = true
		can_move_down = true

	if can_move_up:
		move_up_index = tiles_order[can_move_up_index]
	if can_move_down:
		move_down_index = tiles_order[can_move_down_index]

	#print("can_move_up = %s / %d" % [can_move_up, move_up_index])
	#print("can_move_down = %s / %d" % [can_move_down, move_down_index])


func _draw():
	var index = 0
	var extent
	var area
	var name
	var adj
	for tile_index in tiles_order:
		area = tiles[index]['area']
		if tile_index > empty:
			adj = 0
		else:
			adj = 1
		if tile_index == empty:
			draw_line(area.position, area.end, Color(1,0,0,1))
		else:
			name = "%d" % int(tile_index + adj)
			extent = tile_font.get_string_size(name)
			draw_rect(area, Globals.TilesColor, false)
			draw_string(tile_font, Vector2(area.position.x + ((area.size.x - extent.x) / 2),
										   area.position.y + ((area.size.y - extent.y) / 2) + extent.y),
						name, Globals.TilesFontColor)
		area = null
		index += 1

func _unhandled_input(event):
	if event.is_action_pressed("refresh"):
		update()
	if event.is_action_pressed("quit"):
		var _unused = get_tree().change_scene("res://Main.tscn")
	if event.is_action_pressed("move_left"):
		if can_move_left:
			pass
	if event.is_action_pressed("move_right"):
		if can_move_right:
			pass
	if event.is_action_pressed("move_up"):
		if can_move_up:
			pass
	if event.is_action_pressed("move_down"):
		if can_move_down:
			pass


