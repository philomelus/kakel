extends Control

func _ready():
	pass

func _on_Cancel_pressed():
	var _unused = get_tree().change_scene("res://Main.tscn")

func _on_Start_pressed():
	Globals.Columns = $PanelContainer/CenterBox/VerticalBox/GridContainer/Columns.value
	Globals.Rows = $PanelContainer/CenterBox/VerticalBox/GridContainer/Rows.value
	var _unused = get_tree().change_scene("res://Game.tscn")



func _on_UseNumbers_pressed():
	if $PanelContainer/CenterBox/VerticalBox/GridContainer/UseNumbers.pressed:
		Globals.Numbers = true
	else:
		Globals.Numbers = false

