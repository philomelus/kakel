extends Control


@onready var _load_dialog: FileDialog = get_node("LoadDialog")
var _load_dialog_used: bool = false;
@onready var _tree: SceneTree = get_tree()


func _ready() -> void:
    # Load or initialize preferences
    Globals.preferences.load_(Preferences.P_PREFS)

    # If an image has been loaded before, then default image is gone
    Globals.tiles_default_image = true if Globals.preferences.last_image == "" else false

    # Queue call to auto start if desired
    call_deferred("check_auto_start")


func _unhandled_input(ev: InputEvent) -> void:
    if ev.is_action_pressed("quit"):
        accept_event()
        quit()
    elif ev.is_action_pressed("new"):
        accept_event()
        new_game()
    elif ev.is_action_pressed("load"):
        accept_event()
        load_()
    elif ev.is_action_pressed("prefs"):
        accept_event()
        prefs()


func _on_load_dialog_file_selected(path: String) -> void:
    # Save path if different than last loaded game
    if Globals.preferences.last_game != path:
        Globals.preferences.last_game = path
        Globals.preferences.save(Preferences.P_PREFS)
    load_game(path)


func _on_load_pressed():
    load_()


func _on_new_pressed():
    new_game()


func _on_prefs_pressed():
    prefs()


func _on_quit_pressed():
    quit()


func check_auto_start() -> void:
    if Globals.preferences.auto_load and not Globals.auto_started:
        var d: Directory = Directory.new()
        if d.file_exists(Globals.preferences.auto_path):
            Globals.auto_started = true
            load_game(Globals.preferences.auto_path)


func load_() -> void:
    Globals.auto_started = false
    if _load_dialog_used:
        _load_dialog.popup()
    else:
        _load_dialog_used = true
        if Globals.preferences.last_game != "":
            _load_dialog.current_path = Globals.preferences.last_game
        _load_dialog.popup_centered()


func load_game(path: String) -> void:
    Globals.tiles_loading = true
    Globals.tiles_load_path = path
    _tree.change_scene("res://Game.tscn")


func new_game() -> void:
    Globals.auto_started = false
    _tree.change_scene("res://NewGame.tscn")


func prefs() -> void:
    _tree.change_scene("res://Prefs.tscn")


func quit() -> void:
    _tree.quit()
