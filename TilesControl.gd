class_name TilesControl
extends Control

# Local
var _columns0: int
var _empty: int
var _emptyId: int
var _gameComplete: bool
var _image: Image
var _lastSignal: float
var _lastWinner: float
var _moveDownIndex: int
var _movedSignal: int
var _moveLeftIndex: int
var _moveRightIndex: int
var _moves: int
var _moveUpIndex: int
var _numTiles0: int
var _readyToRun: bool
var _rows0: int
var _tileSize: Vector2			# Pixels of tiles
var _tilesOrder: PoolIntArray
var _tilesReady: bool = false
var _tilesRectScreen: Array 	# Rect2 of tiles in window/display
var _tilesRectTexture: Array	# Rect2 of tiles from tiles image
var _tilesTexture: ImageTexture
onready var _tree: SceneTree = get_tree()

const FILE_VERSION: int = 1

# Color used for unused area of tiles when keeping aspect ratio
export var background_color: Color setget background_color_set, background_color_get
func background_color_get() -> Color:
	return background_color
func background_color_set(newVal: Color) -> void:
	if background_color != newVal:
		background_color = newVal
		update()

# When true, a tile can be moved down.
export var can_move_down: bool setget , can_move_down_get
func can_move_down_get() -> bool:
	return can_move_down

# When true, a tile can be moved left.
export var can_move_left: bool setget , can_move_left_get
func can_move_left_get() -> bool:
	return can_move_left

# When true, a tile can be moved right.
export var can_move_right: bool setget, can_move_right_get
func can_move_right_get() -> bool:
	return can_move_right

# When true, a tile can be moved up.
export var can_move_up: bool setget , can_move_up_get
func can_move_up_get() -> bool:
	return can_move_up

# Number of columns in tiles.
export var columns: int setget columns_set, columns_get
func columns_get() -> int:
	return columns
func columns_set(newVal: int) -> void:
	if columns != newVal:
		columns = newVal
		_columns0 = newVal - 1
		tiles_count = newVal * rows
		_numTiles0 = tiles_count - 1
		_tilesReady = false

# When true, the blank tiles is hilited
export var hilite_blank: bool setget hilite_blank_set, hilite_blank_get
func hilite_blank_get() -> bool:
	return hilite_blank
func hilite_blank_set(newVal: bool) -> void:
	if hilite_blank != newVal:
		hilite_blank = newVal
		update()

# Color used to hilite the blank tile
export var hilite_blank_color: Color setget hilite_blank_color_set, hilite_blank_color_get
func hilite_blank_color_get() -> Color:
	return hilite_blank_color
func hilite_blank_color_set(newVal: Color) -> void:
	if hilite_blank_color != newVal:
		hilite_blank_color = newVal
		update()

# Path to image for tiles.
export var image_path: String setget image_path_set, image_path_get
func image_path_get() -> String:
	return image_path
func image_path_set(newVal: String) -> void:
	if image_path != newVal:
		image_path = newVal
		if newVal != "":
			_image = load_image(newVal)

# When true, image will maintain its aspect ratio
export var keep_aspect: bool setget keep_aspect_set, keep_aspect_get
func keep_aspect_get() -> bool:
	return keep_aspect
func keep_aspect_set(newVal: bool) -> void:
	if keep_aspect != newVal:
		keep_aspect = newVal
		recalc_tiles()

# When true, user movements are allowed.
export var movable: bool setget movable_set, movable_get
func movable_get() -> bool:
	return movable
func movable_set(newVal: bool) -> void:
	if movable != newVal:
		movable = newVal

# Color to use for drawing tile numbers.
export var numbers_color: Color setget numbers_color_set, numbers_color_get
func numbers_color_get() -> Color:
	return numbers_color
func numbers_color_set(newVal: Color) -> void:
	if numbers_color != newVal:
		numbers_color = newVal
		if self.numbers_visible:
			update()

# Font to use for drawing tile numbers.
export var numbers_font: Font setget numbers_font_set, numbers_font_get
func numbers_font_get() -> Font:
	return numbers_font
func numbers_font_set(newVal: Font) -> void:
	if numbers_font != newVal:
		numbers_font = newVal
		if self.numbers_visible:
			update()

# When true, show tile number in upper left.
export var numbers_visible: bool setget numbers_visible_set, numbers_visible_get
func numbers_visible_get() -> bool:
	return numbers_visible
func numbers_visible_set(newVal: bool) -> void:
	if numbers_visible != newVal:
		numbers_visible = newVal
		update()

# Color to use for drawing tile outlines.
export var outlines_color: Color setget outlines_color_set, outlines_color_get
func outlines_color_get() -> Color:
	return outlines_color
