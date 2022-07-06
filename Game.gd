extends Control

var hint_visible: bool = false

onready var _moves: Control = get_node("PanelContainer/VBoxContainer/HFlowContainer/Moves")
onready var _tiles: Control = get_node("PanelContainer/VBoxContainer/Tiles")
onready var _win: WindowDialog = get_node("WindowDialog")


func _on_Abort_pressed():
	var _unused = get_tree().change_scene("res://Main.tscn")


func _on_Hint_pressed():
	print("Game: _on_Hint_pressed")


func _on_Load_pressed():
	print("Game: _on_Load_pressed")


func _on_Quit_pressed():
	get_tree().quit()


func _on_Save_pressed():
	print("Game: _on_Save_pressed")


func _on_Tiles_item_rect_changed():
	if _tiles != null:
		_tiles.recalc_tiles()


func _on_Tiles_moved(_direction, count):
	_moves.text = str(count)


func _on_Tiles_won():
	_win.popup_centered()


func _on_Close_pressed():
	_win.hide()
	get_tree().change_scene("res://Main.tscn")
