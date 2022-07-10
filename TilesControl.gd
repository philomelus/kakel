extends Control

# When false, no movement is allowed
var moves_enabled = true

# Precalc these as they are used several times
var columns: int
var columns0: int
var rows: int
var rows0: int

# Total number of tiles
var num_tiles: int
var num_tiles0: int

# Spacing between edge of canvas and tiles
export(Vector2) var margin = Vector2(10, 10)

# Spacing between each tile
export(Vector2) var tile_spacing = Vector2(5, 5)

# Font for drawing numerical id's of tiles
var tile_font: DynamicFont = load("res://tile_font.tres")

# If true, default image was used
var tiles_image_default = true

# Image/Texture for display
var tiles_image: Image
var tiles_texture: ImageTexture

# Size of each tile in pixels
var tile_size: Vector2

# Data for each tile.  Each tile is a two item Rect array
var tiles: Array
const IDX_DEST: int = 0
const IDX_SRC: int = 1

# Actual positions of each tile (i.e. index 0 = tile at upper left)
var tiles_order: Array

# When true, move_*_index holds index of tile than can move there
var can_move_down: bool = false
var can_move_left: bool = false
var can_move_right: bool = false
var can_move_up: bool = false

# Index within tiles_order that moves in direction
var move_down_index: int = -1
var move_left_index: int = -1
var move_right_index: int = -1
var move_up_index: int = -1

# Number of movements made since initial display
var moves: int = 0

# This is signalled every time a tile is moved
enum DIRECTION { NORTH, EAST, SOUTH, WEST}
signal moved(direction, count)

# This is signalled when the tiles are all in correct order
signal won()

# Index within tiles of the empty tile
var empty: int

# Index value of the empty tile (always the last global tile index)
var empty_id: int

#var _log: File = null


func _draw():
	var index = 0
	var extent
	var area
	var name
	var tile
	for tile_index in tiles_order:
		tile = tiles[index]
		area = tile[IDX_DEST]
		if tile_index != empty_id:
			if Globals.TilesUseImage:
				draw_texture_rect_region(tiles_texture, tiles[index][IDX_DEST],
										 tiles[tiles_order[index]][IDX_SRC])
			else:
				name = "%d" % (tile_index + 1)
				extent = tile_font.get_string_size(name)
				draw_rect(area, Globals.TilesColor, false)
				draw_string(tile_font, Vector2(area.position.x + ((area.size.x - extent.x) / 2),
											   area.position.y + ((area.size.y - extent.y) / 2) + extent.y),
							name, Globals.TilesFontColor)
		index += 1


func _physics_process(_delta):
	check_complete()

func _ready():
#	_log = File.new()
#	var _unused = _log.open("user://TileControl.log", File.WRITE_READ)

	# Precalc these as they are used several times
	columns = int(Globals.TilesSize.x)
	columns0 = columns - 1
	rows = int(Globals.TilesSize.y)
	rows0 = rows - 1

	# Total number of tiles
	num_tiles = columns * rows
	num_tiles0 = num_tiles - 1

	# Calculdate image and tile data
	#recalc_tiles()

	# Initial blank tiles is last tile
	empty = num_tiles0
	empty_id = empty

	# Need some random numbers
	var rng = RandomNumberGenerator.new()
	rng.randomize()

	# Determine tile order
	tiles_order = []
	var order
	for i in range(num_tiles):
		if i == empty:
			assert(not empty in tiles_order)
			tiles_order.append(empty_id)
		else:
			order = rng.randi_range(0, num_tiles0)
			if len(tiles_order) > 0:
				order = tiles_order[0]
				while order in tiles_order or order == empty:
					order = rng.randi_range(0, num_tiles0)
			else:
				if order == empty:
					while order == empty:
						order = rng.randi_range(0, num_tiles0)
			assert(not order in tiles_order)
			assert(order != empty)
			tiles_order.append(order)

	# Determine valid moves
	calc_movables()


func _unhandled_input(event):
	# Do nothing when paused
	if get_tree().paused or not moves_enabled:
		return

	# If event isn't an action, ignore
	if not event.is_action_type():
		return

	# Take appropriate action
	if event.is_action_pressed("refresh"):
		accept_event()
		update()
		return
	if event.is_action_pressed("quit"):
		accept_event()
		var _unused = get_tree().change_scene("res://Main.tscn")
		return
	if event.is_action_pressed("move_left"):
		if can_move_left:
			accept_event()
			move_left()
			return
	if event.is_action_pressed("move_right"):
		if can_move_right:
			accept_event()
			move_right()
			return
	if event.is_action_pressed("move_up"):
		if can_move_up:
			accept_event()
			move_up()
			return
	if event.is_action_pressed("move_down"):
		if can_move_down:
			accept_event()
			move_down()
			return


func _input(event):
	# Do nothing while paused
	if get_tree().paused or not moves_enabled:
		return

	# Only handle mouse clicks here
	if not event is InputEventMouseButton:
		return
	# warning-ignore:unsafe_cast
	var ev = event as InputEventMouseButton

	# Only handle left mouse clicks
	if ev.button_index != 1:
		return

	# Don't pay attention to double clicks
	if ev.doubleclick:
		return

	# Only repond to clicks when released
	if ev.pressed:
		return

	# Adjust position of click for our position
	var p = event.position - rect_position

