extends Node2D


func _on_Quit_pressed():
	get_tree().quit()


func _on_Abort_pressed():
	var _unused = get_tree().change_scene("res://Main.tscn")


func _on_Save_pressed():
	pass # Replace with function body.


func _on_Load_pressed():
	pass # Replace with function body.


func _on_Tiles_minimum_size_changed():
	print("_on_Tiles_minimum_size_changed")


func _on_Tiles_size_flags_changed():
	print("_on_Tiles_size_flags_changed")

func _on_Tiles_moved(count):
	$PanelContainer/VBoxContainer/HBoxContainer/Moves.text = str(count)