func outlines_color_set(newVal: Color) -> void:
	if outlines_color != newVal:
		outlines_color = newVal
		if self.outlines_visible:
			update()

# When true, outline each tile.
export var outlines_visible: bool setget outlines_visible_set, outlines_visible_get
func outlines_visible_get() -> bool:
	return outlines_visible
func outlines_visible_set(newVal: bool) -> void:
	if outlines_visible != newVal:
		outlines_visible = newVal
		update()

# Number of rows in tiles.
export var rows: int setget rows_set, rows_get
func rows_get() -> int:
	return rows
func rows_set(newVal: int) -> void:
	if rows != newVal:
		rows = newVal
		_rows0 = newVal - 1
		tiles_count = newVal * columns
		_numTiles0 = tiles_count - 1
		_tilesReady = false

# Distance between tiles in pixels.
export var spacing: Vector2 setget spacing_set, spacing_get
func spacing_get() -> Vector2:
	return spacing
func spacing_set(newVal: Vector2) -> void:
	if spacing != newVal:
		spacing = newVal

# Total number of tiles in tiles.
export var tiles_count: int setget , tiles_count_get
func tiles_count_get() -> int:
	return tiles_count

# Encapsulate use image
func use_image() -> bool:
	return image_path != ""

signal loaded()
signal moved(count)
signal saved()
signal won()


func _draw() -> void:
	# No drawing before everything is ready
	if not _readyToRun:
		return

	# Tiles need to be defined to draw
	if not _tilesReady:
		return

	# Loop through all tiles and draw them
	var extent: Vector2
	var index: int = 0
	var tileIndex: int
	for i in range(self.tiles_count):
		# Get actual location of tile
		tileIndex = _tilesOrder[i]

		# If its not the blank tile
		if tileIndex != _emptyId:
			if use_image():
				# Copy image to screen
				draw_texture_rect_region(_tilesTexture, _tilesRectScreen[index],
										 _tilesRectTexture[_tilesOrder[index]])

				# Add the tile number is desired
				if self.numbers_visible:
					var name: String = str(tileIndex + 1)
					extent = self.numbers_font.get_string_size(name)
					draw_string(self.numbers_font, Vector2(_tilesRectScreen[index].position.x + 5,
							_tilesRectScreen[index].position.y + 10 + (extent.y / 2)), name, self.numbers_color)

				# Add an outline if desired
				if self.outlines_visible:
					outline(self, _tilesRectScreen[index], self.outlines_color)
			else:	# if (useImage())
				# Draw outline of tile
				var area: Rect2 = _tilesRectScreen[index]
				draw_rect(area, self.outlines_color, false)

				# Draw tile number of tile
				var name: String = str(tileIndex + 1)
				extent = self.numbers_font.get_string_size(name)
				draw_string(self.numbers_font, Vector2(area.position.x + ((area.size.x - extent.x) / 2),
						area.position.y + ((area.size.y - extent.y) / 2) + extent.y), name, self.numbers_color)
		else:	# if (tileIndex != _emptyId)
			if self.hilite_blank:
				var area: Rect2 = _tilesRectScreen[index]
				if self.outlines_visible:
					outline(self, area, self.hilite_blank_color)
				var text: String = "Blank Tile"
				extent = self.numbers_font.get_string_size(text)
				draw_string(self.numbers_font, Vector2(area.position.x + ((area.size.x - extent.x) / 2),
						area.position.y + ((area.size.y - extent.y) / 2) + extent.y), text, self.hilite_blank_color)
		index += 1


func _init() -> void:
	background_color = Color.transparent
	columns = 4
	_columns0 = 3
	_gameComplete = false
	image_path = ""
	_lastSignal = 0.0
	_lastWinner = 0.0
	_movedSignal = 0
	_moves = 0
	movable = false
	tiles_count = 16
	_numTiles0 = 15
	numbers_color = Color(0.8, 0.8, 0.8, 1)
	numbers_visible = true
	outlines_color = Color(0, 0, 0, 1)
	outlines_visible = false
	_readyToRun = false
	rows = 4
	_rows0 = 3
	spacing = Vector2(5, 5)
	_tilesReady = false


