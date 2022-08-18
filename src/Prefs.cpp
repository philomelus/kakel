#include "Prefs.hpp"
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include "function.hpp"

using namespace godot;

void Prefs::_bind_methods()
{
	FUNC_("Prefs::_bind_methods");
		
	// API
	ClassDB::bind_method(D_METHOD("on_cancel_pressed"), &Prefs::on_cancel_pressed);
	ClassDB::bind_method(D_METHOD("on_save_pressed"), &Prefs::on_save_pressed);
	ClassDB::bind_method(D_METHOD("quit"), &Prefs::quit);
}

Prefs::Prefs()
{
	FUNC_("Prefs::Prefs");
}

Prefs::~Prefs()
{
	FUNC_("Prefs::~Prefs");
}

void Prefs::_ready()
{
	FUNC_("Prefs::_ready");
		
	_globals = get_node<AppGlobals>("/root/Globals");
	ERR_FAIL_COND(_globals == nullptr);

	_preferences = get_node<AppPreferences>("/root/Preferences");
	ERR_FAIL_COND(_preferences == nullptr);

	_autoLoad = get_node<CheckButton>("CenterContainer/VBoxContainer/GridContainer/AutoLoad");
	ERR_FAIL_COND(_autoLoad == nullptr);

	_autoSave = get_node<CheckButton>("CenterContainer/VBoxContainer/GridContainer/AutoSave");
	ERR_FAIL_COND(_autoSave == nullptr);

	_cancel = get_node<Button>("CenterContainer/VBoxContainer/MarginContainer/HBoxContainer/Cancel");
	ERR_FAIL_COND(_cancel == nullptr);
	
	_columns = get_node<SpinBox>("CenterContainer/VBoxContainer/GridContainer/Columns");
	ERR_FAIL_COND(_columns == nullptr);

	_hiliteBlank = get_node<CheckButton>("CenterContainer/VBoxContainer/GridContainer/HiliteBlank");
	ERR_FAIL_COND(_hiliteBlank == nullptr);

	_hiliteBlankColor = get_node<ColorPickerButton>("CenterContainer/VBoxContainer/GridContainer/HiliteBlankColor");
	ERR_FAIL_COND(_hiliteBlankColor == nullptr);

	_keepAspect = get_node<CheckButton>("CenterContainer/VBoxContainer/GridContainer/KeepAspect");
	ERR_FAIL_COND(_keepAspect == nullptr);

	_numbersColor = get_node<ColorPickerButton>("CenterContainer/VBoxContainer/GridContainer/NumbersColor");
	ERR_FAIL_COND(_numbersColor == nullptr);

	_numbersVisible = get_node<CheckButton>("CenterContainer/VBoxContainer/GridContainer/NumbersVisible");
	ERR_FAIL_COND(_numbersVisible == nullptr);

	_outlinesColor = get_node<ColorPickerButton>("CenterContainer/VBoxContainer/GridContainer/OutlinesColor");
	ERR_FAIL_COND(_outlinesColor == nullptr);

	_outlinesVisible = get_node<CheckButton>("CenterContainer/VBoxContainer/GridContainer/OutlinesVisible");
	ERR_FAIL_COND(_outlinesVisible == nullptr);

	_rows = get_node<SpinBox>("CenterContainer/VBoxContainer/GridContainer/Rows");
	ERR_FAIL_COND(_autoLoad == nullptr);

	_save = get_node<Button>("CenterContainer/VBoxContainer/MarginContainer/HBoxContainer/Save");
	ERR_FAIL_COND(_save == nullptr);
	
	_tree = get_tree();

    // Signals
    _cancel->connect("pressed", Callable(this, "on_cancel_pressed"));
    _save->connect("pressed", Callable(this, "on_save_pressed"));

	// Copy globals to controls
	_autoLoad->set_pressed(_preferences->auto_load_get());
	_autoSave->set_pressed(_preferences->auto_save_get());
	_columns->set_value(_preferences->columns_get());
	_hiliteBlank->set_pressed(_preferences->hilite_blank_get());
	_hiliteBlankColor->set_pick_color(_preferences->hilite_blank_color_get());
	_keepAspect->set_pressed(_preferences->keep_aspect_get());
	_numbersColor->set_pick_color(_preferences->numbers_color_get());
	_numbersVisible->set_pressed(_preferences->numbers_visible_get());
	_outlinesColor->set_pick_color(_preferences->outlines_color_get());
	_outlinesVisible->set_pressed(_preferences->outlines_visible_get());
	_rows->set_value(_preferences->rows_get());
}

void Prefs::on_cancel_pressed()
{
	FUNC_("Prefs::on_cancel_pressed");
	// Return to main menu
	quit();
}
	
void Prefs::on_save_pressed()
{
	FUNC_("Prefs::on_save_pressed");
		
	// Update globals from controls
	int updates = 0;
	if (_preferences->auto_load_get() != _autoLoad->is_pressed())
	{
		_preferences->auto_load_set(_autoLoad->is_pressed());
		++updates;
	}
	if (_preferences->auto_save_get() != _autoSave->is_pressed())
	{
		_preferences->auto_save_set(_autoSave->is_pressed());
		++updates;
	}
	if (_preferences->columns_get() != _columns->get_value())
	{
		_preferences->columns_set(_columns->get_value());
		++updates;
	}
	if (_preferences->hilite_blank_get() != _hiliteBlank->is_pressed())
	{
		_preferences->hilite_blank_set(_hiliteBlank->is_pressed());
		++updates;
	}
	if (_preferences->hilite_blank_color_get() != _hiliteBlankColor->get_pick_color())
	{
		_preferences->hilite_blank_color_set(_hiliteBlankColor->get_pick_color());
		++updates;
	}
	if (_preferences->keep_aspect_get() != _keepAspect->is_pressed())
	{
		_preferences->keep_aspect_set(_keepAspect->is_pressed());
		++updates;
	}
	if (_preferences->numbers_color_get() != _numbersColor->get_pick_color())
	{
		_preferences->numbers_color_set(_numbersColor->get_pick_color());
		++updates;
	}
	if (_preferences->outlines_color_get() != _outlinesColor->get_pick_color())
	{
		_preferences->outlines_color_set(_outlinesColor->get_pick_color());
		++updates;
	}
	if (_preferences->rows_get() != _rows->get_value())
	{
		_preferences->rows_set(_rows->get_value());
		++updates;
	}
	if (_preferences->numbers_visible_get() != _numbersVisible->is_pressed())
	{
		_preferences->numbers_visible_set(_numbersVisible->is_pressed());
		++updates;
	}
	if (_preferences->outlines_visible_get() != _outlinesVisible->is_pressed())
	{
		_preferences->outlines_visible_set(_outlinesVisible->is_pressed());
		++updates;
	}

	// Save settings if needed
	if (updates > 0)
		_preferences->save(_preferences->P_PREFS);

	// Return to main menu
	quit();
}
	
void Prefs::quit()
{
	FUNC_("Prefs::quit");
		
	_tree->change_scene("res://Main.tscn");
}

