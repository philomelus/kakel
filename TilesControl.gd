extends Control

class_name TilesControl

# For save files, version of file format.
const FILE_VERSION: int = 1

# Index within tiles of the empty tile.
var _empty: int;

# Index value of the empty tile (always the last global tile index,
# i.e. `_numTiles0` below).
var _empty_id: int

# When false, no movement is allowed.
# Public (which has no meaning in GDScript)
var moves_enabled: bool = false

# Columns in tiles.
export var columns: int = 4 setget columns_set
var _columns0: int = 3

func columns_set(cols):
	assert(cols >= 3 and cols <= 99)
	columns = cols
	_columns0 = cols - 1
	_num_tiles = columns * rows
	_num_tiles0 = _num_tiles - 1
	_tiles_ready = false


# Rows in tiles.
export var rows: int = 4 setget rows_set
var _rows0: int = 3

func rows_set(val):
	assert(val >= 3 and val <= 99)
	rows = val
	_rows0 = rows - 1
	_num_tiles = rows * columns
	_num_tiles0 = _num_tiles - 1
	_tiles_ready = false

# Total number of tiles.
var _num_tiles: int = 16
var _num_tiles0: int = 15

# Spacing between each tile.
export var spacing : Vector2 = Vector2(5, 5)

# Font for drawing numerical id's of tiles.
export var number_font: Font

# Outline color for non-image tiles.
export var outline_color: Color = Color.black

# Color of font for tile numbers.
export var number_color: Color = Color.white

# Whn true, add numbers to all tiles (incuding images).
export var show_numbers: bool = false setget _show_numbers_set

func _show_numbers_set(show):
	if show_numbers != show:
		show_numbers = show
		update()

# Used internally to know when image not used
var _use_image: bool setget , _use_image_get

func _use_image_get() -> bool:
	return image_path != null and len(image_path) > 0

# Image used for tiles.
var _image: Image

# Path to image file.
export var image_path: String setget image_path_set

func image_path_set(path) -> void:
	if len(path) == 0:
		image_path = ""
		_image = null
	else:
		image_path = path
		if image_path.substr(0, 4) == "res:":
			_image = load(image_path)
		else:
			_image = Image.new()
			var _x = _image.load(image_path)

# `_tilesImage` converted to texture.  Only valid when `_tilesReady` is true.
var _tiles_texture: ImageTexture

# Size of each tile in pixels.  Only valid when `_tilesReady` is true.
var _tile_size: Vector2

# Data for each tile.  Each tile is a two item Rect array.
var _tiles: Array

# Index within `_tiles`[...][] of Rect2 of tile on `CanvasItem`.
const IDX_DEST: int = 0

# Index within `_tiles`[...][] of Rect2 of tile on `_tilesTexture`.
const IDX_SRC: int = 1

# When true, `_tiles`, `_tilesTexture`, and `_tileSize` are valid.
var _tiles_ready: bool = false

# Actual positions of each tile (i.e. index 0 = tile at upper left).
# Columns before rows (i.e. index 0 = row 0/col 0, index 1 = row 0/col 1,
# index 2 = row 0/col 2, index 3 = row 1/col 0, etc. if `Columns` is 3).
var _tiles_order: Array

# When true, `_moveDownIndex` holds index within `_tiles` that can move down.
var _can_move_down: bool

# When true, `_moveLeftIndex` holds index within `_tiles` than can move left.
var _can_move_left: bool

# When true, `_moveRightIndex` holds index within `_tiles` than can move right.
var _can_move_right: bool

# When true, `_moveUpIndex` holds index within `_tiles` than can move up.
var _can_move_up: bool

# Index within `_tilesOrder` that can move down. Valid only when `_canMoveDown`
# is true.
var _move_down_index: int

# Index within `_tiles_order` that can moves left. Valid only when `_canMoveLeft`
# is true.
var _move_left_index: int

# Index within `_tiles_order` that can move right. Valid only when `_canMoveRight`
# is true.
var _move_right_index: int

# Index within `_tiles_order` that can move up. Valid only when `_canMoveUp`
# is true.
var _move_up_index: int

# Number of movements made since initial display.
var _moves: int = 0

