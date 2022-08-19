#include "Game.hpp"
#include <Directory.hpp>
#include <DynamicFont.hpp>
#include <GlobalConstants.hpp>
#include <PopupMenu.hpp>
#include <ResourceLoader.hpp>
#include "function.hpp"

using namespace godot;

namespace godot
{
	void Game::_register_methods()
	{
		FUNC_("Game::_register_methods");
		
		// API
		register_method("_input", &Game::_input);
		register_method("_ready", &Game::_ready);
		register_method("on_abort_pressed", &Game::on_abort_pressed);
		register_method("on_hintDialog_close_pressed", &Game::on_hintDialog_close_pressed);
		register_method("on_hintDialog_popupHide", &Game::on_hintDialog_popupHide);
		register_method("on_hintDialog_resized", &Game::on_hintDialog_resized);
		register_method("on_hint_pressed", &Game::on_hint_pressed);
		register_method("on_loadDialog_fileSelected", &Game::on_loadDialog_fileSelected);
		register_method("on_loadDialog_popupHide", &Game::on_loadDialog_popupHide);
		register_method("on_load_pressed", &Game::on_load_pressed);
		register_method("on_options_itemSelected", &Game::on_options_itemSelected);
		register_method("on_quit_pressed", &Game::on_quit_pressed);
		register_method("on_saveDialog_fileSelected", &Game::on_saveDialog_fileSelected);
		register_method("on_saveDialog_popupHide", &Game::on_saveDialog_popupHide);
		register_method("on_save_pressed", &Game::on_save_pressed);
		register_method("on_tiles_itemRectChanged", &Game::on_tiles_itemRectChanged);
		register_method("on_tiles_loaded", &Game::on_tiles_loaded);
		register_method("on_tiles_moved", &Game::on_tiles_moved);
		register_method("on_tiles_won", &Game::on_tiles_won);
		register_method("on_winnerDialog_close_pressed", &Game::on_winnerDialog_close_pressed);
	}

	Game::Game()
		: _hintDialogUsed(false),
		  _loadDialogUsed(false),
		  _saveDialogUsed(false)
	{
		FUNC_("Game::Game");
	}

	Game::~Game()
	{
		FUNC_("Game::~Game");
	}

	void Game::_init()
	{
		FUNC_("Game::_init");
		
		_abort = Button::_new();
		_gameBoard = VBoxContainer::_new();
		_gridContainer = GridContainer::_new();
		_hflowContainer = HFlowContainer::_new();
		_hflowContainer2 = HFlowContainer::_new();
		_hint = Button::_new();
		_hintClose = Button::_new();
		_hintDialog = WindowDialog::_new();
		_hintImage = TextureRect::_new();
		_load = Button::_new();
		_loadDialog = FileDialog::_new();
		_marginContainer = MarginContainer::_new();
		_moves = LineEdit::_new();
		_movesLabel = Label::_new();
		_panelContainer = PanelContainer::_new();
		_options = MenuButton::_new();
		_quit = Button::_new();
		_save = Button::_new();
		_saveDialog = FileDialog::_new();
		_tiles = TilesControl::_new();
		_vseparator = VSeparator::_new();
		_vseparator2 = VSeparator::_new();
		_vseparator3 = VSeparator::_new();
		_vseparator4 = VSeparator::_new();
		_vseparator5 = VSeparator::_new();
		_winnerClose = Button::_new();
		_winnerDialog = WindowDialog::_new();
		_winnerLabel = Label::_new();
	}

	void Game::_input(const Ref<InputEvent> ev)
	{
		FUNCQ_("Game::_input");
		if (ev->is_action_pressed("quit"))
		{
			accept_event();
            abort();
			return;
		}
		if (ev->is_action_pressed("refresh"))
		{
			accept_event();
			update();
			return;
		}
	}
	
