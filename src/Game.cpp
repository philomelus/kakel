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
	ERR_FAIL_COND(_globals == nullptr);

	_preferences = get_node<AppPreferences>("/root/Preferences");
	ERR_FAIL_COND(_preferences == nullptr);

	Ref<Theme> theme = _globals->theme_get();
	if (!theme.is_valid())
	{
		theme = Ref(ResourceLoader::get_singleton()->load(_preferences->default_theme_get(), "Theme",
														  ResourceLoader::CacheMode::CACHE_MODE_IGNORE));
		_globals->theme_set(theme);
	}
	ERR_FAIL_COND(theme == nullptr);
	
	_abort = get_node<Button>("GameBoard/PanelContainer/HFlowContainer2/HFlowContainer/Abort");
	ERR_FAIL_COND(_abort == nullptr);
	
	_gameBoard = get_node<VBoxContainer>("GameBoard");
	ERR_FAIL_COND(_gameBoard == nullptr);
	
	_hint = get_node<Button>("GameBoard/PanelContainer/HFlowContainer2/HFlowContainer/Hint");
	ERR_FAIL_COND(_hint == nullptr);
	
	_hintClose = get_node<Button>("HintDialog/HintClose");
	ERR_FAIL_COND(_hintClose == nullptr);
	
	_hintDialog = get_node<Window>("HintDialog");
	ERR_FAIL_COND(_hintDialog == nullptr);
	
	_hintImage = get_node<TextureRect>("HintDialog/HintImage");
	ERR_FAIL_COND(_hintImage == nullptr);
	
	_load = get_node<Button>("GameBoard/PanelContainer/HFlowContainer2/HFlowContainer/Load");
	ERR_FAIL_COND(_load == nullptr);
	
	_loadDialog = get_node<FileDialog>("LoadDialog");
	ERR_FAIL_COND(_loadDialog == nullptr);

	_moves = get_node<LineEdit>("GameBoard/PanelContainer/HFlowContainer2/GridContainer/Moves");
	ERR_FAIL_COND(_moves == nullptr);
	
	_options = get_node<MenuButton>("GameBoard/PanelContainer/HFlowContainer2/HFlowContainer/Options");
	ERR_FAIL_COND(_options == nullptr);
	
	_quit = get_node<Button>("GameBoard/PanelContainer/HFlowContainer2/HFlowContainer/Quit");
	ERR_FAIL_COND(_quit == nullptr);
	
	_save = get_node<Button>("GameBoard/PanelContainer/HFlowContainer2/HFlowContainer/Save");
	ERR_FAIL_COND(_save == nullptr);
	
	_saveDialog = get_node<FileDialog>("SaveDailog");
	ERR_FAIL_COND(_saveDialog == nullptr);
	
	_tiles = get_node<TilesControl>("GameBoard/MarginContainer/Tiles");
	ERR_FAIL_COND(_tiles == nullptr);
	
	_winnerClose = get_node<Button>("WinnerDialog/WinnerClose");
	ERR_FAIL_COND(_winnerClose == nullptr);
	
	_winnerLabel = get_node<Label>("WinnerDialog/WinnerLabel");
	ERR_FAIL_COND(_winnerLabel == nullptr);
	
	_tree = get_tree();

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

	PopupMenu* pm = _options->get_popup();
	ERR_FAIL_COND(pm == nullptr);
	pm->connect("index_pressed", Callable(this, "on_options_itemSelected"));

	// Update GUI for common settings
	_tiles->numbers_color_set(_preferences->numbers_color_get());
	_tiles->outlines_color_set(_preferences->outlines_color_get());
	_tiles->numbers_font_set(theme->get_default_font());

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