# Signalled when a tile is moved. Throttled to 1/50 second.
signal moved(count)

# When > 0, a move has occurred.  This hrottles the `moved` signal.
var _moved_signal: int = 0;

# This is signalled when the tiles are all in correct order.
signal won()

# When true internal variables are initialized and ready to use.
var _ready_to_run: bool = false;

# Time since last emit_signal for moves.
var _last_signal: float = 0

# Time since last check_complete().
var _last_winner: float = 0

onready var _tree: SceneTree = get_tree()


func _draw() -> void:
	if not _ready_to_run or not _tiles_ready:
		return
	var extent: Vector2
	var index: int = 0
	var tile_index: int
	for i in range(_num_tiles):
		tile_index = _tiles_order[i]
		if tile_index != _empty_id:
			if self._use_image:
				draw_texture_rect_region(_tiles_texture, _tiles[index][IDX_DEST],
						_tiles[_tiles_order[index]][IDX_SRC])
				if self.show_numbers:
					var name: String = str(tile_index + 1)
					extent = self.number_font.get_string_size(name)
					draw_string(self.number_font, Vector2(_tiles[index][IDX_DEST].position.x + 5,
							_tiles[index][IDX_DEST].position.y + 10 + (extent.y / 2)),
							name, self.number_color)
			else:
				var tile: Array = _tiles[index]
				var area: Rect2 = tile[IDX_DEST]
				var name: String = str(tile_index + 1)
				extent = self.number_font.get_string_size(name)
				draw_rect(area, self.outline_color, false)
				draw_string(self.number_font, Vector2(area.position.x + ((area.size.x - extent.x) / 2),
						area.position.y + ((area.size.y - extent.y) / 2) + extent.y),
						name, self.number_color)
		index += 1


func _input(ev: InputEvent) -> void:
	# Do nothing while paused or uninitialized.
	if not _ready_to_run or not _tiles_ready or _tree.paused || not moves_enabled:
		return

	# Only handle mouse clicks here.
	if not ev is InputEventMouseButton:
		return
	var iemb: InputEventMouseButton = ev as InputEventMouseButton;

	# Only handle left mouse clicks.
	if iemb.button_index != BUTTON_LEFT:
		return

	# Don't pay attention to double clicks.
	if iemb.doubleclick:
		return

	# Only repond to clicks when released.
	if iemb.pressed:
		return

	# Adjust position of click to local coordinates
	var evmb: InputEventMouseButton = make_input_local(iemb) as InputEventMouseButton

	# Take appropriate action if needed
	if _can_move_down:
		if _tiles[_move_down_index][IDX_DEST].has_point(evmb.position):
			accept_event()
			move_down()
			return
	if _can_move_left:
		if _tiles[_move_left_index][IDX_DEST].has_point(evmb.position):
			accept_event()
			move_left()
			return
	if _can_move_right:
		if _tiles[_move_right_index][IDX_DEST].has_point(evmb.position):
			accept_event()
			move_right()
			return
	if _can_move_up:
		if _tiles[_move_up_index][IDX_DEST].has_point(evmb.position):
			accept_event()
			move_up()
			return


func _physics_process(delta: float) -> void:
	if not _ready_to_run:
		return

	# Update tiles if needed.
	if not _tiles_ready:
		recalc_tiles()

	# Emit moved signal if needed @ 1/50th second.
	if _moved_signal > 0:
		if _last_signal + delta > 0.02:
			var m: int = _moves
			_moved_signal = 0
			emit_signal("moved", m)
			_last_signal = 0
		else:
			_last_signal += delta

	# Limit winning checks to 1/20 second.
	if _last_winner + delta > 0.05:
		var _x = check_complete()
		_last_winner = 0
	else:
		_last_winner += delta


func _unhandled_input(ev: InputEvent) -> void:
	if not _ready_to_run or not _tiles_ready or _tree.paused or not moves_enabled or not ev.is_action_type():
		return

	# Take appropriate action.
	if _can_move_left:
		if ev.is_action_pressed("move_left"):
			accept_event()
			move_left()
			return
	if _can_move_right:
		if ev.is_action_pressed("move_right"):
			accept_event()
			move_right()
			return
	if _can_move_up:
		if ev.is_action_pressed("move_up"):
			accept_event()
			move_up()
			return
	if _can_move_down:
		if ev.is_action_pressed("move_down"):
			accept_event()
			move_down()
			return


