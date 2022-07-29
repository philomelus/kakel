class_name TilesControl
extends Control


# For save files, version of file format.
const FILE_VERSION: int = 1

# Index within tiles of the empty tile.
var _empty: int

# Index value of the empty tile (always the last global tile index,
# i.e. `_numTiles0` below).
var _empty_id: int

# When false, no movement is allowed.
var _moves_enabled: bool
var movable: bool:
    get:
       return _moves_enabled
    set(newVal):
       if _moves_enabled != newVal:
          _moves_enabled = newVal

# Columns in tiles.
var _columns: int = 4
var _columns0: int = 3
@export var columns: int:
    get:
        return _columns
    set(newVal):
        assert(newVal >= 3 and newVal <= 99)
        _columns = newVal
        _columns0 = newVal - 1
        _num_tiles = newVal * rows
        _num_tiles0 = _num_tiles - 1
        _tiles_ready = false

# Rows in tiles.
var _rows: int = 4
var _rows0: int = 3
@export var rows: int:
    get:
        return _rows
    set(newVal):
        assert(newVal >= 3 and newVal <= 99)
        _rows = newVal
        _rows0 = newVal - 1
        _num_tiles = newVal * columns
        _num_tiles0 = _num_tiles - 1
        _tiles_ready = false

# Total number of tiles.
var _num_tiles: int = 16
var _num_tiles0: int = 15
var tiles_count: int:
    get:
        return _num_tiles

# Spacing between each tile.
var _spacing: Vector2 = Vector2(5, 5)
@export var spacing: Vector2:
    get:
        return _spacing
    set(newVal):
        _spacing = newVal

# Font for drawing numerical id's of tiles.
var _number_font: Font
@export var number_font: Font:
    get:
        return _number_font
    set(newVal):
        _number_font = newVal

# Color of font for tile numbers.
var _number_color: Color = Color.WHITE
@export var number_color: Color:
    get:
        return _number_color
    set(newVal):
        _number_color = newVal

# Outline color for non-image tiles.
var _outline_color: Color = Color.BLACK
@export var outline_color: Color:
    get:
        return _outline_color
    set(newVal):
        _outline_color = newVal

# When true, add numbers to all tiles (incuding images).
var _show_numbers: bool
@export var show_numbers: bool:
    get:
        return _show_numbers
    set(newVal):
        if _show_numbers != newVal:
            _show_numbers = newVal
            update()

# When true, add outline to all tiles (incuding images).
var _show_outlines: bool
@export var show_outlines: bool:
    get:
        return _show_outlines
    set(newVal):
        if _show_outlines != newVal:
            _show_outlines = newVal
            update()

var use_image: bool:
    get:
        return image_path != null and image_path != ""

# Image used for tiles.
var _image: Image

# Path to image file.
var _image_path: String
@export var image_path: String:
    get:
        return _image_path
    set(newVal):
        if newVal == "" or newVal == null:
            _image_path = ""
            _image = null
        else:
            _image_path = newVal
            _image = load_image(_image_path)

# `_tiles_image` converted to texture.  Only valid when `_tiles_ready` is true.
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

# When true, `_move_down_index` holds index within `_tiles` that can move down.
var _can_move_down: bool
var can_move_down: bool:
    get:
        return _can_move_down

# When true, `_move_left_index` holds index within `_tiles` than can move left.
var _can_move_left: bool
var can_move_left: bool:
    get:
        return _can_move_left

# When true, `_move_right_index` holds index within `_tiles` than can move right.
var _can_move_right: bool
var can_move_right: bool:
    get:
        return _can_move_right

# When true, `_move_up_index` holds index within `_tiles` than can move up.
var _can_move_up: bool
var can_move_up: bool:
    get:
        return _can_move_up

# Index within `_tiles_order` that can move down. Valid only when
# `_can_move_down` is true.
var _move_down_index: int

# Index within `_tiles_order` that can moves left. Valid only when
# `_can_move_left` is true.
var _move_left_index: int

# Index within `_tiles_order` that can move right. Valid only when
# `_can_move_right` is true.
var _move_right_index: int

# Index within `_tiles_order` that can move up. Valid only when
# `_can_move_up` is true.
var _move_up_index: int

# Number of movements made since initial display.
var _moves: int = 0

# Signalled when a tile is moved. Throttled to 1/50 second.
signal moved(count)

# When > 0, a move has occurred.  This hrottles the `moved` signal.
var _moved_signal: int = 0

# This is signalled when the tiles are all in correct order.
signal won()

# When true internal variables are initialized and ready to use.
var _ready_to_run: bool = false

# Time since last EmitSignal for moves.
var _last_signal: float = 0

# Time since last CheckComplete().
var _last_winner: float = 0

@onready var _tree: SceneTree = get_tree()


