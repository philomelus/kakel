#include "Main.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/directory.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "function.hpp"

using namespace godot;

void Main::_bind_methods()
{
	FUNC_("Main::_bind_method");
		
	// API
	ClassDB::bind_method(D_METHOD("check_auto_start"), &Main::check_auto_start);
	ClassDB::bind_method(D_METHOD("on_new_pressed"), &Main::on_new_pressed);
	ClassDB::bind_method(D_METHOD("on_load_pressed"), &Main::on_load_pressed);
	ClassDB::bind_method(D_METHOD("on_options_pressed"), &Main::on_options_pressed);
	ClassDB::bind_method(D_METHOD("on_quit_pressed"), &Main::on_quit_pressed);
	ClassDB::bind_method(D_METHOD("on_loadDialog_fileSelected"), &Main::on_loadDialog_fileSelected);
}

Main::Main()
	: _loadDialogUsed(false)
{
	FUNC_("Main::Main");
	
	_centerContainer = memnew(CenterContainer);
	_vboxContainer = memnew(VBoxContainer);
	_label = memnew(Label);
	_load = memnew(Button);
	_loadDialog = memnew(FileDialog);
	_options = memnew(Button);
	_quit = memnew(Button);
	_start = memnew(Button);
}

Main::~Main()
{
	FUNC_("Main::~Main");
	
	memdelete(_centerContainer);
	memdelete(_vboxContainer);
	memdelete(_label);
	memdelete(_load);
	memdelete(_loadDialog);
	memdelete(_options);
	memdelete(_quit);
	memdelete(_start);
}

void Main::_ready()
{
	FUNC_("Main::_ready");

	_globals = get_node<Globals>("/root/Globals");
	_preferences = get_node<Preferences>("/root/Preferences");
	_tree = get_tree();

	ERR_FAIL_COND(_globals == nullptr);
	ERR_FAIL_COND(_preferences == nullptr);

	// Load default theme if needed
	Ref<Theme> theme = _globals->theme_get();
	if (theme == nullptr)
	{
		theme = Ref(ResourceLoader::get_singleton()->load("res://theme.tres", "Theme",
														  ResourceLoader::CacheMode::CACHE_MODE_IGNORE));
		_globals->theme_set(theme);
	}
		
	ERR_FAIL_COND(theme == nullptr);

	// Set our settings
	set_anchors_preset(Control::LayoutPreset::PRESET_FULL_RECT);
	set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	set_h_grow_direction(Control::GrowDirection::GROW_DIRECTION_BOTH);
	set_v_grow_direction(Control::GrowDirection::GROW_DIRECTION_BOTH);
			
	// Add center contianer
	add_child(_centerContainer);
			
	// Add v box container
	_centerContainer->add_child(_vboxContainer);

	// Add label
	_label->set_theme(theme);
	_label->set_text("KAKEL");
	_label->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
	_label->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	_label->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	_vboxContainer->add_child(_label);
			
	// Add new button
	_start->set_theme(theme);
	_start->set_tooltip("Start a new game.");
	_start->set_text("New");
	_vboxContainer->add_child(_start);
			
	// Add load button
	_load->set_theme(theme);
	_load->set_tooltip("Load a previously saved game.");
	_load->set_text("Load");
	_vboxContainer->add_child(_load);
			
	// Add preferences button
	_options->set_theme(theme);
	_options->set_tooltip("Change game settings.");
	_options->set_text("Options");
	_vboxContainer->add_child(_options);
			
	// Add quit button
	_quit->set_theme(theme);
	_quit->set_tooltip("Exit the game.");
	_quit->set_text("Quit");
	_vboxContainer->add_child(_quit);
			
	// Add load dialog
	_loadDialog->set_min_size(Vector2(480, 320));
	_loadDialog->set_title("Open A Game");
	_loadDialog->set_mode_overrides_title(false);
	_loadDialog->set_file_mode(FileDialog::FileMode::FILE_MODE_OPEN_FILE);
	_loadDialog->set_access(FileDialog::Access::ACCESS_FILESYSTEM);
	PackedStringArray filters;
	filters.append("*.kakel;Kakel Games");
	_loadDialog->set_filters(filters);
	add_child(_loadDialog);
		
	// Connect signals
	_start->connect("pressed", Callable(this, "on_new_pressed"));
	_load->connect("pressed", Callable(this, "on_load_pressed"));
	_options->connect("pressed", Callable(this, "on_options_pressed"));
	_quit->connect("pressed", Callable(this, "on_quit_pressed"));
	_loadDialog->connect("file_selected", Callable(this, "on_loadDialog_fileSelected"));

	// If an image was loaded previously, then default is no longer known
	if (_preferences->last_image_get().length() > 0)
		_globals->tiles_default_image_set(false);

	// Queue call to auto start
	call_deferred("check_auto_start");
}

void Main::_unhandled_input(const Ref<InputEvent>& ev)
{
	FUNCQ_("Main::_unhandled_input");
		
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
		options();
	}
}

void Main::check_auto_start()
{
	FUNC_("Main::check_auto_start");

	if (_preferences->auto_load_get())
	{
		Ref<Directory> d = memnew(Directory);
		const String autoPath = _preferences->auto_path_get();
		if (d->file_exists(_preferences->auto_path_get()))
		{
			UtilityFunctions::print("Main::check_auto_start: Loading game from \"{0}\"", autoPath);
			_globals->auto_started_set(true);
			load_game(autoPath);
		}
	}
}

void Main::load()
{
	FUNC_("Main::load");
		
	_globals->auto_started_set(false);
	if (_loadDialogUsed)
	{
		_loadDialog->popup();
	}
	else
	{
		_loadDialogUsed = true;
		if (_preferences->last_game_get().length() > 0)
			_loadDialog->set_current_path(_preferences->last_game_get());
		_loadDialog->popup_centered();
	}
}
	
void Main::load_game(const String path)
{
	FUNC_("Main::load_game");
		
	_globals->tiles_loading_set(true);
	_globals->tiles_load_path_set(path);
	_tree->change_scene("res://Game.tscn");
}
	
void Main::new_game()
{
	FUNC_("Main::new_game");
		
	_globals->auto_started_set(false);
	_tree->change_scene("res://NewGame.tscn");
}
	
void Main::on_load_pressed()
{
	FUNC_("Main::on_load_pressed");
		
	load();
}
	
void Main::on_loadDialog_fileSelected(const String path)
{
	FUNC_("Main::on_loadDialog_fileSelected");
		
	if (_preferences->last_game_get() != path)
	{
		_preferences->last_game_set(path);
		_preferences->save(_preferences->P_PREFS);
	}
	UtilityFunctions::print("Main::on_loadDialog_fileSelected:  Loading game from \"{0}\"", path);
	load_game(path);
}
	
void Main::on_new_pressed()
{
	FUNC_("Main::on_new_pressed");
	new_game();
}
	
void Main::on_options_pressed()
{
	FUNC_("Main::on_options_pressed");
	options();
}
	
void Main::on_quit_pressed()
{
	FUNC_("Main::on_quit_pressed");
	quit();
}
	
void Main::options()
{
	FUNC_("Main::prefs");
	_tree->change_scene("res://Prefs.tscn");
}
	
void Main::quit()
{
	FUNC_("Main::quit");
	_tree->quit();
}

