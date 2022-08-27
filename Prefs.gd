extends Control


onready var _auto_load: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/AutoLoad")
onready var _auto_save: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/AutoSave")
onready var _columns: SpinBox = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Columns")
onready var _hilite_blank: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/HiliteBlank")
onready var _hilite_blank_color: ColorPickerButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/HiliteBlankColor")
onready var _keep_aspect: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/KeepAspect")
onready var _numbers_color: ColorPickerButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/NumbersColor")
onready var _numbers_visible: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/NumbersVisible")
onready var _outlines_color: ColorPickerButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/OutlinesColor")
onready var _outlines_visible: CheckButton = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/OutlinesVisible")
onready var _rows: SpinBox = get_node("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Rows")
onready var _tree: SceneTree = get_tree()


func _ready() -> void:
	# Pass globals to controls
	_auto_load.pressed = Preferences.auto_load
	_auto_save.pressed = Preferences.auto_save
	_columns.value = Preferences.columns
	_keep_aspect.pressed = Preferences.keep_aspect
	_hilite_blank.pressed = Preferences.hilite_blank
	_hilite_blank_color.color = Preferences.hilite_blank_color
	_numbers_color.color = Preferences.numbers_color
	_numbers_visible.pressed = Preferences.numbers_visible
	_outlines_color.color = Preferences.outlines_color
	_outlines_visible.pressed = Preferences.outlines_visible
	_rows.value = Preferences.rows


func _on_cancel_pressed() -> void:
	quit()


func _on_save_pressed():
	# Update globals from controls
	var updates: int = 0
	if Preferences.auto_load != _auto_load.pressed:
		Preferences.auto_load = _auto_load.pressed
		updates += 1
	if Preferences.auto_save != _auto_save.pressed:
		Preferences.auto_save = _auto_save.pressed
		updates += 1
	if Preferences.columns != _columns.value:
		Preferences.columns = int(_columns.value)
		updates += 1
	if Preferences.keep_aspect != _keep_aspect.pressed:
		Preferences.keep_aspect = _keep_aspect.pressed
		updates += 1
	if Preferences.hilite_blank != _hilite_blank.pressed:
		Preferences.hilite_blank = _hilite_blank.pressed
		updates += 1
	if Preferences.hilite_blank_color != _hilite_blank_color.color:
		Preferences.hilite_blank_color = _hilite_blank_color.color
		updates += 1
	if Preferences.numbers_color != _numbers_color.color:
		Preferences.numbers_color = _numbers_color.color
		updates += 1
	if Preferences.numbers_visible != _numbers_visible.pressed:
		Preferences.numbers_visible = _numbers_visible.pressed
		updates += 1
	if Preferences.outlines_color != _outlines_color.color:
		Preferences.outlines_color = _outlines_color.color
		updates += 1
	if Preferences.outlines_visible != _outlines_visible.pressed:
		Preferences.outlines_visible = _outlines_visible.pressed
		updates += 1
	if Preferences.rows != _rows.value:
		Preferences.rows = int(_rows.value)
		updates += 1

	# Save settings if needed
	if updates > 0:
		Preferences.save(Preferences.P_PREFS)

	# Return to main menu
	quit()


func quit() -> void:
	var _r = _tree.change_scene("res://Main.tscn")