	void Game::_ready()
	{
		FUNC_("Game::_ready");
		
		_globals = get_node<AppGlobals>("/root/Globals");
		_preferences = get_node<AppPreferences>("/root/Preferences");
		_tree = get_tree();

		ERR_FAIL_COND(_globals == nullptr);
		ERR_FAIL_COND(_preferences == nullptr);
		
		Ref<Theme> theme = _globals->theme_get();

		ERR_FAIL_COND(theme == nullptr);

		Ref<DynamicFont> font = ResourceLoader::get_singleton()->load("res://font_24.tres", "DynamicFont");
		
		// Our settings
		set_anchor(GlobalConstants::MARGIN_RIGHT, 1, false, false);
		set_anchor(GlobalConstants::MARGIN_BOTTOM, 1, false, false);
		set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);

		// Add v box control to hold everything
		_gameBoard->set_margin(GlobalConstants::MARGIN_RIGHT, 1280);
		_gameBoard->set_margin(GlobalConstants::MARGIN_BOTTOM, 720);
		_gameBoard->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		_gameBoard->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		add_child(_gameBoard);
		
		// Add panel container around top area
		_gameBoard->add_child(_panelContainer);

		// Add h flow connector for full toolbar
		_hflowContainer2->set_theme(theme);
		_hflowContainer2->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		_panelContainer->add_child(_hflowContainer2);

		// Add h flow container for left part of toolbar
		_hflowContainer->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		_hflowContainer2->add_child(_hflowContainer);

		// Add quit button
		_abort->set_text("Quit Game");
		_abort->set_h_size_flags(0);
		_abort->set_v_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
		_hflowContainer->add_child(_abort);

		// Separate buttons
		_hflowContainer->add_child(_vseparator);

		// Add exit button
		_quit->set_text("Exit Kakel");
		_quit->set_h_size_flags(0);
		_quit->set_v_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
		_hflowContainer->add_child(_quit);

		// Separate buttons
		_hflowContainer->add_child(_vseparator2);

		// Add load game button
		_load->set_text("Load");
		_load->set_h_size_flags(0);
		_load->set_v_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
		_hflowContainer->add_child(_load);

		// Separate buttons
		_hflowContainer->add_child(_vseparator3);

		// Add save game button
		_save->set_text("Save");
		_save->set_h_size_flags(0);
		_save->set_v_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
		_hflowContainer->add_child(_save);

		// Separate buttons
		_hflowContainer->add_child(_vseparator4);

		// Add options menu
		_options->set_text("Options");
		_options->set_h_size_flags(0);
		_options->set_v_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
		PopupMenu* pm = _options->get_popup();
		pm->add_check_item("Outlines", 0);
		pm->add_check_item("Numbers", 1);
		_hflowContainer->add_child(_options);
		
		// Separate control areas
		_hflowContainer->add_child(_vseparator5);

		// Add hint button
		_hint->set_text("Hint");
		_hint->set_h_size_flags(0);
		_hint->set_v_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
		_hflowContainer->add_child(_hint);
		