func calc_movables() -> void:
	# Do nothing until initialized.
	if not _ready_to_run:
		return

	# Row and column of blank tile.
	# warning-ignore:integer_division
	var row: int = _empty / self.columns
	var column: int = _empty % self.columns

	# Determine tiles that can move horizontally.
	var left_side: bool = (column == 0)
	var right_side: bool = (column == _columns0)
	var inside_h: bool = (column > 0 && column < _columns0)
	if left_side:
		_can_move_left = true
		_can_move_right = false
	elif right_side:
		_can_move_left = false
		_can_move_right = true
	elif inside_h:
		_can_move_left = true
		_can_move_right = true
	if _can_move_left:
		_move_left_index = (row * self.columns) + column + 1
	if _can_move_right:
		_move_right_index = (row * self.columns) + column - 1;

	# Determine tiles that can move vertically.
	var top_side: bool = (row == 0)
	var bottom_side: bool = (row == _rows0)
	var inside_v: bool = (row > 0 and row < _rows0)
	if top_side:
		_can_move_up = true
		_can_move_down = false
	elif bottom_side:
		_can_move_up = false
		_can_move_down = true
	elif inside_v:
		_can_move_up = true
		_can_move_down = true
	if _can_move_up:
		_move_up_index = ((row + 1) * self.columns) + column
	if _can_move_down:
		_move_down_index = ((row - 1) * self.columns) + column


func check_complete() -> bool:
	for index in range(_num_tiles):
		if _tiles_order[index] != index:
			return false
	moves_enabled = false
	emit_signal("won")
	return true


func load(path: String) -> void:
	moves_enabled = false

	var inp: File = File.new()
	var _x = inp.open(path, File.READ)

	# Read file format version.
	var v: int = inp.get_16()
	assert(v == FILE_VERSION)

	# Load columns and rows.
	var co: int = inp.get_8()
	var ro: int = inp.get_8()

	# Load UseImage.
	var tmp = inp.get_8()
	var use_image: bool = tmp != 0

	# Load image path.
	var _image_path = inp.get_pascal_string()

	# Load tiles order.
	var num_tiles: int = inp.get_16()
	var order: Array = []
	for _i in range(num_tiles):
		order.append(9999)
	for i in range(num_tiles):
		order[i] = inp.get_16()

	# Read blank tile index.
	var e: int = inp.get_16()
	var eid: int = inp.get_16()

	# Read moves so far.
	var m: int = inp.get_32()

	# Read show numbers.
	tmp = inp.get_8()
	var _show_numbers = tmp != 0

	# Done loading data
	inp.close()

	if use_image:
		self.image_path = _image_path
	else:
		self.image_path = ""
	self.columns = co
	self.rows = ro
	_tiles_order = order
	_moves = m
	_empty = e
	_empty_id = eid
	self.show_numbers = _show_numbers
	_moved_signal += 1

	# Reset tiles
	call_deferred("recalc_tiles")
	call_deferred("calc_movables")

	# All ready
	_ready_to_run = true
	moves_enabled = true
	_tiles_ready = false
	call_deferred("update")


func move_down() -> void:
	var new_empty: int = _tiles_order[_move_down_index]
	_tiles_order[_move_down_index] = _empty_id
	_tiles_order[_empty] = new_empty
	_empty = _move_down_index
	_moves += 1
	_moved_signal += 1
	calc_movables()
	update()


func move_left() -> void:
	var new_empty: int = _tiles_order[_move_left_index]
	_tiles_order[_move_left_index] = _empty_id
	_tiles_order[_empty] = new_empty
	_empty = _move_left_index
	_moves += 1
	_moved_signal += 1
	calc_movables()
	update()


func move_right() -> void:
	var new_empty: int = _tiles_order[_move_right_index]
	_tiles_order[_move_right_index] = _empty_id
	_tiles_order[_empty] = new_empty
	_empty = _move_right_index
	_moves += 1
	_moved_signal += 1
	calc_movables()
	update()