func _draw() -> void:
    if not _ready_to_run or not _tiles_ready:
        return
    var extent: Vector2
    var index: int = 0
    var tile_index: int
    var name_: String
    var area: Rect2
    var tile: Array
    for i in range(_num_tiles):
        tile_index = _tiles_order[i]
        if tile_index != _empty_id:
            if use_image:
                draw_texture_rect_region(_tiles_texture, _tiles[index][IDX_DEST],
                        _tiles[_tiles_order[index]][IDX_SRC])
                if show_numbers:
                    name_ = str(tile_index + 1)
                    extent = number_font.get_string_size(name)
                    draw_string(number_font, Vector2(_tiles[index][IDX_DEST].position.x + 5,
                            _tiles[index][IDX_DEST].position.y + 10 + (extent.y / 2)),
                            name_, HORIZONTAL_ALIGNMENT_LEFT, -1, 24, number_color);
                if show_outlines:
                    tile = _tiles[index]
                    area = tile[IDX_DEST]
                    var v: Vector2 = area.position
                    v.x -= 1
                    v.y -= 1
                    area.position = v
                    v = area.size;
                    v.x += 1
                    v.y += 1
                    area.size = v;
                    draw_rect(area, outline_color, false)
            else:
                tile = _tiles[index]
                area = tile[IDX_DEST]
                name_ = str(tile_index + 1)
                extent = number_font.get_string_size(name)
                draw_rect(area, outline_color, false)
                draw_string(number_font,
                            Vector2(area.position.x + ((area.size.x - extent.x) / 2),
                            area.position.y + ((area.size.y - extent.y) / 2) + extent.y),
                            name_, HORIZONTAL_ALIGNMENT_CENTER, -1, 24, number_color)
        index += 1


func _input(ev: InputEvent) -> void:
    # Do nothing while paused or uninitialized
    if not _ready_to_run or not _tiles_ready or _tree.paused or not _moves_enabled:
        return

    # Only handle mouse clicks here
    if not ev is InputEventMouseButton:
        return
    var iemb: InputEventMouseButton = ev as InputEventMouseButton

    # Only handle left mouse clicks
    if iemb.button_index != 1:
        return

    # Don't pay attention to double clicks
    if iemb.double_click:
        return

    # Only repond to clicks when released
    if iemb.pressed:
        return

    # Adjust position of click to local coordinates.
    var evmb: InputEventMouseButton = make_input_local(iemb) as InputEventMouseButton

    # Take appropriate action if needed
    var sub: Rect2
    if _can_move_down:
        sub = _tiles[_move_down_index][IDX_DEST]
        if sub.has_point(evmb.position):
            accept_event()
            move_down()
            return
    if _can_move_left:
        sub = _tiles[_move_left_index][IDX_DEST]
        if sub.has_point(evmb.position):
            accept_event()
            move_left()
            return
    if _can_move_right:
        sub = _tiles[_move_right_index][IDX_DEST]
        if sub.has_point(evmb.position):
            accept_event()
            move_right()
            return
    if _can_move_up:
        sub = _tiles[_move_up_index][IDX_DEST]
        if sub.has_point(evmb.position):
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
        var _x = check_complete();
        _last_winner = 0
    else:
        _last_winner += delta


func _ready() -> void:
    _ready_to_run = false
    _moves_enabled = false


func _unhandled_input(ev: InputEvent) -> void:
    if not _ready_to_run or not _tiles_ready or _tree.paused or not _moves_enabled or not ev.is_action_type():
        return

    # Take appropriate action
    if _can_move_left:
        if ev.is_action_pressed("move_left"):
            accept_event()
            move_left()
    if _can_move_right:
        if ev.is_action_pressed("move_right"):
            accept_event()
            move_right()
    if _can_move_up:
        if ev.is_action_pressed("move_up"):
            accept_event()
            move_up()
    if _can_move_down:
        if ev.is_action_pressed("move_down"):
            accept_event()
            move_down()


func calc_movables() -> void:
    # Do nothing until initialized
    if not _ready_to_run:
        return

    # Row and column of blank tile
    # warning-ignore:integer_division
    @warning_ignore(integer_division)
    var row: int = _empty / columns
    var column: int = _empty % columns

    # Determine tiles that can move horizontally
    var left_side: bool = (column == 0)
    var right_side: bool = (column == _columns0)
    var inside_h: bool = (column > 0 and column < _columns0)
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
        _move_left_index = (row * columns) + column + 1
    if _can_move_right:
        _move_right_index = (row * columns) + column - 1

    # Determine tiles that can move vertically
    var top_side: bool = (row == 0)
    var bottom_side: bool = (row == _rows0)
    var inside_v: bool = (row > 0 and row < _rows0)
    if top_side:
        _can_move_up = true
        _can_move_down = false
    elif bottom_side:
        _can_move_up = false;
        _can_move_down = true;
    elif inside_v:
        _can_move_up = true;
        _can_move_down = true;
    if _can_move_up:
        _move_up_index = ((row + 1) * columns) + column
    if _can_move_down:
        _move_down_index = ((row - 1) * columns) + column


func check_complete() -> bool:
    for index in range(_num_tiles):
        if _tiles_order[index] != index:
            return false
    _moves_enabled = false
    emit_signal("won")
    return true


