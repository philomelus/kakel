extends Control

# Spacing between edge of canvas and tiles
const margin = Vector2(10, 10)

# Spacing between each tile
const tile_spacing = Vector2(5, 5)

# Font for drawing numerical id's of tiles
var tile_font: DynamicFont = load("res://tile_font.tres")

# Image/Texture for display
var tile_image: Image
var tile_texture: ImageTexture

# Size of each tile in pixels
var tile_size: Vector2

# Data for each tile.  Each item is a dictionary.
# Valid indexes of dictionaries:
# dest_rect = Rect2 of display location
# src_rect = Rect2 of source image
var tiles: Array = []

# Actual position of each tile (i.e. index 0 = tile at upper left)
var tiles_order: Array = []

# Total size of tile image in pixels
var tiles_size: Vector2

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

# Index within tiles of the empty tile
var empty: int

# Index value of the empty tile (always the last global tile index)
var empty_id: int


func _draw():
	var index = 0
	var extent
	var area
	var name
	var tile
	for tile_index in tiles_order:
		tile = tiles[index]
		area = tile["dest_rect"]
		#if tile_index == empty_id:
		#	draw_line(area.position, area.end, Color(1,0,0,1))
		#else:
		if tile_index != empty_id:
			if false:
				name = "%d" % (tile_index + 1)
				extent = tile_font.get_string_size(name)
				draw_rect(area, Globals.TilesColor, false)
				draw_string(tile_font, Vector2(area.position.x + ((area.size.x - extent.x) / 2),
											   area.position.y + ((area.size.y - extent.y) / 2) + extent.y),
							name, Globals.TilesFontColor)
			else:
				draw_texture_rect_region(tile_texture, tiles[index]["dest_rect"],
										 tiles[tiles_order[index]]["src_rect"])
		index += 1


func _on_Control_resized():
	_ready()


func _ready():
	# Precalc these as they are used several times
	var columns = int(Globals.TilesSize.x)
	var columns0 = columns - 1
	var rows = int(Globals.TilesSize.y)
	var rows0 = rows - 1

	# Determine width and height of tiles from viewport size
	#var screen = get_viewport_rect()
	var screen = Rect2(rect_position, rect_size)
	tile_size = Vector2(int((screen.size.x - (2 * margin.x) - (columns0 * tile_spacing.x)) / columns),
						int((screen.size.y - (2 * margin.y) - (rows0 * tile_spacing.y)) / rows))
	tiles_size = Vector2(int(((tile_size.x + margin.x) * columns0) + tile_size.x),
						 int(((tile_size.y + margin.y) * rows0) + tile_size.y))

	# Total size of each tile with spacing between tiles
	var xext = tile_size.x + tile_spacing.x
	var yext = tile_size.y + tile_spacing.y

	# Load and resize image for display
	tile_image = ResourceLoader.load("res://default_image.png", "Image")
	# warning-ignore:narrowing_conversion
	# warning-ignore:narrowing_conversion
	tile_image.resize(tile_size.x * columns, tile_size.y * rows, Image.INTERPOLATE_LANCZOS)
	tile_texture = ImageTexture.new()
	tile_texture.create_from_image(tile_image, 0)

	# Pre-calculate the bounding boxes for each tile for both display and image
	var tile
	for row in range(rows):
		for col in range(columns):
			tile = {}
			tile["dest_rect"] = Rect2(Vector2(margin.x + (col * xext), margin.y + (row * yext)),
									  Vector2(tile_size.x, tile_size.y))
			print("tile %d @ %s" % [len(tiles) + 1, tile["dest_rect"]])
			tile["src_rect"] = Rect2(Vector2(int(col * tile_size.x), int(row * tile_size.y)), tile_size)
			tiles.append(tile)

	# Total number of tiles
	var num_tiles = columns * rows
	var num_tiles0 = num_tiles - 1

	# Initial blank tiles is last tile
	empty = num_tiles0
	empty_id = empty

	# Need some random numbers
	var rng = RandomNumberGenerator.new()
	rng.randomize()

	# Determine initial random tile order
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
	if event.is_action_type():
		if event.is_action_pressed("refresh"):
			update()
		if event.is_action_pressed("quit"):
			var _unused = get_tree().change_scene("res://Main.tscn")
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
	if event is InputEventMouseButton:
		if event.button_index == 1:
			if not event.doubleclick:
				if not event.pressed:
					var p = event.position
					p -= rect_position
					if can_move_down:
						if tiles[move_down_index]["dest_rect"].has_point(p):
							accept_event()
							move_down();
							return
					if can_move_left:
						if tiles[move_left_index]["dest_rect"].has_point(p):
							accept_event()
							move_left();
							return
					if can_move_right:
						if tiles[move_right_index]["dest_rect"].has_point(p):
							accept_event()
							move_right();
							return
					if can_move_up:
						if tiles[move_up_index]["dest_rect"].has_point(p):
							accept_event()
							move_up();
							return


func calc_movables():
	# These calcs are done more than once (_mo = minus one)
	var cols = int(Globals.TilesSize.x)
	var cols0: int = cols - 1
	var rows = int(Globals.TilesSize.y)
	var rows0: int = rows - 1

	# Row and column of blank tile
	# warning-ignore:integer_division
	var row: int = int(empty / cols)
	var column: int = empty % cols

	# Determine tiles that can move horizontally
	var left_side: bool = (column == 0)
	var right_side: bool = (column == cols0)
	var inside_h: bool = (column > 0 and column < cols0)
	var can_move_left_index = ((row * cols) + column + 1)
	var can_move_right_index = ((row * cols) + column - 1)

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
		move_left_index = can_move_left_index
	if can_move_right:
		move_right_index = can_move_right_index

	# Determine tiles that can move vertically
	var top_side: bool = (row == 0)
	var bottom_side: bool = (row == rows0)
	var inside_v: bool = (row > 0 and row < rows0)

	var can_move_up_index = (((row + 1) * cols) + column)
	var can_move_down_index = (((row - 1) * cols) + column)

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
		move_up_index = can_move_up_index
	if can_move_down:
		move_down_index = can_move_down_index


# Return true if puzzle is in correct sequence
func is_complete():
	var index = 0
	for tile in tiles:
		if tiles_order[index] != index:
			return false
		index += 1
	return true


func move_down():
	var newEmpty = tiles_order[move_down_index]
	tiles_order[move_down_index] = empty_id
	tiles_order[empty] = newEmpty
	empty = move_down_index
	calc_movables()
	update()
	if is_complete():
		get_tree().change_scene("res://Main.tscn")


func move_left():
	var newEmpty = tiles_order[move_left_index]
	tiles_order[move_left_index] = empty_id
	tiles_order[empty] = newEmpty
	empty = move_left_index
	calc_movables()
	update()
	if is_complete():
		get_tree().change_scene("res://Main.tscn")


func move_right():
	var newEmpty = tiles_order[move_right_index]
	tiles_order[move_right_index] = empty_id
	tiles_order[empty] = newEmpty
	empty = move_right_index
	calc_movables()
	update()
	if is_complete():
		get_tree().change_scene("res://Main.tscn")


func move_up():
	var newEmpty = tiles_order[move_up_index]
	tiles_order[move_up_index] = empty_id
	tiles_order[empty] = newEmpty
	empty = move_up_index
	calc_movables()
	update()
	if is_complete():
		get_tree().change_scene("res://Main.tscn")
