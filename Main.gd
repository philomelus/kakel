extends Control


onready var _load_dialog = get_node("LoadDialog")
var _load_dialog_used: bool = false
onready var _title: Label = get_node("PanelContainer/CenterContainer/VBoxContainer/Title")
onready var _tree: SceneTree = get_tree()


func _on_LoadDialog_file_selected(path):
	Globals.TilesLoading = true
	Globals.TilesLoadPath = path
	_tree.ChangeScene("res://Game.tscn")


func _on_Load_pressed():
	LoadGame()


func _on_New_pressed():
	NewGame()


func _on_Quit_pressed():
	Quit()


func _ready():
	if Globals.TilesNumberFont == null:
		Globals.TilesNumberFont = get_theme_default_font()


func _unhandled_key_input(event):
	if event.is_action_pressed("quit"):
		accept_event()
		Quit()
	if event.is_action_pressed("load"):
		accept_event()
		LoadGame()
	if event.is_action_pressed("new"):
		accept_event()
		NewGame()
		

func LoadGame():
	if _load_dialog_used:
		_load_dialog.popup()
	else:
		_load_dialog_used = true
		_load_dialog.popup_centered()
	
	
func NewGame():
	_tree.change_scene("res://NewGame.tscn")
	

func Quit():
	_tree.quit()
