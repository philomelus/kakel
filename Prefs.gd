extends Control


@onready var _auto_load: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/AutoLoad")
@onready var _auto_save: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/AutoSave")
@onready var _columns: SpinBox = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Columns")
@onready var _outline_color: ColorPickerButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/OutlineColor")
@onready var _number_color: ColorPickerButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/NumberColor")
@onready var _rows: SpinBox = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Rows")
@onready var _show_numbers: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/ShowNumbers")
@onready var _show_outlines: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/ShowOutlines")
@onready var _tree: SceneTree = get_tree()


func _ready() -> void:
	# Pass globals to controls
	_auto_load.button_pressed = Globals.preferences.auto_load
	_auto_save.button_pressed = Globals.preferences.auto_save
	_columns.value = Globals.preferences.columns
	_number_color.color = Globals.preferences.number_color
	_outline_color.color = Globals.preferences.outline_color
	_rows.value = Globals.preferences.rows
	_show_numbers.button_pressed = Globals.preferences.show_numbers
	_show_outlines.button_pressed = Globals.preferences.show_outlines


func _on_cancel_pressed() -> void:
	quit()


func _on_save_pressed():
	# Update globals from controls
	var updates: int = 0
	if Globals.preferences.auto_load != _auto_load.button_pressed:
		Globals.preferences.auto_load = _auto_load.button_pressed
		updates += 1
	if Globals.preferences.auto_save != _auto_save.button_pressed:
		Globals.preferences.auto_save = _auto_save.button_pressed
		updates += 1
	if Globals.preferences.columns != _columns.value:
		Globals.preferences.columns = _columns.value as int
		updates += 1
	if Globals.preferences.number_color != _number_color.color:
		Globals.preferences.number_color = _number_color.color
		updates += 1
	if Globals.preferences.outline_color != _outline_color.color:
		Globals.preferences.outline_color = _outline_color.color
		updates += 1
	if Globals.preferences.rows != _rows.value:
		Globals.preferences.rows = _rows.value as int
		updates += 1
	if Globals.preferences.show_numbers != _show_numbers.button_pressed:
		Globals.preferences.show_numbers = _show_numbers.button_pressed
		updates += 1
	if Globals.preferences.show_outlines != _show_outlines.button_pressed:
		Globals.preferences.show_outlines = _show_outlines.button_pressed
		updates += 1

	# Save settings if needed
	if updates > 0:
		Globals.preferences.save(Preferences.P_PREFS)

	# Return to main menu
	quit()


func quit() -> void:
	_tree.change_scene_to_file("res://Main.tscn")
