extends Control

func _on_Cancel_pressed():
	var _unused = get_tree().change_scene("res://Main.tscn")

func _on_Start_pressed():
	Globals.TilesSize = Vector2($PanelContainer/CenterBox/VerticalBox/GridContainer/Columns.value,
								$PanelContainer/CenterBox/VerticalBox/GridContainer/Rows.value)
	var _unused = get_tree().change_scene("res://Game.tscn")
