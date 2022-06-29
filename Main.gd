extends Control


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_New_pressed():
	new_game()


func _on_Load_pressed():
	pass # Replace with function body.


func _on_Quit_pressed():
	get_tree().quit()


func _unhandled_input(event):
	if event.is_action_pressed("quit"):
		get_tree().quit()
	if event.is_action_pressed("new"):
		new_game()

func new_game():
	var _unused =  get_tree().change_scene("res://NewGame.tscn")