		// Add moves section
		_gridContainer->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_END);
		_gridContainer->set_columns(2);
		_hflowContainer2->add_child(_gridContainer);

		// Add moves label
		_movesLabel->set_text("Moves");
		_gridContainer->add_child(_movesLabel);

		// Add moves line edit
		_moves->set_text("0");
		_moves->set_v_size_flags(Control::SizeFlags::SIZE_FILL
								 + Control::SizeFlags::SIZE_SHRINK_CENTER);
		_moves->set_align(LineEdit::Align::ALIGN_CENTER);
		_moves->set_editable(false);
		_moves->set_context_menu_enabled(false);
		_moves->set_virtual_keyboard_enabled(false);
		_moves->set_shortcut_keys_enabled(false);
		_moves->set_middle_mouse_paste_enabled(false);
		_gridContainer->add_child(_moves);
		
		// Add margin around tiles
		_marginContainer->set_margin(GlobalConstants::MARGIN_TOP, 73);
		_marginContainer->set_margin(GlobalConstants::MARGIN_RIGHT, 1280);
		_marginContainer->set_margin(GlobalConstants::MARGIN_BOTTOM, 720);
		_marginContainer->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		_marginContainer->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		_gameBoard->add_child(_marginContainer);

		// Add tiles
		_tiles->set_margin(GlobalConstants::MARGIN_LEFT, 10);
		_tiles->set_margin(GlobalConstants::MARGIN_TOP, 10);
		_tiles->set_margin(GlobalConstants::MARGIN_RIGHT, 1270);
		_tiles->set_margin(GlobalConstants::MARGIN_BOTTOM, 637);
		_tiles->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		_tiles->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		_tiles->numbers_font_set(font);
		_marginContainer->add_child(_tiles);

		// Add winner dialog
		_winnerDialog->set_custom_minimum_size(Vector2(320, 320));
		_winnerDialog->set_exclusive(true);
		_winnerDialog->set_title("Winner!");
		add_child(_winnerDialog);

		// Add the label to the dialog
		_winnerLabel->set_theme(theme);
		_winnerLabel->set_align(Label::Align::ALIGN_CENTER);
		_winnerLabel->set_valign(Label::VAlign::VALIGN_CENTER);
		_winnerLabel->set_h_size_flags(0);
		_winnerLabel->set_v_size_flags(0);
		_winnerDialog->add_child(_winnerLabel);

		// Add the close button to the dialog
		_winnerClose->set_text("Close");
		_winnerClose->set_theme(theme);
		_winnerDialog->add_child(_winnerClose);

		// Add hint dialog
		_hintDialog->set_custom_minimum_size(Vector2(480, 320));
		_hintDialog->set_resizable(true);
		_hintDialog->set_exclusive(true);
		_hintDialog->set_title("Hint");
		add_child(_hintDialog);

		// Add hint image to dialog
		_hintImage->set_expand(true);
		_hintImage->set_stretch_mode(TextureRect::StretchMode::STRETCH_KEEP_ASPECT_CENTERED);
		_hintDialog->add_child(_hintImage);

		// Add close button to dialog
		_hintClose->set_text("Close");
		_hintClose->set_h_size_flags(0);
		_hintClose->set_v_size_flags(0);
		_hintDialog->add_child(_hintClose);

		// Add load game dialog
		_loadDialog->set_pause_mode(Node::PauseMode::PAUSE_MODE_PROCESS);
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
		add_child(_loadDialog);

		// Add save game dialog
		_saveDialog->set_pause_mode(Node::PauseMode::PAUSE_MODE_PROCESS);
		_saveDialog->set_custom_minimum_size(Vector2(480, 320));
		_saveDialog->set_exclusive(true);
		_saveDialog->set_title("Save Game");
		_saveDialog->set_resizable(true);
		_saveDialog->set_mode_overrides_title(false);
		_saveDialog->set_mode(FileDialog::Mode::MODE_SAVE_FILE);
		_saveDialog->set_access(FileDialog::Access::ACCESS_FILESYSTEM);
		_saveDialog->set_filters(filters);
		add_child(_saveDialog);

		// Connect signals
		_abort->connect("pressed", this, "on_abort_pressed");
		_hint->connect("pressed", this, "on_hint_pressed");
		_hintClose->connect("pressed", this, "on_hintDialog_close_pressed");
		_hintDialog->connect("popup_hide", this, "on_hintDialog_popupHide");
		_hintDialog->connect("resized", this, "on_hintDialog_resized");
		_load->connect("pressed", this, "on_load_pressed");
		_loadDialog->connect("file_selected", this, "on_loadDialog_fileSelected");
		_loadDialog->connect("popup_hide", this, "on_loadDialog_popupHide");
		_quit->connect("pressed", this, "on_quit_pressed");
		_save->connect("pressed", this, "on_save_pressed");
		_saveDialog->connect("file_selected", this, "on_saveDialog_fileSelected");
		_saveDialog->connect("popup_hide", this, "on_saveDialog_popupHide");
		_tiles->connect("item_rect_changed", this, "on_tiles_itemRectChanged");
		_tiles->connect("loaded", this, "on_tiles_loaded");
		_tiles->connect("moved", this, "on_tiles_moved");
		_tiles->connect("won", this, "on_tiles_won");
		_winnerClose->connect("pressed", this, "on_winnerDialog_close_pressed");
        pm->connect("index_pressed", this, "on_options_itemSelected");

        // Update GUI for common settings
        _tiles->numbers_color_set(_preferences->numbers_color_get());
        _tiles->outlines_color_set(_preferences->outlines_color_get());
		bool b = _preferences->numbers_visible_get();
        _tiles->numbers_visible_set(b);
        pm->set_item_checked(1, b);
		b = _preferences->outlines_visible_get();
        _tiles->outlines_visible_set(b);
        pm->set_item_checked(0, b);

        // Not all globals are used if loading a game.
        if (_globals->tiles_loading_get())
        {
            _globals->tiles_loading_set(false);
            const String tilesLoadPath = _globals->tiles_load_path_get();
			FUNCP_("Game::_ready: Loading game from \"{0}\"", tilesLoadPath);
            _tiles->load_game(tilesLoadPath);
        }
        else
        {
            _tiles->columns_set(_preferences->columns_get());
            _tiles->rows_set(_preferences->rows_get());
            if (_preferences->use_image_get())
            {
                if (_globals->tiles_default_image_get())
                    _tiles->image_path_set(_preferences->default_image_get());
                else
                    _tiles->image_path_set(_preferences->last_image_get());
                pm->set_item_disabled(0, false);
                pm->set_item_disabled(1, false);
				FUNCP_("Game::_ready: tiles using image");
            }
            else
            {
                _tiles->image_path_set("");
                pm->set_item_disabled(0, true);
                pm->set_item_disabled(1, true);
				FUNCP_("Game::_ready: tiles not using image");
            }
            _tiles->start();
        }
	}
	
	void Game::abort()
	{
		FUNC_("Game::abort");
        _tree->change_scene("res://Main.tscn");
	}
	
	void Game::hide_hint()
	{
		FUNC_("Game::hide_hint");
        _gameBoard->show();
        _hintDialog->hide();
	}
	
	Ref<Image> Game::load_image(const String path)
	{
		FUNC_("Game::load_image");
		if (path.substr(0, 4) == "res:")
			return ResourceLoader::get_singleton()->load(path, "Image", true);
		else
		{
			Ref<Image> i = Image::_new();
			i->load(path);
			return i;
		}
	}
	
	void Game::on_abort_pressed()
	{
		FUNC_("Game::on_abort_pressed");
        abort();
	}
	
	void Game::on_hintDialog_close_pressed()
	{
		FUNC_("Game::on_hintDialog_close_pressed");
		hide_hint();
	}
	
	void Game::on_hintDialog_popupHide()
	{
		FUNC_("Game::on_hintDialog_popupHide");
		hide_hint();
	}
	
	void Game::on_hintDialog_resized()
	{
		FUNC_("Game::on_hintDialog_resized");
		update_hintDialog();
	}
	
	void Game::on_hint_pressed()
	{
		FUNC_("Game::on_hint_pressed");
        _gameBoard->hide();
        if (_hintImage->get_texture() == nullptr)
        {
            Ref<Image> image;
            if (_globals->tiles_default_image_get())
                image = load_image(_preferences->default_image_get());
            else
                image = load_image(_preferences->last_image_get());
            Ref<ImageTexture> it(ImageTexture::_new());
            it->create_from_image(image);
            _hintImage->set_texture(it);
        }
		update_hintDialog();
        if (_hintDialogUsed)
        {
            _hintDialog->popup();
        }
        else
        {
            _hintDialogUsed = true;
           _hintDialog->popup_centered();
        }
	}
	
	void Game::on_loadDialog_fileSelected(const String path)
	{
		FUNC_("Game::on_loadDialog_fileSelected");
        if (_preferences->last_game_get() != path)
        {
            _preferences->last_game_set(path);
            _preferences->save(_preferences->P_PREFS);
        }
        _tiles->load_game(path);
        _tiles->movable_set(true);
	}
	
	void Game::on_loadDialog_popupHide()
	{
		FUNC_("Game::on_loadDialog_popupHide");
        _tiles->movable_set(true);
	}
	
	void Game::on_load_pressed()
	{
		FUNC_("Game::on_load_pressed");
        _tiles->movable_set(false);
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
	
	void Game::on_options_itemSelected(const int index)
	{
		FUNC_("Game::on_options_itemSelected");
        PopupMenu* pm = _options->get_popup();
        pm->toggle_item_checked(index);
        const bool on = pm->is_item_checked(index);
        switch (index)
        {
        case 0: // Outlines
            _tiles->outlines_visible_set(on);
            break;

        case 1: // Numbers
            _tiles->numbers_visible_set(on);
            break;
        }
	}
	
	void Game::on_quit_pressed()
	{
		FUNC_("Game::on_quit_pressed");
        // Save game if desired.
        if (_preferences->auto_save_get())
            _tiles->save_game(_preferences->auto_path_get());

        // Get outta here.
        _tree->quit();
	}
	
	void Game::on_saveDialog_fileSelected(const String path)
	{
		FUNC_("Game::on_saveDialog_fileSelected");
        _tiles->save_game(path);
        _tiles->movable_set(true);
	}
	
	void Game::on_saveDialog_popupHide()
	{
		FUNC_("Game::on_saveDialog_popupHide");
        _tiles->movable_set(true);
	}
	
	void Game::on_save_pressed()
	{
		FUNC_("Game::on_save_pressed");
        _tiles->movable_set(false);
        if (_saveDialogUsed)
        {
            _saveDialog->popup();
        }
        else
        {
            _saveDialogUsed = true;
            _saveDialog->popup_centered();
        }
	}
	
	void Game::on_tiles_itemRectChanged()
	{
		FUNC_("Game::on_tiles_itemRectChanged");
        if (_tiles != nullptr)
            _tiles->recalc_tiles();
	}

	void Game::on_tiles_loaded()
	{
		FUNC_("Game::on_tiles_loaded");
		
		// Update options
        PopupMenu* pm = _options->get_popup();
        pm->set_item_checked(1, _tiles->numbers_visible_get());
        pm->set_item_checked(0, _tiles->outlines_visible_get());
		const bool disabled = _tiles->image_path_get().length() > 0 ? false : true;
		pm->set_item_disabled(0, disabled);
		pm->set_item_disabled(1, disabled);
	}
	
	void Game::on_tiles_moved(int count)
	{
		FUNC_("Game::on_tiles_moved");
        _moves->set_text(String::num_int64(count));
	}
	
	void Game::on_tiles_won()
	{
		FUNC_("Game::on_tiles_won");
		
        // Update message.
        _winnerLabel->set_text("Congratulations!\nYou won in\n" + _moves->get_text() + "\nmoves!");

        // Size everything correctly.
		_winnerLabel->set_position(Vector2(0, 0));
		const Vector2 wds = _winnerDialog->get_size();
		const Vector2 wcs = _winnerClose->get_size();
        _winnerLabel->set_size(Vector2(wds.x, wds.y - 20 - wcs.y));
        _winnerClose->set_position(Vector2((wds.x - wcs.x) / 2, wds.y - 10 - wcs.y));

        // If game was auto started, then remove auto save if desired
        if (_globals->auto_started_get() && _preferences->auto_remove_on_win_get())
        {
            Ref<Directory> d(Directory::_new());
			const String autoPath = _preferences->auto_path_get();
            if (d->file_exists(autoPath))
                d->remove(autoPath);
        }

        // Notify user.
        _winnerDialog->popup_centered();
		FUNCP_("_winnerDialog: p {0} s {1}", _winnerDialog->get_position(), _winnerDialog->get_size());
		FUNCP_("_winnerLabel: p {0} s {1}", _winnerLabel->get_position(), _winnerLabel->get_size());
	}
	
	void Game::on_winnerDialog_close_pressed()
	{
		FUNC_("Game::on_winnerDialog_close_pressed");
		abort();
	}
	
	void Game::update_hintDialog()
	{
		FUNC_("Game::update_hintDialog");
        const Vector2 ws = _hintDialog->get_size();
        const Vector2 cs = _hintClose->get_size();
        _hintImage->set_position(Vector2(10, 10));
        _hintImage->set_size(Vector2(ws.x - 20, ws.y - 25 - cs.y));
        _hintClose->set_position(Vector2((ws.x - cs.x) / 2, ws.y - cs.y - 10));
	}
}
