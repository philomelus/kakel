extends Control

func _ready():
	pass

func _on_Cancel_pressed():
	get_tree().change_scene("res://Main.tscn")

func _on_Start_pressed():
	Globals.Columns = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Columns").value
	Globals.Rows = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Rows").value
	get_tree().change_scene("res://Game.tscn")



func _on_UseNumbers_pressed():
	if $PanelContainer/CenterBox/VerticalBox/GridContainer/UseNumbers.pressed:
		Globals.Numbers = true
	else:
		Globals.Numbers = false

