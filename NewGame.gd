extends Control

# UI
onready var _browse: Button = get_node("PanelContainer/CenterContainer/VBoxContainer/Browse")
onready var _keep_aspect: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/KeepAspect")
onready var _tiles_image: TextureRect = get_node("PanelContainer/CenterContainer/VBoxContainer/TilesImage")
onready var _tiles_image_dialog:FileDialog = get_node("TileImageDialog")
onready var _tree: SceneTree = get_tree()
onready var _use_image: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/UseImage")

# Local
var _changed_image_path: bool = false
var _image: Image
var _image_path: String
var _image_texture: ImageTexture
var _tiles_image_dialog_used: bool = false


func _on_browse_pressed() -> void:
	if _tiles_image_dialog_used:
		_tiles_image_dialog.popup()
	else:
		_tiles_image_dialog_used = true;
		var lastImage = Preferences.last_image
		if lastImage != "":
			_tiles_image_dialog.current_path = lastImage
		_tiles_image_dialog.popup_centered()


func _on_cancel_pressed() -> void:
	var _x = _tree.change_scene("res://Main.tscn")


func _on_keep_aspect_pressed():
	_tiles_image.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT_CENTERED if Globals.tiles_keep_aspect else TextureRect.STRETCH_SCALE
	update_image()


func _on_start_pressed() -> void:
	if _use_image.pressed:
		Globals.tiles_use_image = true
		if _changed_image_path:
			Globals.tiles_default_image = false
			Preferences.last_image = _image_path
			Preferences.save(Preferences.P_PREFS)
	else:
		Globals.tiles_use_image = false
	Globals.tiles_keep_aspect = _keep_aspect.pressed
	var _x = _tree.change_scene("res://Game.tscn")


func _on_tile_image_dialog_file_selected(path: String) -> void:
	_image_path = path
	_image = load_image(_image_path)
	_changed_image_path = true
	call_deferred("update_image")


func _on_use_image_pressed() -> void:
	var pressed = _use_image.pressed
	_browse.disabled = not pressed
	_keep_aspect.disabled = not pressed


func _ready() -> void:
	# Update variables from globals
	var useImage: bool = Globals.tiles_use_image
	_use_image.pressed = useImage == true
	_keep_aspect.pressed = Globals.tiles_keep_aspect
	_keep_aspect.disabled = useImage == false
	_browse.disabled = useImage == false
	if Globals.tiles_default_image:
		_image_path = Preferences.default_image
	else:
		_image_path = Preferences.last_image
	_image = load_image(_image_path)
	_tiles_image.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT_CENTERED if Globals.tiles_keep_aspect else TextureRect.STRETCH_SCALE

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
	var i: Image = _image.duplicate()
	var size: Vector2 = _tiles_image.rect_size
	if _keep_aspect.pressed:
		var imageSize: Vector2 = i.get_size()
		var h: float = size.x * (imageSize.y / imageSize.x)
		var w: float = size.y * (imageSize.x / imageSize.y)
		if h <= size.y:
			i.resize(int(size.x), int(h))
		else:
			i.resize(int(w), int(size.y))
	else:
		i.resize(int(size.x), int(size.y))
	_image_texture = ImageTexture.new()
	_image_texture.create_from_image(i)
	_tiles_image.set_texture(_image_texture)