#	_log.store_string("click: %s --> %s\n" % [event.position, p])

	# If click was valid, take appropriate action
	if can_move_down:
		if tiles[move_down_index][IDX_DEST].has_point(p):
			accept_event()
			move_down();
			return
	if can_move_left:
		if tiles[move_left_index][IDX_DEST].has_point(p):
			accept_event()
			move_left();
			return
	if can_move_right:
		if tiles[move_right_index][IDX_DEST].has_point(p):
			accept_event()
			move_right();
			return
	if can_move_up:
		if tiles[move_up_index][IDX_DEST].has_point(p):
			accept_event()
			move_up();
			return


func calc_movables():
	# Row and column of blank tile
	# warning-ignore:integer_division
	var row: int = int(empty / columns)
	var column: int = empty % columns

	# Determine tiles that can move horizontally
	var left_side: bool = (column == 0)
	var right_side: bool = (column == columns0)
	var inside_h: bool = (column > 0 and column < columns0)
	if left_side:
		can_move_left = true
		can_move_right = false
	elif right_side:
		can_move_left = false
		can_move_right = true
	else:
		assert(inside_h)
		can_move_left = true
		can_move_right = true
	if can_move_left:
		move_left_index = ((row * columns) + column + 1)
	if can_move_right:
		move_right_index = ((row * columns) + column - 1)

	# Determine tiles that can move vertically
	var top_side: bool = (row == 0)
	var bottom_side: bool = (row == rows0)
	var inside_v: bool = (row > 0 and row < rows0)
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
		move_up_index = (((row + 1) * columns) + column)
	if can_move_down:
		move_down_index = (((row - 1) * columns) + column)


func check_complete():
 	var index = 0
	for tile in tiles:
		if tiles_order[index] != index:
			return false
		index += 1
	moves_enabled = false
	emit_signal("won")
	return true


func move_down():
	var newEmpty = tiles_order[move_down_index]
	tiles_order[move_down_index] = empty_id
	tiles_order[empty] = newEmpty
	empty = move_down_index
	moves += 1
	emit_signal("moved", DIRECTION.SOUTH, moves)
	calc_movables()
	update()


func move_left():
	var newEmpty = tiles_order[move_left_index]
	tiles_order[move_left_index] = empty_id
	tiles_order[empty] = newEmpty
	empty = move_left_index
	moves += 1
	emit_signal("moved", DIRECTION.WEST, moves)
	calc_movables()
	update()


func move_right():
	var newEmpty = tiles_order[move_right_index]
	tiles_order[move_right_index] = empty_id
	tiles_order[empty] = newEmpty
	empty = move_right_index
	moves += 1
	emit_signal("moved", DIRECTION.EAST, moves)
	calc_movables()
	update()


func move_up():
	var newEmpty = tiles_order[move_up_index]
	tiles_order[move_up_index] = empty_id
	tiles_order[empty] = newEmpty
	empty = move_up_index
	moves += 1
	emit_signal("moved", DIRECTION.NORTH, moves)
	calc_movables()
	update()


func recalc_tiles():
	# Determine width and height of tiles from our size
	tile_size = Vector2((rect_size.x - (columns0 * tile_spacing.x)) / columns,
						(rect_size.y - (rows0 * tile_spacing.y)) / rows)
	assert(tile_size.x > 0 and tile_size.y > 0)

	# Resize image for display if needed
	if Globals.TilesUseImage:
		if Globals.TilesImagePath == "":
			tiles_image = Globals.TilesImageDefault.duplicate()
		else:
			tiles_image = Globals.TilesImage.duplicate()
		# warning-ignore:narrowing_conversion
		# warning-ignore:narrowing_conversion
		tiles_image.resize(tile_size.x * columns, tile_size.y * rows, Image.INTERPOLATE_LANCZOS)

		# Convert image to texture for display
		tiles_texture = ImageTexture.new()
		tiles_texture.create_from_image(tiles_image, 0)
	else:
		tiles_image = null

	# Calculate the bounding boxes for each tile for both display and image
	var tile_size_int = Vector2(int(tile_size.x), int(tile_size.y))
	tiles = []
	var tile
	var left
	var top
#	_log.store_string("tile_size: %s\n" % tile_size)
#	_log.store_string("tile_spacing: %s\n" % tile_spacing)
	for row in range(rows):
		for col in range(columns):
			tile = [null, null]
			left = (col * tile_size.x) + (col * tile_spacing.x)
			top = (row * tile_size.y) + (row * tile_spacing.y)
			tile[IDX_DEST] = Rect2(Vector2(int(left), int(top)), tile_size_int)
			left = col * tile_size.x
			top = row * tile_size.y
			tile[IDX_SRC] = Rect2(Vector2(int(left), int(top)), tile_size_int)
			tiles.append(tile)

#			_log.store_string("tile %2d: D=[(%3d, %3d), (%3d, %3d)]\n" % [len(tiles),
#																			tile[IDX_DEST].position.x,
#																			tile[IDX_DEST].position.y,
#																			tile[IDX_DEST].end.x,
#																			tile[IDX_DEST].end.y])
