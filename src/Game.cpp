#include "Game.hpp"
#include <godot_cpp/classes/directory.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include "function.hpp"

using namespace godot;

namespace
{
	// Options menu items indexes
	const int MI_HILITEBLANK = 3;
	const int MI_KEEPASPECT = 2;
	const int MI_NUMBERSVISIBLE = 1;
	const int MI_OUTLINESVISIBLE = 0;
}

void Game::_bind_methods()
{
	FUNC_("Game::_bind_methods");
		
	// API
	ClassDB::bind_method(D_METHOD("on_abort_pressed"), &Game::on_abort_pressed);
	ClassDB::bind_method(D_METHOD("on_hintDialog_close_pressed"), &Game::on_hintDialog_close_pressed);
	ClassDB::bind_method(D_METHOD("on_hintDialog_closeRequested"), &Game::on_hintDialog_closeRequested);
	ClassDB::bind_method(D_METHOD("on_hintDialog_sizeChanged"), &Game::on_hintDialog_sizeChanged);
	ClassDB::bind_method(D_METHOD("on_hint_pressed"), &Game::on_hint_pressed);
	ClassDB::bind_method(D_METHOD("on_loadDialog_cancelled"), &Game::on_loadDialog_cancelled);
	ClassDB::bind_method(D_METHOD("on_loadDialog_fileSelected"), &Game::on_loadDialog_fileSelected);
	ClassDB::bind_method(D_METHOD("on_load_pressed"), &Game::on_load_pressed);
	ClassDB::bind_method(D_METHOD("on_options_itemSelected"), &Game::on_options_itemSelected);
	ClassDB::bind_method(D_METHOD("on_quit_pressed"), &Game::on_quit_pressed);
	ClassDB::bind_method(D_METHOD("on_saveDialog_cancelled"), &Game::on_saveDialog_cancelled);
	ClassDB::bind_method(D_METHOD("on_saveDialog_fileSelected"), &Game::on_saveDialog_fileSelected);
	ClassDB::bind_method(D_METHOD("on_save_pressed"), &Game::on_save_pressed);
	ClassDB::bind_method(D_METHOD("on_tiles_itemRectChanged"), &Game::on_tiles_itemRectChanged);
	ClassDB::bind_method(D_METHOD("on_tiles_loaded"), &Game::on_tiles_loaded);
	ClassDB::bind_method(D_METHOD("on_tiles_moved"), &Game::on_tiles_moved);
	ClassDB::bind_method(D_METHOD("on_tiles_won"), &Game::on_tiles_won);
	ClassDB::bind_method(D_METHOD("on_winnerDialog_close_pressed"), &Game::on_winnerDialog_close_pressed);
}

Game::Game()
	: _hintDialogUsed(false),
	  _loadDialogUsed(false),
	  _saveDialogUsed(false)
{
	FUNC_("Game::Game");
		
	_abort = memnew(Button);
	_gameBoard = memnew(VBoxContainer);
	_gridContainer = memnew(GridContainer);
	_hflowContainer = memnew(HFlowContainer);
	_hflowContainer2 = memnew(HFlowContainer);
	_hint = memnew(Button);
	_hintClose = memnew(Button);
	_hintDialog = memnew(Window);
	_hintImage = memnew(TextureRect);
	_load = memnew(Button);
	_loadDialog = memnew(FileDialog);
	_marginContainer = memnew(MarginContainer);
	_moves = memnew(LineEdit);
	_movesLabel = memnew(Label);
	_panelContainer = memnew(PanelContainer);
	_options = memnew(MenuButton);
	_quit = memnew(Button);
	_save = memnew(Button);
	_saveDialog = memnew(FileDialog);
	_tiles = memnew(TilesControl);
	_vseparator = memnew(VSeparator);
	_vseparator2 = memnew(VSeparator);
	_vseparator3 = memnew(VSeparator);
	_vseparator4 = memnew(VSeparator);
	_vseparator5 = memnew(VSeparator);
	_winnerClose = memnew(Button);
	_winnerDialog = memnew(Window);
	_winnerLabel = memnew(Label);
}