func load_game(path: String) -> void:
    _moves_enabled = false

    var inp: File = File.new()
    var _x = inp.open(path, File.READ)

    # Read file format version.
    var v: int = inp.get_16()
    if v != FILE_VERSION:
        inp.close()
        return

    # Load columns and rows.
    var co: int = inp.get_8()
    var ro: int = inp.get_8()

    # Load UseImage.
    var use_image_: bool = inp.get_8() != 0

    # Load image path.
    var image_path_: String = inp.get_pascal_string()

    # Load tiles order.
    var num_tiles_: int = inp.get_16()
    var order: Array = []
    for _i in range(num_tiles_):
        order.append(inp.get_16())

    # Read blank tile index.
    var e: int = inp.get_16()
    var eid: int = inp.get_16()

    # Read moves so far
    var m: int = inp.get_32()

    var show_numbers_: bool = inp.get_8() != 0

    # Done loading data
    inp.close()

    if use_image_:
        image_path = image_path_
        _image = load_image(image_path)
    else:
        image_path = ""
        _image = null

    columns = co
    rows = ro
    _tiles_order = order
    _moves = m
    _empty = e
    _empty_id = eid
    show_numbers = show_numbers_
    _moved_signal += 1

    # Reset tiles
    call_deferred("recalc_tiles")
    call_deferred("calc_movables")

    # All ready
    _ready_to_run = true
    _moves_enabled = true
    _tiles_ready = false
    call_deferred("update")


func load_image(path: String) -> Image:
    if path.substr(0, 4) == "res:":
        return ResourceLoader.load(path, "Image") as Image
    else:
        var i: Image = Image.new()
        var _x = i.load(path)
        return i


func move_down() -> void:
    var newEmpty: int = _tiles_order[_move_down_index]
    _tiles_order[_move_down_index] = _empty_id
    _tiles_order[_empty] = newEmpty
    _empty = _move_down_index
    _moves += 1;
    _moved_signal += 1
    calc_movables()
    update()


func move_left() -> void:
    var newEmpty: int = _tiles_order[_move_left_index]
    _tiles_order[_move_left_index] = _empty_id
    _tiles_order[_empty] = newEmpty
    _empty = _move_left_index
    _moves += 1
    _moved_signal += 1
    calc_movables()
    update()


func move_right() -> void:
    var newEmpty: int = _tiles_order[_move_right_index]
    _tiles_order[_move_right_index] = _empty_id
    _tiles_order[_empty] = newEmpty
    _empty = _move_right_index
    _moves += 1
    _moved_signal += 1
    calc_movables()
    update()


func move_up() -> void:
    var newEmpty: int = _tiles_order[_move_up_index]
    _tiles_order[_move_up_index] = _empty_id
    _tiles_order[_empty] = newEmpty
    _empty = _move_up_index
    _moves += 1
    _moved_signal += 1
    calc_movables()
    update()


func recalc_tiles() -> void:
    if not _ready_to_run:
        return

    assert(columns != 0)
    assert(rows != 0)

    # Stop processing that relies on tiles being set.
    _tiles_ready = false

    # Reload and resize if size changed
    if use_image:
        if _image == null or size.x != _image.get_width() or size.y != _image.get_height():
            _image = load_image(_image_path)
            _image.resize(size.x as int, size.y as int)
        _tiles_texture = ImageTexture.create_from_image(_image)

    # Determine width and height of tiles from our size.
    _tile_size = Vector2((size.x - (_columns0 * spacing.x)) / columns,
            (size.y - (_rows0 * spacing.y)) / rows)
    if _tile_size.x <= 0 or _tile_size.y <= 0:
        # If this happens, its a fluke at startup and this will get called again
        # which will be succesful.  In Godot 4+, this doesn't happen.
        return;

    # Calculate the bounding boxes for each tile for both display and image
    _tiles = []
    for _i in range(_num_tiles):
        _tiles.append([])
    var tile
    for row in range(rows):
        for col in range(columns):
            tile = [null, null]
            tile[IDX_DEST] = Rect2(Vector2((col * _tile_size.x) + (col * spacing.x),
                    (row * _tile_size.y) + (row * spacing.y)), _tile_size)
            tile[IDX_SRC] = Rect2(Vector2(col * _tile_size.x, row * _tile_size.y), _tile_size)
            _tiles[(row * columns) + col ] = tile
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
    _moves_enabled = true
    call_deferred("recalc_tiles")
    call_deferred("calc_movables")


func save_game(path: String) -> void:
    if not _ready_to_run:
        return

    var sav: File = File.new()
    var _x = sav.open(path, File.WRITE)

    # Save file version
    sav.store_16(FILE_VERSION)

    # Save TilesSize
    sav.store_8(columns)
    sav.store_8(rows)

    # Save UseImage state
    if use_image:
        sav.store_8(1)
    else:
        sav.store_8(0)

    # Save image path
    sav.store_pascal_string(image_path)

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
    if _show_numbers:
        sav.store_8(1)
    else:
        sav.store_8(0)

    # All done
    sav.close()


func start() -> void:
    reset_tiles()
