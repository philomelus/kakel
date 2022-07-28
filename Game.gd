extends Control


onready var _hint_close: Button = get_node("HintDialog/CloseHint")
onready var _hint_dialog: WindowDialog = get_node("HintDialog")
var _hint_dialog_used: bool = false
onready var _hint_image: TextureRect = get_node("HintDialog/HintImage")
onready var _game_board: VBoxContainer = get_node("GameBoard")
onready var _load_dialog: FileDialog = get_node("LoadDialog")
var _load_dialog_used: bool = false
onready var _moves: LineEdit = get_node("GameBoard/PanelContainer/HFlowContainer2/HFlowContainer3/Moves")
onready var _prefs: MenuButton = get_node("GameBoard/PanelContainer/HFlowContainer2/HFlowContainer/Prefs")
onready var _save_dialog: FileDialog = get_node("SaveDialog")
var _save_dialog_used: bool = false
onready var _tiles: TilesControl = get_node("GameBoard/MarginContainer/Tiles")
onready var _tree: SceneTree = get_tree()
onready var _winner_close: Button = get_node("WinnerDialog/CloseWinner")
onready var _winner_dialog: WindowDialog = get_node("WinnerDialog")
onready var _winner_label: Label = get_node("WinnerDialog/CenterContainer/Label")


func _input(ev: InputEvent) -> void:
	if ev.is_action_pressed("quit"):
		accept_event()
		abort()
		return
	if ev.is_action_pressed("refresh"):
		accept_event()
		update()
		return


func _on_abort_pressed() -> void:
	abort()


func _on_hint_dialog_close_pressed() -> void:
	hide_hint()


func _on_hint_dialog_popup_hide() -> void:
	hide_hint()


func _on_hint_dialog_item_rect_changed() -> void:
	if _hint_dialog != null:
		update_hint_dialog()


func _on_hint_dialog_resized() -> void:
	if _hint_dialog != null:
		update_hint_dialog()


func _on_hint_pressed() -> void:
	_game_board.hide()
	if _hint_image.texture == null:
		var image: Image
		if Globals.tiles_default_image:
			image = load_image(Globals.preferences.default_image)
		else:
			image = load_image(Globals.preferences.last_image)
		var it: ImageTexture = ImageTexture.new()
		it.create_from_image(image)
		_hint_image.texture = it
	if _hint_dialog_used:
		_hint_dialog.popup()
	else:
		_hint_dialog_used = true
		_hint_dialog.popup_centered()


func _on_load_dialog_file_selected(path: String) -> void:
	if Globals.preferences.last_game != path:
		Globals.preferences.last_game = path
		Globals.preferences.save(Preferences.P_PREFS)
	_tiles.load_game(path)
	_tiles.movable = true


func _on_load_dialog_popup_hide() -> void:
	_tiles.movable = true


func _on_load_pressed():
	_tiles.movable = false
	if _load_dialog_used:
		_load_dialog.popup()
	else:
		_load_dialog_used = true
		if Globals.preferences.last_game != "":
			_load_dialog.current_path = Globals.preferences.last_game
		_load_dialog.popup_centered()


func _on_prefs_item_selected(index: int) -> void:
	var pm: PopupMenu = _prefs.get_popup()
	pm.toggle_item_checked(index)
	var on: bool = pm.is_item_checked(index)
	match index:
		0: # Outlines
			_tiles.show_outlines = on

		1: # Numbers
			_tiles.show_numbers = on


func _on_quit_pressed() -> void:
	# Save game if desired.
	if Globals.preferences.auto_save:
		_tiles.save_game(Globals.preferences.auto_path)

	# Get outta here.
	_tree.quit()


func _on_save_dialog_file_selected(path: String) -> void:
	_tiles.save_game(path)
	_tiles.movable = true


func _on_save_dialog_popup_hide() -> void:
	_tiles.movable = true


func _on_save_pressed() -> void:
	_tiles.movable = false
	if _save_dialog_used:
		_save_dialog.popup()
	else:
		_save_dialog_used = true
		_save_dialog.popup_centered()


func _on_tiles_item_rect_changed() -> void:
	if _tiles != null:
		_tiles.recalc_tiles()


func _on_tiles_moved(count: int) -> void:
	_moves.text = str(count)


func _on_tiles_won() -> void:
	# Update message.
	_winner_label.text = "Congratulations!\nYou won in\n%s\nmoves!" % _moves.text;

	# Size everything correctly.
	_winner_label.rect_position = Vector2(10, 10)
	_winner_label.rect_size = Vector2(_winner_dialog.rect_size.x - 20,
			_winner_dialog.rect_size.y - 30 - _winner_close.rect_size.y)
	_winner_close.rect_position = Vector2((_winner_dialog.rect_size.x - _winner_close.rect_size.x) / 2,
			_winner_dialog.rect_size.y - 10 - _winner_close.rect_size.y)

	# If game was auto started, then remove auto save if desired.
	if Globals.auto_started and Globals.preferences.auto_remove_on_win:
		var d: Directory = Directory.new()
		if d.file_exists(Globals.preferences.auto_path):
			var _x = d.remove(Globals.preferences.auto_path)

	# Notify user.
	_winner_dialog.popup_centered()


func _on_winner_dialog_close_pressed() -> void:
	abort()


func _ready() -> void:
	# Connct to popup menu item selection (no way to do this in editor)
	var pm: PopupMenu = _prefs.get_popup()
	var _x = pm.connect("index_pressed", self, "_on_prefs_item_selected")

	# Update GUI for common settings
	_tiles.number_color = Globals.preferences.number_color
	_tiles.outline_color = Globals.preferences.outline_color
	_tiles.show_numbers = Globals.preferences.show_numbers
	pm.set_item_checked(1, Globals.preferences.show_numbers)
	_tiles.show_outlines = Globals.preferences.show_outlines
	pm.set_item_checked(0, Globals.preferences.show_outlines)

	# Not all globals are used if loading a game.
	if Globals.tiles_loading:
		Globals.tiles_loading = false
		_tiles.load_game(Globals.tiles_load_path)
	else:
		_tiles.columns = Globals.preferences.columns
		_tiles.rows = Globals.preferences.rows
		if Globals.tiles_use_image:
			if Globals.tiles_default_image:
				_tiles.image_path = Globals.preferences.default_image
			else:
				_tiles.image_path = Globals.preferences.last_image
			pm.set_item_disabled(0, false)
			pm.set_item_disabled(1, false)
		else:
			_tiles.image_path = ""
			pm.set_item_disabled(0, true)
			pm.set_item_disabled(1, true)
		_tiles.start()


func abort() -> void:
	var _x = _tree.change_scene("res://Main.tscn")


func hide_hint() -> void:
	_game_board.show()
	_hint_dialog.hide()


func load_image(path: String) -> Image:
	if path.substr(0, 4) == "res:":
		return ResourceLoader.load(path, "Image") as Image
	else:
		var i: Image = Image.new()
		var _x = i.load(path)
		return i


func update_hint_dialog() -> void:
	var ws: Vector2 = _hint_dialog.rect_size
	var cs: Vector2 = _hint_close.rect_size
	_hint_image.rect_position = Vector2(10, 10)
	_hint_image.rect_size = Vector2(ws.x - 20, ws.y - 25 - cs.y)
	_hint_close.rect_position = Vector2((ws.x - cs.x) / 2, ws.y - cs.y - 10)
