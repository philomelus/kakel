extends Control


@onready var _browse: Button = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Right/Browse")
@onready var _columns: SpinBox = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Left/Columns")
@onready var _default_image: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Right/DefaultImage")
var _image: Image
var _image_path: String
var _image_texture: ImageTexture
@onready var _number_color: ColorPickerButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Left/NumbersColor")
@onready var _outline_color: ColorPickerButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Left/OutlineColor")
@onready var _rows: SpinBox = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Left/Rows")
@onready var _show_numbers: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Left/ShowNumbers")
@onready var _tiles_image: TextureRect = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Right/TilesImage")
@onready var _tiles_image_dialog: FileDialog = get_node("TilesImageDialog")
var _tiles_image_dialog_used: bool = false
@onready var _tree: SceneTree = get_tree()
@onready var _use_image: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Right/UseImage")


func _on_browse_pressed() -> void:
    if _tiles_image_dialog_used:
        _tiles_image_dialog.popup()
    else:
        _tiles_image_dialog_used = true
        _tiles_image_dialog.popup_centered()


func _on_cancel_pressed() -> void:
    var _x = _tree.change_scene("res://Main.tscn")


func _on_default_image_pressed() -> void:
    _browse.disabled = _default_image.button_pressed
    if _default_image.button_pressed:
        _image_path = Globals.TilesImageDefault
        _image = load_image(_image_path)
        update_image()


func _on_start_pressed() -> void:
    Globals.TilesColumns = _columns.value as int
    Globals.TilesRows = _rows.value as int
    if _use_image.button_pressed:
        Globals.TilesUseImage = true
        if _default_image.button_pressed:
            Globals.TilesDefaultImage = true
            Globals.TilesImagePath = Globals.TilesImageDefault
        else:
            Globals.TilesDefaultImage = false
            Globals.TilesImagePath = _image_path
    else:
        Globals.TilesUseImage = false
        Globals.TilesDefaultImage = true
        Globals.TilesImagePath = Globals.TilesImageDefault
    Globals.TilesNumberColor = _number_color.color
    Globals.TilesOutlineColor = _outline_color.color
    Globals.TilesShowNumbers = _show_numbers.button_pressed
    var _x = _tree.change_scene("res://Game.tscn");


func _on_tiles_image_dialog_file_selected(path: String) -> void:
    _image_path = path
    _image = load_image(_image_path)
    update_image()


func _on_use_image_pressed() -> void:
    _default_image.disabled = not _use_image.button_pressed;
    _browse.disabled = not _use_image.button_pressed;


func _ready() -> void:
    # Update variables from globals
    _use_image.button_pressed = Globals.TilesUseImage
    if Globals.TilesDefaultImage:
        _image_path = Globals.TilesImageDefault
        _image_texture = null
        _default_image.button_pressed = true
        _browse.disabled = true
    else:
        _image_path = Globals.TilesImagePath
        _default_image.button_pressed = false
        _browse.disabled = false
    _image = load_image(_image_path)
    _show_numbers.button_pressed = Globals.TilesShowNumbers
    _columns.value = Globals.TilesColumns
    _rows.value = Globals.TilesRows
    _number_color.color = Globals.TilesNumberColor
    _outline_color.color = Globals.TilesOutlineColor

    # Resize image and set as texture
    update_image()


func load_image(path: String) -> Image:
    if path.substr(0, 4) == "res:":
        return ResourceLoader.load(path, "Image", ResourceLoader.CACHE_MODE_IGNORE) as Image
    else:
        var i: Image = Image.new()
        var _x = i.load(path)
        return i


func update_image() -> void:
    _image.resize(_tiles_image.size.x as int, _tiles_image.size.y as int)
    _image_texture = ImageTexture.create_from_image(_image)
    _tiles_image.texture = _image_texture
