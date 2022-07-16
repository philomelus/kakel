extends Control


@onready var _hintDialog: Window = get_node("HintDialog")
@onready var _hintTexture: TextureRect = get_node("HintDialog/TextureRect")
@onready var _tree: SceneTree = get_tree()
@onready var _vboxContainer: VBoxContainer = get_node("PanelContainer/VBoxContainer")


func _on_abort_pressed():
	_tree.change_scene("res://Main.tscn")


func _on_hint_dialog_close_pressed():
	hide_hint()


func _on_hint_dialog_close_requested() -> void:
	print("_on_hint_dialog_close_requested")


func _on_hint_dialog_popup_hide():
	print("_on_hint_dialog_popup_hide")


func _on_hint_dialog_size_changed():
	print("_on_hint_dialog_size_changed")


func _on_hint_pressed():
	_tree.paused = true
	_vboxContainer.hide()
	if _hintTexture.texture == null:
		var image: Image
		if Globals.TilesImage == null:
			image = Globals.TilesImageDefault.duplicate()
		else:
			image = Globals.TilesImage.duplicate()
		_hintTexture.texture = ImageTexture.create_from_image(image)
	_hintDialog.popup_centered()


func _on_hint_dialog_visibility_changed():
	if not _hintDialog.visible:
		_tree.paused = false
		_vboxContainer.show()


func _on_load_dialog_file_selected(path):
	pass


func _on_load_pressed():
	pass


func _on_quit_pressed():
	# TODO:  save first?
	_tree.quit()


func _on_save_dialog_file_selected(path):
	pass


func _on_save_pressed():
	pass


func _on_tiles_moved(direction, count):
	pass


func _on_tiles_won():
	pass


func _on_winner_dialog_close_pressed():
	_tree.paused = false
	var _x = _tree.change_scene("res://Main.tscn")


func _process(delta):
	pass


func _ready():
	pass


func hide_hint():
	_tree.paused = false
	_vboxContainer.show()
	_hintDialog.hide()


