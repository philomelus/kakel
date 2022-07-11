extends Control


onready var _abort: Button = get_node("PanelContainer/VBoxContainer/HFlowContainer/Abort")
onready var _closeHint: Button = get_node("HintDialog/CloseHint")
onready var _hintDialog: WindowDialog = get_node("HintDialog")
onready var _load: Button = get_node("PanelContainer/VBoxContainer/HFlowContainer/Load")
onready var _loadDialog: FileDialog = get_node("LoadDialog")
onready var _moves: LineEdit = get_node("PanelContainer/VBoxContainer/HFlowContainer/Moves")
onready var _quit: Button = get_node("PanelContainer/VBoxContainer/HFlowContainer/Quit")
onready var _save: Button = get_node("PanelContainer/VBoxContainer/HFlowContainer/Save")
onready var _saveDialog: FileDialog = get_node("SaveDialog")
onready var _textureRect: TextureRect = get_node("HintDialog/TextureRect")
onready var _tiles: Control = get_node("PanelContainer/VBoxContainer/Tiles")
onready var _tree: SceneTree = get_tree()
onready var _vboxContainer: VBoxContainer = get_node("PanelContainer/VBoxContainer")
onready var _win: WindowDialog = get_node("WinnerDialog")


func _on_Abort_pressed():
	var _unused = _tree.change_scene("res://Main.tscn")


func _on_CloseHint_pressed():
	hide_hint()


func _on_Close_pressed():
	_tree.paused = false
	var _unused = _tree.change_scene("res://Main.tscn")


func _on_HintDialog_item_rect_changed():
	var s: Vector2 = _hintDialog.rect_size
	var cs: Vector2 = _closeHint.rect_size
	_textureRect.rect_size = Vector2(s.x - 20, s.y - 50)
	_closeHint.rect_position = Vector2((s.x - cs.x) / 2, s.y - cs.y - 10)


func _on_HintDialog_modal_closed():
	hide_hint()


func _on_HintDialog_popup_hide():
	hide_hint()


func _on_Hint_pressed():
	_tree.paused = true
	_vboxContainer.hide()
	if _textureRect.texture == null:
		var image;
		if Globals.TilesImage == null:
			image = Globals.TilesImageDefault.duplicate()
		else:
			image = Globals.TilesImage.duplicate()
		var t = ImageTexture.new()
		t.create_from_image(image)
		_textureRect.texture = t
	_hintDialog.popup_centered()


func _on_LoadDialog_file_selected(path):
	_tiles.Load(path)
	_tree.paused = false


func _on_LoadDialog_modal_closed():
	_tree.paused = false


func _on_LoadDialog_popup_hide():
	_tree.paused = false


func _on_Load_pressed():
	_tree.paused = true
	_loadDialog.popup_centered()


func _on_Quit_pressed():
	_tree.quit()


func _on_SaveDialog_file_selected(path):
	_tiles.Save(path)
	_tree.paused = false


func _on_SaveDialog_modal_closed():
	_tree.paused = false


func _on_SaveDialog_popup_hide():
	_tree.paused = false


func _on_Save_pressed():
	_tree.paused = true
	_saveDialog.popup_centered()


func _on_Tiles_item_rect_changed():
	if _tiles != null:
		_tiles.recalc_tiles()


func _on_Tiles_moved(_direction, count):
	_moves.text = str(count)


func _on_Tiles_won():
	_tree.paused = true
	_win.popup_centered()


func hide_hint():
	_tree.paused = false
	_vboxContainer.show()
	_hintDialog.hide()
