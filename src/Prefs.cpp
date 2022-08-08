#include "Prefs.hpp"
#include <GlobalConstants.hpp>
#include <SceneTree.hpp>
#include "auto_free.hpp"

using namespace godot;

namespace godot
{
	// TODO:  There is no cleanup of controls.  This will cause memory leaks
	// since the controls are recreated everytime the prefs scene is shown!
	// Where is the correct place to do this, though?  There is no _uninit.
	
	void Prefs::_register_methods()
	{
		// API
		register_method("_ready", &Prefs::_ready);
		register_method("on_cancel_pressed", &Prefs::on_cancel_pressed);
		register_method("on_save_pressed", &Prefs::on_save_pressed);
		register_method("quit", &Prefs::quit);
	}

	Prefs::Prefs()
	{
	}

	Prefs::~Prefs()
	{
	}

	void Prefs::_init()
	{
		_autoLoad = CheckButton::_new();
		_autoLoadLabel = Label::_new();
		_autoSave = CheckButton::_new();
		_autoSaveLabel = Label::_new();
		_cancel = Button::_new();
		_centerContainer = CenterContainer::_new();
		_columns = SpinBox::_new();
		_columnsLabel = Label::_new();
		_gridContainer = GridContainer::_new();
		_hboxContainer = HBoxContainer::_new();
		_marginContainer = MarginContainer::_new();
		_numbersColor = ColorPickerButton::_new();
		_numbersColorLabel = Label::_new();
		_numbersVisible = CheckButton::_new();
		_numbersVisibleLabel = Label::_new();
		_outlinesColor = ColorPickerButton::_new();
		_outlinesColorLabel = Label::_new();
		_outlinesVisible = CheckButton::_new();
		_outlinesVisibleLabel = Label::_new();
		_rows = SpinBox::_new();
		_rowsLabel = Label::_new();
		_save = Button::_new();
		_vboxContainer = VBoxContainer::_new();
	}

	void Prefs::_ready()
	{
		_globals = get_node<Globals>("/root/Globals");
		_tree = get_tree();

		ERR_FAIL_COND(_globals == nullptr);

		Ref<Theme> theme = _globals->theme_get();

		// Set out PanelContainer settings
		set_anchors_preset(Control::LayoutPreset::PRESET_WIDE);
		set_margin(GlobalConstants::MARGIN_LEFT, 0);
		set_margin(GlobalConstants::MARGIN_RIGHT, 0);
		set_margin(GlobalConstants::MARGIN_TOP, 0);
		set_margin(GlobalConstants::MARGIN_BOTTOM, 0);
		set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		set_h_grow_direction(Control::GrowDirection::GROW_DIRECTION_BOTH);
		set_v_grow_direction(Control::GrowDirection::GROW_DIRECTION_BOTH);
		
		// Add center contqiner
		add_child(_centerContainer);
		_centerContainer->set_margin(GlobalConstants::MARGIN_LEFT, 7);
		_centerContainer->set_margin(GlobalConstants::MARGIN_TOP, 7);
		_centerContainer->set_margin(GlobalConstants::MARGIN_RIGHT, 1273);
		_centerContainer->set_margin(GlobalConstants::MARGIN_BOTTOM, 713);

		// Add v box container
		_centerContainer->add_child(_vboxContainer);
		_vboxContainer->set_theme(theme);
		
		// Add grid container
		_vboxContainer->add_child(_gridContainer);
		_gridContainer->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER
										 + Control::SizeFlags::SIZE_FILL);
		_gridContainer->set_columns(2);
		
		// Add columns label
		_gridContainer->add_child(_columnsLabel);
		_columnsLabel->set_text("Columns");
		_columnsLabel->set_tooltip("Number of columns in tile grid.");
		_columnsLabel->set_mouse_filter(0);

		// Add columns
		_gridContainer->add_child(_columns);
 		_columns->set_min(3);
		_columns->set_max(99);
		_columns->set_align(LineEdit::Align::ALIGN_CENTER);
		
		// Add rows label
		_gridContainer->add_child(_rowsLabel);
		_rowsLabel->set_text("Rows");
		_rowsLabel->set_tooltip("Number of rows in tile grid.");
		_rowsLabel->set_mouse_filter(0);

		// Add rows
		_gridContainer->add_child(_rows);
		_rows->set_min(3);
		_rows->set_max(99);
		_rows->set_align(LineEdit::Align::ALIGN_CENTER);

		// Add outlines visible label
		_gridContainer->add_child(_outlinesVisibleLabel);
		_outlinesVisibleLabel->set_text("Show Outlines");
		_outlinesVisibleLabel->set_tooltip("When enabled, tiles are outlined.");
		_outlinesVisibleLabel->set_mouse_filter(0);

		// Add outlines visible
		_gridContainer->add_child(_outlinesVisible);
		_outlinesVisible->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
		
		// Add outlines color label
		_gridContainer->add_child(_outlinesColorLabel);
		_outlinesColorLabel->set_text("Outlines Color");
		_outlinesColorLabel->set_tooltip("Outlines are drawn in this color.");
		_outlinesColorLabel->set_mouse_filter(0);