func move_up() -> void:
	var new_empty: int = _tiles_order[_move_up_index]
	_tiles_order[_move_up_index] = _empty_id
	_tiles_order[_empty] = new_empty
	_empty = _move_up_index
	_moves += 1
	_moved_signal += 1
	calc_movables()
	update()

	
func recalc_tiles() -> void:
	if not _ready_to_run:
		return

	assert(self.columns >= 3 and self.columns <= 99)
	assert(self.rows >= 3 and self.rows <= 99)

	# Stop processing that relies on tiles being set.
	_tiles_ready = false

	# Reload and resize if size changed
	if self._use_image:
		if _image == null or rect_size.x != _image.get_width() or rect_size.y != _image.get_height():
			if image_path.substr(0, 4) == "res:":
				_image = load(image_path)
			else:
				_image = Image.new()
				var _x = _image.load(self.image_path)
			# warning-ignore:narrowing_conversion
			# warning-ignore:narrowing_conversion
			_image.resize(rect_size.x, rect_size.y)
		_tiles_texture = ImageTexture.new()
		_tiles_texture.create_from_image(_image)

	# Determine width and height of tiles from our size.
	_tile_size = Vector2((rect_size.x - (_columns0 * self.spacing.x)) / self.columns,
							(rect_size.y - (_rows0 * self.spacing.y)) / self.rows)
	if _tile_size.x < 0 or _tile_size.y < 0:
		return

	# Calculate the bounding boxes for each tile for both display and image
	var tile_size: Vector2 = Vector2(_tile_size.x, _tile_size.y)
	_tiles = []
	for _i in range(_num_tiles):
		_tiles.append([])
	var tile: Array
	for row in range(self.rows):
		for col in range(self.columns):
			tile = [Rect2(), Rect2()]
			tile[IDX_DEST] = Rect2(Vector2((col * _tile_size.x) + (col * self.spacing.x),
					(row * _tile_size.y) + (row * self.spacing.y)), tile_size)
			tile[IDX_SRC] = Rect2(Vector2(col * _tile_size.x, row * _tile_size.y),
					tile_size)
			_tiles[(row * self.columns) + col ] = tile
	_tiles_ready = true
	

func reset_tiles() -> void:
	# Initial blank tile is last tile.
	_empty = _num_tiles0
	_empty_id = _empty

	# Need some random numbers.
	var rng: RandomNumberGenerator = RandomNumberGenerator.new()
	rng.randomize()

	# Determine tile order.
	_tiles_order = []
	for _i in range(_num_tiles):
		_tiles_order.append(9999)
	var order: int
	for i in range(_num_tiles):
		if i == _empty:
			_tiles_order[i] = _empty_id
		else:
			order = rng.randi_range(0, _num_tiles0 - 1)
			if i > 0:
				while order in _tiles_order:
					order = rng.randi_range(0, _num_tiles0 - 1)
			else:
				if order == _empty:
					while order == _empty:
						order = rng.randi_range(0, _num_tiles0 - 1)
			_tiles_order[i] = order

	# Initialized, but queue movement and tiles calc.
	_ready_to_run = true
	moves_enabled = true
	call_deferred("recalc_tiles")
	call_deferred("calc_movables")
	

func save(path: String) -> void:
	if not _ready_to_run:
		return

	var sav: File = File.new()
	var _x = sav.open(path, File.WRITE)

	# Save file version
	sav.store_16(FILE_VERSION)

	# Save TilesSize
	sav.store_8(self.columns)
	sav.store_8(self.rows)

	# Save UseImage state
	if self._use_image:
		sav.store_8(1)
	else:
		sav.store_8(0)

	# Save image path
	sav.store_pascal_string(self.image_path)

	# Save tiles order
	sav.store_16(_num_tiles)
	for index in _tiles_order:
		sav.store_16(index)

	# Save index of empty.
	sav.store_16(_empty)
	sav.store_16(_empty_id)

	# Save number of moves so far.
	sav.store_32(_moves)

	# Save show numbers.
	if self.show_numbers:
		sav.store_8(1)
	else:
		sav.store_8(0)

	# All done
	sav.close()

	
func start() -> void:
	reset_tiles()