Game::~Game()
{
	FUNC_("Game::~Game");
}

void Game::_input(const Ref<InputEvent>& ev)
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

	// Our settings
	set_anchor(Side::SIDE_RIGHT, 1, false, false);
	set_anchor(Side::SIDE_BOTTOM, 1, false, false);
	set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);

	// Add v box control to hold everything
	_gameBoard->set_anchor(Side::SIDE_RIGHT, 1, false, false);
	_gameBoard->set_anchor(Side::SIDE_BOTTOM, 1, false, false);
	_gameBoard->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	_gameBoard->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	add_child(_gameBoard);
		
	// Add panel container around top area
	_panelContainer->set_offset(Side::SIDE_RIGHT, 1280);
	_panelContainer->set_offset(Side::SIDE_BOTTOM, 53);
	_panelContainer->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	_gameBoard->add_child(_panelContainer);

	// Add h flow connector for full toolbar
	_hflowContainer2->set_offset(Side::SIDE_RIGHT, 1280);
	_hflowContainer2->set_offset(Side::SIDE_BOTTOM, 53);
	_hflowContainer2->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	_hflowContainer2->set_theme(theme);
	_panelContainer->add_child(_hflowContainer2);

	// Add h flow container for left part of toolbar
	_hflowContainer->set_offset(Side::SIDE_RIGHT, 1051);
	_hflowContainer->set_offset(Side::SIDE_BOTTOM, 53);
	_hflowContainer->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	_hflowContainer2->add_child(_hflowContainer);

	// Add abort button
	_abort->set_text("Quit Game");
	_abort->set_h_size_flags(0);
	_abort->set_v_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
	_hflowContainer->add_child(_abort);

	// Separate buttons
	_hflowContainer->add_child(_vseparator);

	// Add quit button
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
	pm->add_check_item("Outlines", MI_OUTLINESVISIBLE);
	pm->add_check_item("Numbers", MI_NUMBERSVISIBLE);
	pm->add_check_item("Aspect Ratio", MI_KEEPASPECT);
	pm->add_check_item("Hilite Blank", MI_HILITEBLANK);
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
	_moves->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
	_moves->set_editable(false);
	_moves->set_context_menu_enabled(false);
	_moves->set_virtual_keyboard_enabled(false);
	_moves->set_shortcut_keys_enabled(false);
	_moves->set_middle_mouse_paste_enabled(false);
	_gridContainer->add_child(_moves);
		
	// Add margin around tiles
	_marginContainer->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	_marginContainer->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	_gameBoard->add_child(_marginContainer);

	// Add tiles
	_tiles->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	_tiles->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
	_tiles->numbers_font_set(theme->get_default_font());
	_marginContainer->add_child(_tiles);

	// Add winner dialog
	_winnerDialog->hide();
	_winnerDialog->set_min_size(Vector2(320, 320));
	_winnerDialog->set_exclusive(true);
	_winnerDialog->set_title("Winner!");
	add_child(_winnerDialog);

	// Add the label to the dialog
	_winnerLabel->set_theme(theme);
	_winnerLabel->set_horizontal_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
	_winnerLabel->set_vertical_alignment(VerticalAlignment::VERTICAL_ALIGNMENT_CENTER);
	_winnerLabel->set_h_size_flags(0);
	_winnerLabel->set_v_size_flags(0);
	_winnerDialog->add_child(_winnerLabel);

	// Add the close button to the dialog
	_winnerClose->set_text("Close");
	_winnerClose->set_theme(theme);
	_winnerDialog->add_child(_winnerClose);

	// Add hint dialog
	_hintDialog->hide();
	_hintDialog->set_min_size(Vector2(480, 320));
	_hintDialog->set_exclusive(true);
	_hintDialog->set_title("Hint");
	add_child(_hintDialog);

	// Add hint image to dialog
	_hintImage->set_ignore_texture_size(true);
	_hintImage->set_stretch_mode(TextureRect::StretchMode::STRETCH_KEEP_ASPECT_CENTERED);
	_hintDialog->add_child(_hintImage);

	// Add close button to dialog
	_hintClose->set_text("Close");
	_hintClose->set_h_size_flags(0);
	_hintClose->set_v_size_flags(0);
	_hintDialog->add_child(_hintClose);

	// Add load game dialog
	_loadDialog->set_process_mode(Node::ProcessMode::PROCESS_MODE_PAUSABLE);
	_loadDialog->set_min_size(Vector2(480, 320));
	_loadDialog->set_exclusive(true);
	_loadDialog->set_title("Open A Game");
	_loadDialog->set_mode_overrides_title(false);
	_loadDialog->set_file_mode(FileDialog::FileMode::FILE_MODE_OPEN_FILE);
	_loadDialog->set_access(FileDialog::Access::ACCESS_FILESYSTEM);
	PackedStringArray filters;
	filters.append("*.kakel;Kakel Games");
	_loadDialog->set_filters(filters);
	add_child(_loadDialog);

	// Add save game dialog
	_saveDialog->set_process_mode(Node::ProcessMode::PROCESS_MODE_PAUSABLE);
	_saveDialog->set_min_size(Vector2(480, 320));
	_saveDialog->set_exclusive(true);
	_saveDialog->set_title("Save Game");
	_saveDialog->set_mode_overrides_title(false);
	_saveDialog->set_file_mode(FileDialog::FileMode::FILE_MODE_SAVE_FILE);
	_saveDialog->set_access(FileDialog::Access::ACCESS_FILESYSTEM);
	_saveDialog->set_filters(filters);
	add_child(_saveDialog);

	// Connect signals
	_abort->connect("pressed", Callable(this, "on_abort_pressed"));
	_hint->connect("pressed", Callable(this, "on_hint_pressed"));
	_hintClose->connect("pressed", Callable(this, "on_hintDialog_close_pressed"));
	_hintDialog->connect("close_requested", Callable(this, "on_hintDialog_closeRequested"));
	_hintDialog->connect("size_changed", Callable(this, "on_hintDialog_sizeChanged"));
	_load->connect("pressed", Callable(this, "on_load_pressed"));
	_loadDialog->connect("cancelled", Callable(this, "on_loadDialog_cancelled"));
	_loadDialog->connect("file_selected", Callable(this, "on_loadDialog_fileSelected"));
	_quit->connect("pressed", Callable(this, "on_quit_pressed"));
	_save->connect("pressed", Callable(this, "on_save_pressed"));
	_saveDialog->connect("cancelled", Callable(this, "on_saveDialog_cancelled"));
	_saveDialog->connect("file_selected", Callable(this, "on_saveDialog_fileSelected"));
	_tiles->connect("item_rect_changed", Callable(this, "on_tiles_itemRectChanged"));
	_tiles->connect("loaded", Callable(this, "on_tiles_loaded"));
	_tiles->connect("moved", Callable(this, "on_tiles_moved"));
	_tiles->connect("won", Callable(this, "on_tiles_won"));
	_winnerClose->connect("pressed", Callable(this, "on_winnerDialog_close_pressed"));
	pm->connect("index_pressed", Callable(this, "on_options_itemSelected"));

	// Update GUI for common settings
	_tiles->numbers_color_set(_preferences->numbers_color_get());
	_tiles->outlines_color_set(_preferences->outlines_color_get());

	// Numbers visible?
	bool b = _preferences->numbers_visible_get();
	_tiles->numbers_visible_set(b);
	pm->set_item_checked(MI_NUMBERSVISIBLE, b);

	// Outlines visible?
	b = _preferences->outlines_visible_get();
	_tiles->outlines_visible_set(b);
	pm->set_item_checked(MI_OUTLINESVISIBLE, b);

	// Keep aspect ratio?
	b = _globals->tiles_keep_aspect_get();
	_tiles->keep_aspect_set(b);
	pm->set_item_checked(MI_KEEPASPECT, b);
	
	// Hilite blank?
	b = _globals->tiles_hilite_blank_get();
	_tiles->hilite_blank_set(b);
	pm->set_item_checked(MI_HILITEBLANK, b);
	_tiles->hilite_blank_color_set(_preferences->hilite_blank_color_get());
	
	// Not all globals are used if loading a game.
	if (_globals->tiles_loading_get())
	{
		_globals->tiles_loading_set(false);
		const String tilesLoadPath = _globals->tiles_load_path_get();
		FUNCP_("Loading game from \"", tilesLoadPath, "\"");
		_tiles->load_game(tilesLoadPath);
	}
	else
	{
		_tiles->columns_set(_preferences->columns_get());
		_tiles->rows_set(_preferences->rows_get());
		if (_globals->tiles_use_image_get())
		{
			if (_globals->tiles_default_image_get())
				_tiles->image_path_set(_preferences->default_image_get());
			else
				_tiles->image_path_set(_preferences->last_image_get());
			pm->set_item_disabled(MI_KEEPASPECT, false);
			pm->set_item_disabled(MI_NUMBERSVISIBLE, false);
			pm->set_item_disabled(MI_OUTLINESVISIBLE, false);
			FUNCP_("tiles using image");
		}
		else
		{
			_tiles->image_path_set("");
			pm->set_item_disabled(MI_KEEPASPECT, true);
			pm->set_item_disabled(MI_NUMBERSVISIBLE, true);
			pm->set_item_disabled(MI_OUTLINESVISIBLE, true);
			FUNCP_("tiles not using image");
		}
		_tiles->start();
	}

	// Haven't quit yet
	_globals->tiles_quit_set(false);
}

