extends Control


onready var _browse: Button = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Right/Browse")
onready var _columns: SpinBox = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Left/Columns")
onready var _default_image: CheckButton = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Right/Default")
var _image: Image
var _image_path: String
var _image_texture: ImageTexture
onready var _number_color: ColorPickerButton = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Left/NumberColor")
onready var _outline_color: ColorPickerButton = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Left/OutlineColor")
onready var _rows: SpinBox = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Left/Rows")
onready var _show_numbers: CheckButton = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Left/ShowNumbers")
onready var _tiles_image: TextureRect = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Right/Tiles")
onready var _tiles_image_dialog: FileDialog = get_node("TilesImageDialog")
var _tiles_image_dialog_used: bool = false
onready var _tree: SceneTree = get_tree()
onready var _use_image: CheckButton = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Right/UseImage")


func _on_Browse_pressed():
	if _tiles_image_dialog_used:
		_tiles_image_dialog.popup()
	else:
		_tiles_image_dialog_used = true
		_tiles_image_dialog.popup_centered()


func _on_Cancel_pressed():
	var _x = _tree.change_scene("res://Main.tscn")


func _on_Default_pressed():
	_browse.disabled = _default_image.pressed
	if _default_image.pressed:
		_image_path = Globals.TilesImageDefault
		_image = Image.new()
		var _x = _image.load(_image_path)
		update_image()


func _on_Start_pressed():
	# warning-ignore:narrowing_conversion
	Globals.TilesColumns = _columns.value
	# warning-ignore:narrowing_conversion
	Globals.TilesRows = _rows.value
	if _use_image.pressed:
		Globals.TilesUseImage = true
		if _default_image.pressed:
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
	Globals.TilesShowNumbers = _show_numbers.pressed
	var _x = _tree.change_scene("res://Game.tscn");


func _on_TilesImageDialog_file_selected(path):
	_image_path = path
	_image = Image.new()
	var _x = _image.load(path)
	update_image()


func _on_UseImage_pressed():
	_default_image.disabled = not _use_image.pressed;
	_browse.disabled = not _use_image.pressed;


func _ready():
	# Update variables from globals
	_use_image.pressed = Globals.TilesUseImage
	if Globals.TilesDefaultImage:
		_image_path = Globals.TilesImageDefault
		_image_texture = null
		_default_image.pressed = true
		_browse.disabled = true
	else:
		_image_path = Globals.TilesImagePath
		_default_image.pressed = false
		_browse.disabled = false
	_image = Image.new()
	var _x = _image.load(_image_path)
	_show_numbers.pressed = Globals.TilesShowNumbers
	_columns.value = Globals.TilesColumns
	_rows.value = Globals.TilesRows
	_number_color.color = Globals.TilesNumberColor
	_outline_color.color = Globals.TilesOutlineColor

	# Resize image and set as texture
	update_image()


func update_image():
	# warning-ignore:narrowing_conversion
	# warning-ignore:narrowing_conversion
	_image.resize(_tiles_image.rect_size.x, _tiles_image.rect_size.y)
	_image_texture = ImageTexture.new()
	_image_texture.create_from_image(_image)
	_tiles_image.texture = _image_texture


