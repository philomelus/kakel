extends Control

onready var _columns = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Columns")
onready var _rows = get_node("PanelContainer/CenterBox/VerticalBox/GridContainer/Rows")

func _on_Cancel_pressed():
	var _unused = get_tree().change_scene("res://Main.tscn")

func _on_Start_pressed():
	Globals.TilesSize = Vector2(_columns.value, _rows.value)
	var _unused = get_tree().change_scene("res://Game.tscn")
