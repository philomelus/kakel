#include "Main.hpp"
#include <Button.hpp>
#include <CenterContainer.hpp>
#include <Directory.hpp>
#include <FileDialog.hpp>
#include <GlobalConstants.hpp>
#include <Label.hpp>
#include <SceneTree.hpp>
#include <VBoxContainer.hpp>
#include <ResourceLoader.hpp>
#include "auto_free.hpp"

using namespace godot;

namespace godot
{
	void Main::_register_methods()
	{
		// API
		register_method("_ready", &Main::_ready);
		register_method("_unhandled_input", &Main::_unhandled_input);
		register_method("on_new_pressed", &Main::on_new_pressed);
		register_method("on_load_pressed", &Main::on_load_pressed);
		register_method("on_prefs_pressed", &Main::on_prefs_pressed);
		register_method("on_quit_pressed", &Main::on_quit_pressed);
		register_method("on_loadDialog_fileSelected", &Main::on_loadDialog_fileSelected);
	}

	Main::Main()
		: _loadDialogUsed(false)
	{
	}

	Main::~Main()
	{
	}

	void Main::_init()
	{
		// Create controls
		_centerContainer = CenterContainer::_new();
		_vboxContainer = VBoxContainer::_new();
		_label = Label::_new();
		_load = Button::_new();
		_loadDialog = FileDialog::_new();
		_prefs = Button::_new();
		_quit = Button::_new();
		_start = Button::_new();
	}

	void Main::_ready()
	{
		_globals = get_node<Globals>("/root/Globals");
		godot::Godot::print(String("Main::_globals = {0}"), _globals);
		_tree = get_tree();

		ERR_FAIL_COND(_globals == nullptr);
		
		Ref<Theme> theme = _globals->theme_get();

		// Set our settings
		set_anchors_preset(Control::LayoutPreset::PRESET_WIDE);
		set_margin(GlobalConstants::MARGIN_LEFT, 0);
		set_margin(GlobalConstants::MARGIN_RIGHT, 0);
		set_margin(GlobalConstants::MARGIN_TOP, 0);
		set_margin(GlobalConstants::MARGIN_BOTTOM, 0);
		set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		set_h_grow_direction(Control::GrowDirection::GROW_DIRECTION_BOTH);
		set_v_grow_direction(Control::GrowDirection::GROW_DIRECTION_BOTH);
			
		// Add center contianer
		add_child(_centerContainer);
		_centerContainer->set_owner(this);
		_centerContainer->set_margin(GlobalConstants::MARGIN_LEFT, 7);
		_centerContainer->set_margin(GlobalConstants::MARGIN_RIGHT, 1273);
		_centerContainer->set_margin(GlobalConstants::MARGIN_TOP, 7);
		_centerContainer->set_margin(GlobalConstants::MARGIN_BOTTOM, 713);
			
		// Add v box container
		_centerContainer->add_child(_vboxContainer);
		_vboxContainer->set_owner(_centerContainer);
		_vboxContainer->set_margin(GlobalConstants::MARGIN_LEFT, 561);
		_vboxContainer->set_margin(GlobalConstants::MARGIN_RIGHT, 704);
		_vboxContainer->set_margin(GlobalConstants::MARGIN_TOP, 209);
		_vboxContainer->set_margin(GlobalConstants::MARGIN_BOTTOM, 496);

		// Add label
		_vboxContainer->add_child(_label);
		_label->set_owner(_vboxContainer);
		_label->set_theme(theme);
		_label->set_text("KAKEL");
		_label->set_align(Label::Align::ALIGN_CENTER);
		_label->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		_label->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		_label->set_margin(GlobalConstants::MARGIN_RIGHT, 143);
		_label->set_margin(GlobalConstants::MARGIN_BOTTOM, 67);
			
		// Add new button
		_vboxContainer->add_child(_start);
		_start->set_owner(_vboxContainer);
		_start->set_theme(theme);
		_start->set_margin(GlobalConstants::MARGIN_TOP, 71);
		_start->set_margin(GlobalConstants::MARGIN_RIGHT, 143);
		_start->set_margin(GlobalConstants::MARGIN_BOTTOM, 122);
		_start->set_tooltip("Start a new game.");
		_start->set_text("New");
			
		// Add load button
		_vboxContainer->add_child(_load);
		_load->set_owner(_vboxContainer);
		_load->set_theme(theme);
		_load->set_margin(GlobalConstants::MARGIN_TOP, 126);
		_load->set_margin(GlobalConstants::MARGIN_RIGHT, 143);
		_load->set_margin(GlobalConstants::MARGIN_BOTTOM, 177);
		_load->set_tooltip("Load a previously saved game.");
		_load->set_text("Load");
			
		// Add preferences button
		_vboxContainer->add_child(_prefs);
		_prefs->set_owner(_vboxContainer);
		_prefs->set_theme(theme);
		_prefs->set_margin(GlobalConstants::MARGIN_TOP, 181);
		_prefs->set_margin(GlobalConstants::MARGIN_RIGHT, 143);
		_prefs->set_margin(GlobalConstants::MARGIN_BOTTOM, 232);
		_prefs->set_tooltip("Change game settings.");
		_prefs->set_text("Options");
			
		// Add quit button
		_vboxContainer->add_child(_quit);
		_quit->set_owner(_vboxContainer);
		_quit->set_theme(theme);
		_quit->set_margin(GlobalConstants::MARGIN_TOP, 236);
		_quit->set_margin(GlobalConstants::MARGIN_RIGHT, 143);
		_quit->set_margin(GlobalConstants::MARGIN_BOTTOM, 287);
		_quit->set_tooltip("Exit the game.");
		_quit->set_text("Quit");
			
		// Add load dialog
		add_child(_loadDialog);
		_loadDialog->set_owner(_vboxContainer);
		_loadDialog->set_pause_mode(Node::PauseMode::PAUSE_MODE_PROCESS);
		_loadDialog->set_margin(GlobalConstants::MARGIN_RIGHT, 316);
		_loadDialog->set_margin(GlobalConstants::MARGIN_BOTTOM, 149);
		_loadDialog->set_custom_minimum_size(Vector2(480, 320));
		_loadDialog->set_exclusive(true);
		_loadDialog->set_title("Open A Game");
		_loadDialog->set_resizable(true);
		_loadDialog->set_mode_overrides_title(false);
		_loadDialog->set_mode(FileDialog::Mode::MODE_OPEN_FILE);
		_loadDialog->set_access(FileDialog::Access::ACCESS_FILESYSTEM);
		PoolStringArray filters;
		filters.append("*.kakel;Kakel Games");
		_loadDialog->set_filters(filters);
		
		// Connect signals
		_start->connect("pressed", this, "on_new_pressed");
		_load->connect("pressed", this, "on_load_pressed");
		_prefs->connect("pressed", this, "on_prefs_pressed");
		_quit->connect("pressed", this, "on_quit_pressed");
		_loadDialog->connect("file_selected", this, "on_loadDialog_fileSelected");

		// If an image was loaded previously, then default is no longer known
		if (_globals->preferences_get()->last_image_get().length() > 0)
			_globals->tiles_default_image_set(false);

		// Queue call to auto start
		call_deferred("check_auto_start");
	}

