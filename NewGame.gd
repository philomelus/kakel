extends Control


onready var _browse: Button = get_node("PanelContainer/CenterContainer/VBoxContainer/Browse")
var _changed_image_path: bool = false
onready var _image: Image
onready var _image_path: String
onready var _image_texture: ImageTexture
onready var _tiles_image: TextureRect = get_node("PanelContainer/CenterContainer/VBoxContainer/TilesImage")
onready var _tiles_image_dialog:FileDialog = get_node("TileImageDialog")
var _tiles_image_dialog_used: bool = false
onready var _tree: SceneTree = get_tree()
onready var _use_image: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/HBoxContainer/UseImage")


func _on_browse_pressed() -> void:
	if _tiles_image_dialog_used:
		_tiles_image_dialog.popup()
	else:
		_tiles_image_dialog_used = true;
		if Globals.preferences.last_image != "":
			_tiles_image_dialog.current_path = Globals.preferences.last_image
		_tiles_image_dialog.popup_centered()


func _on_cancel_pressed() -> void:
	var _x = _tree.change_scene("res://Main.tscn")


func _on_start_pressed() -> void:
	if _use_image.pressed:
		Globals.tiles_use_image = true
		if _changed_image_path:
			Globals.tiles_default_image = false
			Globals.preferences.last_image = _image_path
			Globals.preferences.save(Preferences.P_PREFS)
	else:
		Globals.tiles_use_image = false
	var _x = _tree.change_scene("res://Game.tscn")


func _on_tile_image_dialog_file_selected(path: String) -> void:
	_image_path = path
	_image = load_image(_image_path)
	_changed_image_path = true
	update_image()


func _on_use_image_pressed() -> void:
	_browse.disabled = !_use_image.pressed


func _ready() -> void:
	# Update variables from globals
	_use_image.pressed = Globals.tiles_use_image
	_browse.disabled = !Globals.tiles_use_image
	if Globals.tiles_default_image:
		_image_path = Globals.preferences.default_image
	else:
		_image_path = Globals.preferences.last_image
	_image = load_image(_image_path)

	# Resize image and set as texture
	call_deferred("update_image")


func load_image(path: String) -> Image:
	if path.substr(0, 4) == "res:":
		return ResourceLoader.load(path, "Image") as Image
	else:
		var i: Image = Image.new()
		var _x = i.load(path)
		return i


func update_image() -> void:
	_image.resize(_tiles_image.rect_size.x, _tiles_image.rect_size.y)
	_image_texture = ImageTexture.new()
	_image_texture.create_from_image(_image)
	_tiles_image.texture = _image_texture
