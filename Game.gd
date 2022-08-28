extends Control


onready var _hint_close: Button = get_node("HintDialog/CloseHint")
onready var _hint_dialog: WindowDialog = get_node("HintDialog")
onready var _hint_image: TextureRect = get_node("HintDialog/HintImage")
onready var _game_board: VBoxContainer = get_node("GameBoard")
onready var _load_dialog: FileDialog = get_node("LoadDialog")
onready var _moves: LineEdit = get_node("GameBoard/PanelContainer/HFlowContainer2/HFlowContainer3/Moves")
onready var _options: MenuButton = get_node("GameBoard/PanelContainer/HFlowContainer2/HFlowContainer/Prefs")
onready var _save_dialog: FileDialog = get_node("SaveDialog")
onready var _tiles: TilesControl = get_node("GameBoard/MarginContainer/Tiles")
onready var _tree: SceneTree = get_tree()
onready var _winner_close: Button = get_node("WinnerDialog/CloseWinner")
onready var _winner_dialog: WindowDialog = get_node("WinnerDialog")
onready var _winner_label: Label = get_node("WinnerDialog/CenterContainer/Label")

var _hint_dialog_used: bool = false
var _load_dialog_used: bool = false
var _save_dialog_used: bool = false

const MI_HILITE_BLANK: int = 3
const MI_KEEP_ASPECT: int = 2
const MI_NUMBERS_VISIBLE: int = 1
const MI_OUTLINES_VISIBLE: int = 0


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
			image = load_image(Preferences.default_image)
		else:
			image = load_image(Preferences.last_image)
		var it: ImageTexture = ImageTexture.new()
		it.create_from_image(image)
		_hint_image.texture = it
	update_hint_dialog()
	if _hint_dialog_used:
		_hint_dialog.popup()
	else:
		_hint_dialog_used = true
		_hint_dialog.popup_centered()


func _on_load_dialog_file_selected(path: String) -> void:
	if Preferences.last_game != path:
		Preferences.last_game = path
		Preferences.save(Preferences.P_PREFS)
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
		if Preferences.last_game != "":
			_load_dialog.current_path = Preferences.last_game
		_load_dialog.popup_centered()



func _on_options_item_selected(index: int) -> void:
	var pm: PopupMenu = _options.get_popup()
	pm.toggle_item_checked(index)
	var on: bool = pm.is_item_checked(index)
	match index:
		MI_KEEP_ASPECT:
			_tiles.keep_aspect = on

		MI_HILITE_BLANK:
			_tiles.hilite_blank = on

		MI_NUMBERS_VISIBLE:
			_tiles.numbers_visible = on

		MI_OUTLINES_VISIBLE:
			_tiles.outlines_visible = on


func _on_quit_pressed() -> void:
	# Save game if desired.
	if Preferences.auto_save:
		_tiles.save_game(Preferences.auto_path)

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


func _on_tiles_loaded() -> void:
	var pm: PopupMenu = _options.get_popup()
	pm.set_item_checked(MI_KEEP_ASPECT, _tiles.keep_aspect)
	pm.set_item_checked(MI_HILITE_BLANK, _tiles.hilite_blank)
	pm.set_item_checked(MI_NUMBERS_VISIBLE, _tiles.numbers_visible)
	pm.set_item_checked(MI_OUTLINES_VISIBLE, _tiles.outlines_visible)
	var disabled: bool = false if _tiles.image_path == "" else true
	pm.set_item_disabled(MI_KEEP_ASPECT, disabled)
	pm.set_item_disabled(MI_NUMBERS_VISIBLE, disabled)
	pm.set_item_disabled(MI_OUTLINES_VISIBLE, disabled)


func _on_tiles_moved(count: int) -> void:
	_moves.text = str(count)


func _on_tiles_won() -> void:
	# Update message.
	_winner_label.text = "Congratulations!\nYou won in\n%s\nmoves!" % _moves.text

	# Size everything correctly.
	_winner_label.rect_position = Vector2(0, 0)
	var wds: Vector2 = _winner_dialog.rect_size
	var wcs: Vector2 = _winner_close.rect_size
	_winner_label.rect_size = Vector2(wds.x, wds.y - 20 - wcs.y)
	_winner_close.rect_position = Vector2((wds.x - wcs.x) / 2, wds.y - 10 - wcs.y)

	# If game was auto started, then remove auto save if desired.
	if Globals.auto_started and Preferences.auto_remove_on_win:
		var d: Directory = Directory.new()
		if d.file_exists(Preferences.auto_path):
			var _x = d.remove(Preferences.auto_path)

	# Notify user.
	_winner_dialog.popup_centered()


func _on_winner_dialog_close_pressed() -> void:
	abort()


func _ready() -> void:
	# Connct to popup menu item selection (no way to do this in editor)
	var pm: PopupMenu = _options.get_popup()
	var _x = pm.connect("index_pressed", self, "_on_options_item_selected")

	# Update GUI for common settings
	_tiles.numbers_color = Preferences.numbers_color
	_tiles.outlines_color = Preferences.outlines_color

	# Numbers visible
	_tiles.numbers_visible = Preferences.numbers_visible
	pm.set_item_checked(1, Preferences.numbers_visible)

	# Outlines visible
	_tiles.outlines_visible = Preferences.outlines_visible
	pm.set_item_checked(0, Preferences.outlines_visible)

	# Keep aspect
	_tiles.keep_aspect = Globals.tiles_keep_aspect
	pm.set_item_checked(MI_KEEP_ASPECT, Globals.tiles_keep_aspect)

	# Hilite blank
	_tiles.hilite_blank = Globals.tiles_hilite_blank
	pm.set_item_checked(MI_HILITE_BLANK, Globals.tiles_hilite_blank)
	_tiles.hilite_blank_color = Preferences.hilite_blank_color

	# Not all globals are used if loading a game.
	if Globals.tiles_loading:
		Globals.tiles_loading = false
		print("Loading game from \"%s\"" % [Globals.tiles_load_path])
		_tiles.load_game(Globals.tiles_load_path)
	else:
		_tiles.columns = Preferences.columns
		_tiles.rows = Preferences.rows
		if Globals.tiles_use_image:
			if Globals.tiles_default_image:
				_tiles.image_path = Preferences.default_image
			else:
				_tiles.image_path = Preferences.last_image
			pm.set_item_disabled(MI_KEEP_ASPECT, false)
			pm.set_item_disabled(MI_NUMBERS_VISIBLE, false)
			pm.set_item_disabled(MI_OUTLINES_VISIBLE, false)
		else:
			_tiles.image_path = ""
			pm.set_item_disabled(MI_KEEP_ASPECT, true)
			pm.set_item_disabled(MI_NUMBERS_VISIBLE, true)
			pm.set_item_disabled(MI_OUTLINES_VISIBLE, true)
		_tiles.start()

	# Haven't quit yes
	Globals.tiles_quit = false


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