	void Main::_unhandled_input(Ref<InputEvent> ev)
	{
		if (ev->is_action_pressed("quit"))
		{
			accept_event();
			quit();
		}
		else if (ev->is_action_pressed("new"))
		{
			accept_event();
			new_game();
		}
		else if (ev->is_action_pressed("load"))
		{
			accept_event();
			load();
		}
		else if (ev->is_action_pressed("prefs"))
		{
			accept_event();
			prefs();
		}
	}
	
	void Main::check_auto_start()
	{
		if (_globals->preferences_get()->auto_load_get())
		{
			auto_free<Directory> d(Directory::_new());
			if (d->file_exists(_globals->preferences_get()->auto_path_get()))
			{
				_globals->auto_started_set(true);
				load_game(_globals->preferences_get()->auto_path_get());
			}
		}
	}
	
	void Main::load()
	{
		_globals->auto_started_set(false);
		if (_loadDialogUsed)
		{
			_loadDialog->popup();
		}
		else
		{
			_loadDialogUsed = true;
			if (_globals->preferences_get()->last_game_get().length() > 0)
				_loadDialog->set_current_path(_globals->preferences_get()->last_game_get());
			_loadDialog->popup_centered();
		}
	}
	
	void Main::load_game(const String path)
	{
		_globals->tiles_loading_set(true);
		_globals->tiles_load_path_set(path);
		_tree->change_scene("res://Game.tscn");
	}
	
	void Main::new_game()
	{
		_globals->auto_started_set(false);
		_tree->change_scene("res://NewGame.tscn");
	}
	
	void Main::on_load_pressed()
	{
		load();
	}
	
	void Main::on_loadDialog_fileSelected(const String path)
	{
		if (_globals->preferences_get()->last_game_get() != path)
		{
			_globals->preferences_get()->last_game_set(path);
			_globals->preferences_get()->save(_globals->preferences_get()->P_PREFS);
		}
		load_game(path);
	}
	
	void Main::on_new_pressed()
	{
		new_game();
	}
	
	void Main::on_prefs_pressed()
	{
		prefs();
	}
	
	void Main::on_quit_pressed()
	{
		quit();
	}
	
	void Main::prefs()
	{
		_tree->change_scene("res://Prefs.tscn");
	}
	
	void Main::quit()
	{
		_tree->quit();
	}
}