void Game::abort()
{
	FUNC_("Game::abort");
	_globals->tiles_quit_set(true);
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
		return ResourceLoader::get_singleton()->load(path, "Image", ResourceLoader::CacheMode::CACHE_MODE_IGNORE);
	else
	{
		Ref<Image> i = Ref(memnew(Image));
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
	
void Game::on_hintDialog_closeRequested()
{
	FUNC_("Game::on_hintDialog_closeRequested");
	hide_hint();
}
	
void Game::on_hintDialog_sizeChanged()
{
	FUNC_("Game::on_hintDialog_sizeChanged");
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
		Ref<ImageTexture> it = ImageTexture::create_from_image(image);
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
	
void Game::on_loadDialog_cancelled()
{
	FUNC_("Game::on_loadDialog_popupHide");
	_tiles->movable_set(true);
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
	case MI_OUTLINESVISIBLE:
		_tiles->outlines_visible_set(on);
		break;

	case MI_NUMBERSVISIBLE:
		_tiles->numbers_visible_set(on);
		break;

	case MI_KEEPASPECT:
		_tiles->keep_aspect_set(on);
		break;

	case MI_HILITEBLANK:
		_tiles->hilite_blank_set(on);
		break;
		
	default:
		FUNCPF_("invalid menu item index ", index);
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
	
void Game::on_saveDialog_cancelled()
{
	FUNC_("Game::on_saveDialog_popupHide");
	_tiles->movable_set(true);
}
	
void Game::on_saveDialog_fileSelected(const String path)
{
	FUNC_("Game::on_saveDialog_fileSelected");
	_tiles->save_game(path);
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
	{
		FUNCP_("p ", _tiles->get_position(), " s ", _tiles->get_size());
		_tiles->recalc_tiles();
	}
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
		Ref<Directory> d = memnew(Directory);
		const String autoPath = _preferences->auto_path_get();
		if (d->file_exists(autoPath))
			d->remove(autoPath);
	}

	// Notify user.
	_winnerDialog->popup_centered();
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

