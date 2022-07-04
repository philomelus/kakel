extends Control


func _on_Load_pressed():
	pass


func _on_New_pressed():
	new_game()


func _on_Quit_pressed():
	get_tree().quit()


func _unhandled_input(event):
	if event.is_action_pressed("quit"):
		accept_event()
		get_tree().quit()
		return
	if event.is_action_pressed("new"):
		accept_event()
		new_game()
		return


func new_game():
	var _unused =  get_tree().change_scene("res://NewGame.tscn")