func _input(ev: InputEvent) -> void:
	# Do nothing while not started
	if not _readyToRun:
		return

	# Do nothing if started but tiles not setup
	if not _tilesReady:
		return

	# Do nothing if we've been paused or moves are disabled
	if _tree.is_paused() || not movable:
		return

	# Only handle mouse clicks here
	if not ev is InputEventMouseButton:
		return
	var iemb: InputEventMouseButton = ev as InputEventMouseButton

	# Only handle left mouse clicks
	if iemb.get_button_index() != 1:
		return

	# Don't pay attention to double clicks
	if iemb.is_doubleclick():
		return

	# Only repond to clicks when released
	if iemb.is_pressed():
		return

	# Adjust position of click to local coordinates
	var evmb: InputEventMouseButton = make_input_local(iemb) as InputEventMouseButton
	var position: Vector2 = evmb.get_position()

	# Take appropriate action if needed
	if can_move_down:
		if _tilesRectScreen[_moveDownIndex].has_point(position):
			accept_event()
			move_down()
			return
	if can_move_left:
		if _tilesRectScreen[_moveLeftIndex].has_point(position):
			accept_event()
			move_left()
			return
	if can_move_right:
		if _tilesRectScreen[_moveRightIndex].has_point(position):
			accept_event()
			move_right()
			return
	if can_move_up:
		if _tilesRectScreen[_moveUpIndex].has_point(position):
			accept_event()
			move_up()
			return

func _physics_process(delta: float) -> void:
	# Do nothing until started
	if not _readyToRun:
		return

	# Update tiles if needed.
	if not _tilesReady:
		recalc_tiles()

	# Emit moved signal if needed @ 1/50th second.
	if _movedSignal > 0:
		if _lastSignal + delta > 0.02:
			var m: int = _moves
			_movedSignal = 0
			emit_signal("moved", m)
			_lastSignal = 0.0
		else:
			_lastSignal += delta

	# Limit winning checks to 1/20 second.
	if _lastWinner + delta > 0.05:
		check_complete()
		_lastWinner = 0.0
	else:
		_lastWinner += delta


func _ready() -> void:
	_readyToRun = false
	movable = false


func _unhandled_input(ev: InputEvent) -> void:
	if not _readyToRun || not _tilesReady || _tree.is_paused() || not movable || not ev.is_action_type():
		return

	# Take appropriate action
	if can_move_left:
		if ev.is_action_pressed("move_left"):
			accept_event()
			move_left()
			return
	if can_move_right:
		if ev.is_action_pressed("move_right"):
			accept_event()
			move_right()
			return
	if can_move_up:
		if ev.is_action_pressed("move_up"):
			accept_event()
			move_up()
			return
	if can_move_down:
		if ev.is_action_pressed("move_down"):
			accept_event()
			move_down()
			return


func calc_movables() -> void:
	# Do nothing until initialized
	if not _readyToRun:
		return

	# Row and column of blank tile
	# warning-ignore:integer_division
	var row: int = _empty / columns
	var column: int = _empty % columns

	# Determine tiles that can move horizontally
	var left_side: bool = (column == 0)
	var right_side: bool = (column == _columns0)
	var inside_h: bool = (column > 0 and column < _columns0)
	if left_side:
		can_move_left = true
		can_move_right = false
	elif right_side:
		can_move_left = false
		can_move_right = true
	elif inside_h:
		can_move_left = true
		can_move_right = true
	if can_move_left:
		_moveLeftIndex = (row * columns) + column + 1
	if can_move_right:
		_moveRightIndex = (row * columns) + column - 1

	# Determine tiles that can move vertically
	var top_side: bool = (row == 0)
	var bottom_side: bool = (row == _rows0)
	var inside_v: bool = (row > 0 and row < _rows0)
	if top_side:
		can_move_up = true
		can_move_down = false
	elif bottom_side:
		can_move_up = false
		can_move_down = true
	elif inside_v:
		can_move_up = true
		can_move_down = true
	if can_move_up:
		_moveUpIndex = ((row + 1) * columns) + column
	if can_move_down:
		_moveDownIndex = ((row - 1) * columns) + column


func check_complete() -> void:
	# Only signal winner once
	if _gameComplete:
		return

	for index in range(self.tiles_count):
		if _tilesOrder[index] != index:
			return
	_gameComplete = true
	self.movable = false
	emit_signal("won")


