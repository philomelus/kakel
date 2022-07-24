extends Control

onready var _hint_close: Button = get_node("HintDialog/CloseHint")
onready var _hint_dialog: WindowDialog = get_node("HintDialog")
var _hint_dialog_used: bool = false
onready var _hint_texture: TextureRect = get_node("HintDialog/TextureRect")
onready var _game_board: VBoxContainer = get_node("PanelContainer/GameBoard")
onready var _load_dialog: FileDialog = get_node("LoadDialog")
var _load_dialog_used: bool = false
onready var _moves: LineEdit = get_node("PanelContainer/GameBoard/HFlowContainer/Moves")
onready var _save_dialog: FileDialog = get_node("SaveDialog")
var _save_dialog_used: bool = false
onready var _show_numbers: CheckButton = get_node("PanelContainer/GameBoard/HFlowContainer/ShowNumbers")
onready var _tiles: TilesControl = get_node("PanelContainer/GameBoard/Tiles")
onready var _tree: SceneTree = get_tree()
onready var _winner_close: Button = get_node("WinnerDialog/CloseWinner")
onready var _winner_dialog: WindowDialog = get_node("WinnerDialog")
onready var _winner_label: Label = get_node("WinnerDialog/CenterContainer/Label")


func _input(ev: InputEvent):
	if ev.is_action_pressed("quit"):
		accept_event()
		_tree.change_scene("res://Main.tscn")
		return
	if ev.is_action_pressed("refresh"):
		accept_event()
		update()
		return


func _on_Abort_pressed():
	_tree.change_scene("res://Main.tscn")


func _on_HintDialog_Close_pressed():
		hide_hint()


func _on_HintDialog_resized():
	var ws: Vector2 = _hint_dialog.rect_size
	var cs: Vector2 = _hint_close.rect_size
	_hint_texture.rect_position = Vector2(10, 10)
	_hint_texture.rect_size = Vector2(ws.x - 20, ws.y - 25 - cs.y)
	_hint_close.rect_position = Vector2((ws.x - cs.x) / 2, ws.y - cs.y - 10)


func _on_Hint_pressed():
	_game_board.hide()
	if _hint_texture.texture == null:
		var image: Image = Image.new()
		if Globals.TilesDefaultImage:
			image.load(Globals.TilesImageDefault)
		else:
			image.load(Globals.TilesImagePath)
		var texture = ImageTexture.new()
		texture.create_from_image(image)
		_hint_texture.texture = texture
	if _hint_dialog_used:
		_hint_dialog.popup()
	else:
		_hint_dialog_used = true
		_hint_dialog.popup_centered()


func _on_LoadDialog_file_selected(path):
	_tiles.load(path)


func _on_Load_pressed():
	if _load_dialog_used:
		_load_dialog.popup()
	else:
		_load_dialog_used = true
		_load_dialog.popup_centered()


func _on_Quit_pressed():
	_tree.quit()


func _on_SaveDialog_file_selected(path):
	_tiles.save(path)


func _on_Save_pressed():
	if _save_dialog_used:
		_save_dialog.popup()
	else:
		_save_dialog_used = true
		_save_dialog.popup_centered()


func _on_ShowNumbers_pressed():
	_tiles.show_numbers = _show_numbers.pressed


func _on_Tiles_item_rect_changed():
#	if _tiles != null:
#		_tiles.recalc_tiles()
	pass


func _on_Tiles_moved(count: int):
	_moves.text = str(count)


func _on_Tiles_resized():
	if _tiles != null:
		_tiles.recalc_tiles()


func _on_Tiles_won():
	_winner_label.rect_position = Vector2(10, 10)
	_winner_label.rect_size = Vector2(_winner_dialog.rect_size.x - 20,
			_winner_dialog.rect_size.y - 30 - _winner_close.rect_size.y)
	_winner_close.rect_position = Vector2((_winner_dialog.rect_size.x - _winner_close.rect_size.x) / 2,
			_winner_dialog.rect_size.y - 10 - _winner_close.rect_size.y)
	_winner_dialog.popup_centered()


func _on_WinnerDialog_Close_pressed():
	_tree.change_scene("res://Main.tscn")


func _ready():
	# If loading a game, no need to pass globals on.
	_tiles.number_color = Globals.TilesNumberColor
	if Globals.TilesNumberFont != null:
		_tiles.number_font = Globals.TilesNumberFont
	assert(_tiles.number_font != null)
	_tiles.outline_color = Globals.TilesOutlineColor
	if Globals.TilesLoading:
		Globals.TilesLoading = false
		_tiles.load(Globals.TilesLoadPath)
		call_deferred("set_show_numbers")
	else:
		# Pass on global settings to tiles control.
		_tiles.columns = Globals.TilesColumns
		_tiles.rows = Globals.TilesRows
		if Globals.TilesUseImage:
			if Globals.TilesDefaultImage:
				_tiles.image_path = Globals.TilesImageDefault
			else:
				_tiles.image_path = Globals.TilesImagePath
		else:
			_tiles.image_path = ""
		_tiles.show_numbers = Globals.TilesShowNumbers
		_show_numbers.pressed = Globals.TilesShowNumbers
		_tiles.start()


func hide_hint():
	_game_board.show()
	_hint_dialog.hide()
	
	
func set_show_numbers():
	_show_numbers.pressed = _tiles.show_numbers