		// Add outlines color
		_gridContainer->add_child(_outlinesColor);

		// Add numbers visible label
		_gridContainer->add_child(_numbersVisibleLabel);
		_numbersVisibleLabel->set_text("Show Numbers");
		_numbersVisibleLabel->set_tooltip("When enabled, tile numbers are shown in upper left of each tile.");
		_numbersVisibleLabel->set_mouse_filter(0);

		// Add numbers visible
		_gridContainer->add_child(_numbersVisible);
		_numbersVisible->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);

		// Add numbers color label
		_gridContainer->add_child(_numbersColorLabel);
		_numbersColorLabel->set_text("Numbers Color");
		_numbersColorLabel->set_tooltip("Tile numbers are drawn in this color.");
		_numbersColorLabel->set_mouse_filter(0);
		
		// Add numbers color
		_gridContainer->add_child(_numbersColor);

		// Add auto save label
		_gridContainer->add_child(_autoSaveLabel);
		_autoSaveLabel->set_text("Auto Save on Exit");
		_autoSaveLabel->set_tooltip("If \"Exit Kakel\" button is used, automatically save the current game before exit.");
		_autoSaveLabel->set_mouse_filter(0);

		// Add auto save
		_gridContainer->add_child(_autoSave);
		_autoSave->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);

		// Add auto load label
		_gridContainer->add_child(_autoLoadLabel);
		_autoLoadLabel->set_text("Auto Load on Start");
		_autoLoadLabel->set_tooltip("If previous automatically save game from exit\nexists, automatically load that garm on launch.");
		_autoLoadLabel->set_mouse_filter(0);

		// Add auto load
		_gridContainer->add_child(_autoLoad);
		_autoLoad->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);

		// Add margin container
		_vboxContainer->add_child(_marginContainer);

		// Add h box container
		_marginContainer->add_child(_hboxContainer);
		_hboxContainer->set_alignment(BoxContainer::AlignMode::ALIGN_CENTER);
		
		// Add cancel button
		_hboxContainer->add_child(_cancel);
		_cancel->set_text("Cancel");
		
		// Add save button
		_hboxContainer->add_child(_save);
		_save->set_text("Save");
		
		// Signals
		_cancel->connect("pressed", this, "on_cancel_pressed");
		_save->connect("pressed", this, "on_save_pressed");

		// Copy globals to controls
        _autoLoad->set_pressed(_globals->preferences_get()->auto_load_get());
        _autoSave->set_pressed(_globals->preferences_get()->auto_save_get());
        _columns->set_value(_globals->preferences_get()->columns_get());
        _numbersColor->set_pick_color(_globals->preferences_get()->numbers_color_get());
        _numbersVisible->set_pressed(_globals->preferences_get()->numbers_visible_get());
        _outlinesColor->set_pick_color(_globals->preferences_get()->outlines_color_get());
        _outlinesVisible->set_pressed(_globals->preferences_get()->outlines_visible_get());
        _rows->set_value(_globals->preferences_get()->rows_get());
	}

	void Prefs::on_cancel_pressed()
	{
		// Return to main menu
		quit();
	}
	
	void Prefs::on_save_pressed()
	{
        // Update globals from controls
        int updates = 0;
        if (_globals->preferences_get()->auto_load_get() != _autoLoad->is_pressed())
        {
            _globals->preferences_get()->auto_load_set(_autoLoad->is_pressed());
            ++updates;
        }
        if (_globals->preferences_get()->auto_save_get() != _autoSave->is_pressed())
        {
            _globals->preferences_get()->auto_save_set(_autoSave->is_pressed());
            ++updates;
        }
        if (_globals->preferences_get()->columns_get() != _columns->get_value())
        {
            _globals->preferences_get()->columns_set(_columns->get_value());
            ++updates;
        }
        if (_globals->preferences_get()->numbers_color_get() != _numbersColor->get_pick_color())
        {
            _globals->preferences_get()->numbers_color_set(_numbersColor->get_pick_color());
            ++updates;
        }
        if (_globals->preferences_get()->outlines_color_get() != _outlinesColor->get_pick_color())
        {
            _globals->preferences_get()->outlines_color_set(_outlinesColor->get_pick_color());
            ++updates;
        }
        if (_globals->preferences_get()->rows_get() != _rows->get_value())
        {
            _globals->preferences_get()->rows_set(_rows->get_value());
            ++updates;
        }
        if (_globals->preferences_get()->numbers_visible_get() != _numbersVisible->is_pressed())
        {
            _globals->preferences_get()->numbers_visible_set(_numbersVisible->is_pressed());
            ++updates;
        }
        if (_globals->preferences_get()->outlines_visible_get() != _outlinesVisible->is_pressed())
        {
            _globals->preferences_get()->outlines_visible_set(_outlinesVisible->is_pressed());
            ++updates;
        }

        // Save settings if needed
        if (updates > 0)
            _globals->preferences_get()->save(_globals->preferences_get()->P_PREFS);

		// Return to main menu
		quit();
	}
	
	void Prefs::quit()
	{
		_tree->change_scene("res://Main.tscn");
	}
}