func load_game(path: String) -> void:
	self.movable = false

	var inp: File = File.new()
	var err = inp.open(path, File.READ)
	if err != OK:
		print("TilesControl::load_game: File.open returned %s" % [err])
		return

	# Read file format version.
	var v: int = inp.get_16()
	if v != FILE_VERSION:
		print("TilesControl::load_game: Unknown file version %s" % [v])
		return

	# Load columns and rows.
	var co: int = inp.get_8()
	var ro: int = inp.get_8()

	# Load UseImage.
	var useImage_: bool = (inp.get_8() != 0)

	# Load image path.
	var imagePath_: String = inp.get_pascal_string()

	# Load tiles order.
	var numTiles_: int = inp.get_16()
	var order: PoolIntArray = PoolIntArray()
	order.resize(numTiles_)
	for i in range(numTiles_):
		order[i] = inp.get_16()

	# Read blank tile index.
	var e: int = inp.get_16()
	var eid: int = inp.get_16()

	# Read moves so far
	var m: int = inp.get_32()

	var numbersVisible_: bool = (inp.get_8() != 0)
	var outlinesVisible_: bool = (inp.get_8() != 0)
	var keepAspect_: bool = (inp.get_8() != 0)

	# Done loading data
	inp.close()

	# Re/Initialize all settings for loaded game
	if useImage_:
		self.image_path = imagePath_
	else:
		self.image_path = ""
	self.columns = co
	self.rows = ro
	_tilesOrder = order
	_moves = m
	_empty = e
	_emptyId = eid
	self.numbers_visible = numbersVisible_
	self.outlines_visible = outlinesVisible_
	self.keep_aspect = keepAspect_
	_movedSignal += 1

	# Let owner know a game was loaded
	emit_signal("loaded")

	# All ready
	_readyToRun = true
	self.movable = true
	_tilesReady = false
	call_deferred("update")
	call_deferred("calc_movables")


func load_image(path: String) -> Image:
	var i: Image
	print("TilesControl::load_image: Loading image from \"%s\"" % [path])
	if path.substr(0, 4) == "res:":
		i = ResourceLoader.load(path, "Image", true)
	else:
		i = Image.new()
		var _r = i.load(path)
		assert(_r == OK)
	return i


func move_down() -> void:
	var newEmpty: int = _tilesOrder[_moveDownIndex]
	_tilesOrder[_moveDownIndex] = _emptyId
	_tilesOrder[_empty] = newEmpty
	_empty = _moveDownIndex
	_moves += 1
	_movedSignal += 1
	calc_movables()
	update()


func move_left() -> void:
	var newEmpty: int = _tilesOrder[_moveLeftIndex]
	_tilesOrder[_moveLeftIndex] = _emptyId
	_tilesOrder[_empty] = newEmpty
	_empty = _moveLeftIndex
	_moves += 1
	_movedSignal += 1
	calc_movables()
	update()


func move_right() -> void:
	var newEmpty: int = _tilesOrder[_moveRightIndex]
	_tilesOrder[_moveRightIndex] = _emptyId
	_tilesOrder[_empty] = newEmpty
	_empty = _moveRightIndex
	_moves += 1
	_movedSignal += 1
	calc_movables()
	update()


func move_up() -> void:
	var newEmpty: int = _tilesOrder[_moveUpIndex]
	_tilesOrder[_moveUpIndex] = _emptyId
	_tilesOrder[_empty] = newEmpty
	_empty = _moveUpIndex
	_moves += 1
	_movedSignal += 1
	calc_movables()
	update()


func outline(on: TilesControl, where: Rect2, color: Color) -> void:
	var area: Rect2 = where
	var v: Vector2 = area.position
	v.x -= 1
	v.y -= 1
	area.position = v
	v = area.size
	v.x += 2
	v.y += 2
	area.size = v
	on.draw_rect(area, color, false)

