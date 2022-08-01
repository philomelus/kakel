extends Control

onready var data = preload("res://bin/kakel.gdns").new()

func _on_Button_pressed():
	$Label.text = "Data = " + data.get_data()
