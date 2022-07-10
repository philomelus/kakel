extends Control

onready var _browse: Button = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Browse")
onready var _columns: SpinBox = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Columns")
onready var _default: Button = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Default")
onready var _openTileImage: FileDialog = get_node("OpenTileImage")
onready var _rows: SpinBox = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Rows")
onready var _tiles: Control = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Tiles")
onready var _useImage: CheckButton = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/UseImage")

var imagePath: String = ""
var image: Image = null
var imageTexture: ImageTexture = null


func _on_Browse_pressed():
	match OS.get_name():
		"Windows":
			_openTileImage.rect_min_size = Vector2(480, 320)
		_:
			_openTileImage.rect_min_size = Vector2(240, 240)
	_openTileImage.popup_centered()


func _on_Cancel_pressed():
	var _unused = get_tree().change_scene("res://Main.tscn")


func _on_Default_pressed():
	_browse.disabled = _default.pressed


func _on_OpenTileImage_file_selected(path):
	imagePath = path
	image = Image.new()
	var _unused = image.load(path)
	image.resize(int(_tiles.rect_size.x), int(_tiles.rect_size.y))
	imageTexture = ImageTexture.new()
	imageTexture.create_from_image(image, 0)
	_tiles.texture = imageTexture


func _on_Start_pressed():
	Globals.TilesSize = Vector2(_columns.value, _rows.value)
	Globals.TilesUseImage = _useImage.pressed
	if _useImage.pressed:
		if _default.pressed:
			Globals.TilesImage = null
			Globals.TilesImagePath = ""
		else:
			Globals.TilesImagePath = imagePath
			Globals.TilesImage = Image.new()
			var _unused = Globals.TilesImage.load(Globals.TilesImagePath)
	var _unused = get_tree().change_scene("res://Game.tscn")


func _on_UseImage_pressed():
	_default.disabled = not _useImage.pressed
	_browse.disabled = not _useImage.pressed


func _ready():
	# Set up image usage controls
	if Globals.TilesUseImage:
		_useImage.pressed = true
	else:
		_useImage.pressed = false
		_default.disabled = true
		_browse.disabled = true

	# Set up image
	if Globals.TilesImagePath == "":
		imagePath = ""
		image = Globals.TilesImageDefault.duplicate()
		imageTexture = null
		_default.pressed = true
		_browse.disabled = true
	else:
		assert(Globals.TilesImagePath != "")
		imagePath = Globals.TilesImagePath
		image = Globals.TilesImage.duplicate()
		_default.pressed = false
		_browse.disabled = false

	# Resize image and set as texture
	image.resize(int(_tiles.rect_size.x), int(_tiles.rect_size.y))
	imageTexture = ImageTexture.new()
	imageTexture.create_from_image(image, 0)
	_tiles.texture = imageTexture

	# Initialize tile grid size
	_columns.value = Globals.TilesSize.x
	_rows.value = Globals.TilesSize.y
