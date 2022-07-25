extends Control


@onready var _hint_close: Button = get_node("HintDialog/CloseHint")
@onready var _hint_dialog: Window = get_node("HintDialog")
var _hint_dialog_used: bool = false
@onready var _hint_image: TextureRect = get_node("HintDialog/HintImage")
@onready var _game_board: VBoxContainer = get_node("GameBoard")
@onready var _load_dialog: FileDialog = get_node("LoadDialog")
var _load_dialog_used: bool = false
@onready var _moves: LineEdit = get_node("GameBoard/PanelContainer/HFlowContainer/Moves")
@onready var _save_dialog: FileDialog = get_node("SaveDialog")
var _save_dialog_used: bool = false
@onready var _show_numbers: CheckButton = get_node("GameBoard/PanelContainer/HFlowContainer/ShowNumbers")
@onready var _tiles: TilesControl = get_node("GameBoard/MarginContainer/Tiles")
@onready var _tree: SceneTree = get_tree()
@onready var _winner_close: Button = get_node("WinnerDialog/CloseWinner")
@onready var _winner_dialog: Window = get_node("WinnerDialog")
@onready var _winner_label: Label = get_node("WinnerDialog/CenterContainer/Label")


func _input(ev: InputEvent):
    if ev.is_action_pressed("quit"):
        accept_event()
        abort()
        return
    if ev.is_action_pressed("refresh"):
        accept_event()
        update()
        return


func _on_abort_pressed() -> void:
    abort()


func _on_hint_dialog_close_pressed() -> void:
    hide_hint()


func _on_hint_dialog_close_requested() -> void:
    hide_hint()


func _on_hint_dialog_size_changed() -> void:
    var ws: Vector2 = _hint_dialog.size
    var cs: Vector2 = _hint_close.size
    _hint_image.position = Vector2(10, 10)
    _hint_image.size = Vector2(ws.x - 20, ws.y - 25 - cs.y)
    _hint_close.position = Vector2((ws.x - cs.x) / 2, ws.y - cs.y - 10)


func _on_hint_pressed() -> void:
    _game_board.hide()
    if _hint_image.texture == null:
        var image: Image
        if Globals.TilesDefaultImage:
            image = load_image(Globals.TilesImageDefault)
        else:
            image = load_image(Globals.TilesImagePath)
        image.resize(_hint_dialog.size.x - 20 as int,
                _hint_dialog.size.y - 25 - _hint_close.size.y as int)
        var texture: ImageTexture = ImageTexture.create_from_image(image)
        _hint_image.texture = texture
    if _hint_dialog_used:
        _hint_dialog.popup()
    else:
        _hint_dialog_used = true
        _hint_dialog.popup_centered()


func _on_load_dialog_file_selected(path: String) -> void:
    _tiles.load_game(path)


func _on_load_pressed() -> void:
    _tiles.moves_enabled = false
    if _load_dialog_used:
        _load_dialog.popup()
    else:
        _load_dialog_used = true
        _load_dialog.popup_centered()


func _on_quit_pressed() -> void:
    quit()


func _on_save_dialog_file_selected(path: String) -> void:
    _tiles.save_game(path)


func _on_save_pressed() -> void:
    _tiles.moves_enabled = false
    if _save_dialog_used:
        _save_dialog.popup()
    else:
        _save_dialog_used = true
        _save_dialog.popup_centered()


func _on_show_numbers_pressed() -> void:
    _tiles.show_numbers = _show_numbers.button_pressed


func _on_tiles_item_rect_changed() -> void:
    if _tiles != null:
        _tiles.recalc_tiles()


func _on_tiles_moved(moves: int) -> void:
    _moves.text = str(moves)


func _on_tiles_won() -> void:
    _winner_label.position = Vector2(10, 10)
    _winner_label.size = Vector2(_winner_dialog.size.x - 20,
            _winner_dialog.size.y - 30 - _winner_close.size.y)
    _winner_close.position = Vector2((_winner_dialog.size.x - _winner_close.size.x) / 2,
            _winner_dialog.size.y - 10 - _winner_close.size.y)
    _winner_dialog.popup_centered()


func _on_winner_dialog_close_pressed() -> void:
    abort()


func _ready():
    # If loading a game, no need to pass globals on.
    _tiles.number_color = Globals.TilesNumberColor
    if Globals.TilesNumberFont != null:
        _tiles.number_font = Globals.TilesNumberFont
    _tiles.outline_color = Globals.TilesOutlineColor
    if Globals.TilesLoading:
        Globals.TilesLoading = false
        _tiles.load_game(Globals.TilesLoadPath)
        call_deferred("set_show_numbers")
    else:
        # Pass on global settings to tiles control.
        _tiles.columns = Globals.TilesColumns
        _tiles.rows = Globals.TilesRows
        if Globals.TilesUseImage:
            if Globals.TilesDefaultImage:
                _tiles.image_path = Globals.TilesImageDefault
            else:
                _tiles.image_path = Globals.TilesImagePath
        else:
            _tiles.image_path = ""
        _tiles.show_numbers = Globals.TilesShowNumbers
        _show_numbers.button_pressed = Globals.TilesShowNumbers
        _tiles.start()


func abort() -> void:
    _tree.change_scene("res://Main.tscn")


func hide_hint() -> void:
    _game_board.show()
    _tiles.moves_enabled = true
    _hint_dialog.hide()


func load_image(path: String) -> Image:
    if path.substr(0, 4) == "res:":
        return ResourceLoader.load(path, "Image")
    else:
        var i : Image = Image.new()
        i.load(path)
        return i


func quit() -> void:
    _tree.quit()


func set_show_numbers():
    _show_numbers.button_pressed = _tiles.show_numbers


