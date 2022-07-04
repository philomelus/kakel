extends Control

onready var _moves = get_node("PanelContainer/VBoxContainer/HFlowContainer/Moves")
onready var _tiles = get_node("PanelContainer/VBoxContainer/Tiles")

func _notification(what):
	if what == NOTIFICATION_RESIZED:
		print("_notification: %s" % what)


func _on_Abort_pressed():
	var _unused = get_tree().change_scene("res://Main.tscn")


func _on_Hint_pressed():
	print("Game: Hint: pressed")


func _on_Load_pressed():
	print("Game: Load: pressed")


func _on_Quit_pressed():
	get_tree().quit()


func _on_Save_pressed():
	print("Game: Save: pressed")


func _on_Tiles_moved(count):
	_moves.text = str(count)


#----------------------------------------------------------------------

func _on_Control_item_rect_changed():
	print("Game: _on_Control_item_rect_changed: %s" % [rect_size])


func _on_Control_minimum_size_changed():
	print("Game: Control: _on_Control_minimum_size_changed: %s" % [rect_min_size])


func _on_Control_resized():
	print("Game: _on_Control_resized: %s" % [rect_size])


func _on_Tiles_item_rect_changed():
	print("Game: Tiles: _on_Tiles_item_rect_changed: %s %s" % [_tiles.rect_position,
															   _tiles.rect_size])
func _on_Tiles_minimum_size_changed():
	print("Game: Tiles: _on_Tiles_minimum_size_changed: %s" % [_tiles.rect_min_size])


func _on_Tiles_resized():
	print("Game: Tiles: _on_Tiles_resized: %s" % [_tiles.rect_size])


func _on_Tiles_size_flags_changed():
	print("Game: Tiles: _on_Tiles_size_flags_changed: %s %s" % [_tiles.size_flags_horizontal,
																_tiles.size_flags_vertical])


