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
#include "function.hpp"

using namespace godot;

namespace godot
{
	void Main::_register_methods()
	{
		FUNC_("Main::_register_method");
		
		// API
		register_method("_ready", &Main::_ready);
		register_method("_unhandled_input", &Main::_unhandled_input);
		register_method("check_auto_start", &Main::check_auto_start);
		register_method("on_new_pressed", &Main::on_new_pressed);
		register_method("on_load_pressed", &Main::on_load_pressed);
		register_method("on_options_pressed", &Main::on_options_pressed);
		register_method("on_quit_pressed", &Main::on_quit_pressed);
		register_method("on_loadDialog_fileSelected", &Main::on_loadDialog_fileSelected);
	}

	Main::Main()
		: _loadDialogUsed(false)
	{
		FUNC_("Main::Main");
	}

	Main::~Main()
	{
		FUNC_("Main::~Main");
	}

	void Main::_init()
	{
		FUNC_("Main::_init");
	}

	void Main::_ready()
	{
		FUNC_("Main::_ready");
		
		_globals = get_node<AppGlobals>("/root/Globals");
		ERR_FAIL_COND(_globals == nullptr);
		
		_preferences = get_node<AppPreferences>("/root/Preferences");
		ERR_FAIL_COND(_preferences == nullptr);
		
		_tree = get_tree();
		ERR_FAIL_COND(_tree == nullptr);

		_load = get_node<Button>("CenterContainer/VBoxContainer/Load");
		ERR_FAIL_COND(_load == nullptr);
		
		_loadDialog = get_node<FileDialog>("LoadDialog");
		ERR_FAIL_COND(_loadDialog == nullptr);
		
		_options = get_node<Button>("CenterContainer/VBoxContainer/Options");
		ERR_FAIL_COND(_options == nullptr);
		
		_quit = get_node<Button>("CenterContainer/VBoxContainer/Quit");
		ERR_FAIL_COND(_quit == nullptr);
		
		_start = get_node<Button>("CenterContainer/VBoxContainer/Start");
		ERR_FAIL_COND(_start == nullptr);

		// Load default theme if needed
		Ref<Theme> theme = _globals->theme_get();
		if (theme == nullptr)
		{
			theme = Ref(ResourceLoader::get_singleton()->load(_preferences->default_theme_get(), "Theme"));
			_globals->theme_set(theme);
		}
		ERR_FAIL_COND(theme == nullptr);

		
		// Connect signals
		_start->connect("pressed", this, "on_new_pressed");
		_load->connect("pressed", this, "on_load_pressed");
		_options->connect("pressed", this, "on_options_pressed");
		_quit->connect("pressed", this, "on_quit_pressed");
		_loadDialog->connect("file_selected", this, "on_loadDialog_fileSelected");

		// If an image was loaded previously, then default is no longer known
		if (_preferences->last_image_get().length() > 0)
			_globals->tiles_default_image_set(false);

		// Queue call to auto start
		if (_globals->tiles_quit_get() == false)
			call_deferred("check_auto_start");

		// Import preferences once
		if (_globals->preferences_imported_get() == false)
		{
			_globals->tiles_hilite_blank_set(_preferences->hilite_blank_get());
			_globals->tiles_keep_aspect_set(_preferences->keep_aspect_get());
			_globals->preferences_imported_set(true);
		}
	}

	void Main::_unhandled_input(Ref<InputEvent> ev)
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
			auto_free<Directory> d(Directory::_new());
			const String autoPath = _preferences->auto_path_get();
			if (d->file_exists(_preferences->auto_path_get()))
			{
				FUNCP_("Main::check_auto_start: Loading game from \"{0}\"", autoPath);
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
		FUNCP_("Main::on_loadDialog_fileSelected:  Loading game from \"{0}\"", path);
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
}
