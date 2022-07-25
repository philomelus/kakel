extends Control


@onready var _load_dialog = get_node("LoadDialog")
var _load_dialog_used = false
@onready var _tree = get_tree()


func _on_load_dialog_file_selected(path: String) -> void:
    Globals.TilesLoading = true
    Globals.TilesLoadPath = path
    _tree.change_scene("res://Game.tscn")


func _on_load_pressed() -> void:
    load_game()


func _on_new_pressed() -> void:
    new_game()


func _on_quit_pressed() -> void:
    quit()


func _ready() -> void:
    if Globals.TilesNumberFont == null:
        Globals.TilesNumberFont = get_theme_default_font()


func _unhandled_key_input(event):
    if event.is_action_pressed("quit"):
        accept_event()
        quit()
    if event.is_action_pressed("load"):
        accept_event()
        load_game()
    if event.is_action_pressed("new"):
        accept_event()
        new_game()


func load_game() -> void:
    if _load_dialog_used:
        _load_dialog.popup()
    else:
        _load_dialog_used = true
        _load_dialog.popup_centered()


func new_game() -> void:
    _tree.change_scene("res://NewGame.tscn")


func quit() -> void:
    _tree.quit()