func recalc_tiles(forced: bool = false) -> void:
	if not _readyToRun:
		return

	if columns < 3 or columns > 99:
		print("TilesControl::recalc_tiles: columns must be 3 to 99, not %s" % [columns])
		return
	if rows < 3 or rows > 99:
		print("TilesControl::recalc_tiles: rows must be 3 to 99, not %s" % [rows])
		return

	# Stop processing that relies on tiles being set.
	_tilesReady = false

	# Reload and resize image if size changed
	var canvasSize: Vector2 = rect_size
	# In godot 3.x, this can be called before screen/window/canvas is set correctly,
	# so just bail.  It will get called again with the correct settings implemented.
	if canvasSize.x <= 0 or canvasSize.y <= 0:
		print("TilesControl::recalc_tiles: invalid canvas size = %s" % [canvasSize])
		return
	if use_image():
		var i: Image
		if forced or not _image.is_empty() or canvasSize.x != _image.get_width() or canvasSize.y != _image.get_height():
			_image = load_image(image_path)
			if keep_aspect:
				var imageSize: Vector2 = _image.get_size()
				var h: float = canvasSize.x * (imageSize.y / imageSize.x)
				var w: float = canvasSize.y * (imageSize.x / imageSize.y)
				var newSize: Vector2 = Vector2(canvasSize.x, h) if h <= canvasSize.y else Vector2(w, canvasSize.y)
				if newSize.x < canvasSize.x or newSize.y < canvasSize.y:
					# Resize original image
					var tempImage: Image = _image.duplicate()
					tempImage.resize(int(newSize.x), int(newSize.y))
					tempImage.convert(Image.FORMAT_RGBA8);

					# Create and fill final image with transparency
					i = Image.new()
					i.create(int(canvasSize.x), int(canvasSize.y), false, Image.FORMAT_RGBA8)
					i.fill(self.background_color)

					# Copy resized image into final image, centered
					var srcRect: Rect2 = Rect2(Vector2(0, 0), newSize)
					var dst: Vector2 = Vector2(0, 0)
					if newSize.x < canvasSize.x:
						dst.x = canvasSize.x - newSize.x
						dst.x /= 2
					if newSize.y < canvasSize.y:
						dst.y = canvasSize.y - newSize.y
						dst.y /= 2
					i.blit_rect(tempImage, srcRect, dst)
				else:
					i = _image.duplicate()
					i.resize(int(newSize.x), int(newSize.y))
			else:
				i = _image.duplicate()
				i.resize(int(canvasSize.x), int(canvasSize.y))
		var it: ImageTexture = ImageTexture.new()
		it.create_from_image(i, 0)
		_tilesTexture = it

	# Determine width and height of tiles from our size.
	_tileSize.x = (canvasSize.x - (_columns0 * spacing.x)) / columns
	_tileSize.y = (canvasSize.y - (_rows0 * spacing.y)) / rows
	if _tileSize.x <= 0 or _tileSize.y <= 0:
		# If this happens, its a fluke at startup and this will get called again
		# which will be succesful.  In Godot 4+, this doesn't happen.
		print("TilesControl::recalc_tiles: invalid _tileSize = %s" % [_tileSize])
		return

	# Calculate the bounding boxes for each tile for both display and image
	_tilesRectScreen = Array()
	_tilesRectScreen.resize(self.tiles_count)
	_tilesRectTexture = Array()
	_tilesRectTexture.resize(self.tiles_count)
	for row in range(self.rows):
		for col in range(self.columns):
			var idx: int = (row * columns) + col
			_tilesRectScreen[idx] = Rect2(Vector2(2 + (col * _tileSize.x) + (col * self.spacing.x),
					2 + (row * _tileSize.y) + (row * self.spacing.y)), _tileSize)
			_tilesRectTexture[idx] = Rect2(Vector2(col * _tileSize.x, row * _tileSize.y), _tileSize)
	_tilesReady = true
	call_deferred("update")


func reset_tiles() -> void:
	# Initial blank tile is last tile.
	_empty = _numTiles0
	_emptyId = _empty

	# Need some random numbers.
	var rng: RandomNumberGenerator = RandomNumberGenerator.new()
	rng.randomize()

	# Determine tile order.
	_tilesOrder = PoolIntArray()
	_tilesOrder.resize(self.tiles_count)
	for i in range(self.tiles_count):
		_tilesOrder[i] = 9999
	var order: int
	for i in range(self.tiles_count):
		if i == _empty:
			_tilesOrder[i] = _emptyId
		else:
			order = rng.randi_range(0, _numTiles0 - 1)
			while order in _tilesOrder or order == _empty:
				order = rng.randi_range(0, _numTiles0 - 1)
			_tilesOrder[i] = order

	# Initialized, but queue movement and tiles calc.
	_readyToRun = true
	self.movable = true
	_tilesReady = false
	call_deferred("calc_movables")


func save_game(path: String) -> void:
	if not _readyToRun:
		return

	var sav: File = File.new()
	var err = sav.open(path, File.WRITE)
	if err != OK:
		print("TilesControl::save_game: File.open returned %s" % [err])
		return

	# Save file version
	sav.store_16(FILE_VERSION)

	# Save TilesSize
	sav.store_8(self.columns)
	sav.store_8(self.rows)

	# Save UseImage state
	sav.store_8(1 if use_image() else 0)

	# Save image path
	sav.store_pascal_string(self.image_path)

	# Save tiles order
	sav.store_16(self.tiles_count)
	for index in range(self.tiles_count):
		sav.store_16(_tilesOrder[index])

	# Save index of empty.
	sav.store_16(_empty)
	sav.store_16(_emptyId)

	# Save number of moves so far.
	sav.store_32(_moves)

	# Visible outlines and number
	sav.store_8(1 if self.numbers_visible else 0)
	sav.store_8(1 if self.outlines_visible else 0)

	# Keep aspect
	sav.store_8(1 if self.keep_aspect else 0)

	# All done
	sav.close()

	# Let owner know a game was saved
	emit_signal("saved")


func start() -> void:
	reset_tiles()
