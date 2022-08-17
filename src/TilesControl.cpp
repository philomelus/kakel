#include "TilesControl.hpp"
#include <godot_cpp/classes/file.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include "function.hpp"

// #define TEST_WON 1

using namespace godot;

namespace
{
	void outline(TilesControl* on, const Rect2 where, const Color color)
	{
		Rect2 area(where);
		Vector2 v = area.get_position();
		--v.x;
		++v.y;
		area.set_position(v);
		v = area.get_size();
		v.x += 2;
		v.y += 2;
		area.set_size(v);
		on->draw_rect(area, color, false);
	}
}

const int TilesControl::FILE_VERSION = 1;
	
void TilesControl::_bind_methods()
{
	FUNC_("TilesControl::_bind_methods");
		
	// API
	ClassDB::bind_method(D_METHOD("calc_movables"), &TilesControl::calc_movables);
	ClassDB::bind_method(D_METHOD("load_game"), &TilesControl::load_game);
	ClassDB::bind_method(D_METHOD("recalc_tiles"), &TilesControl::recalc_tiles);
	ClassDB::bind_method(D_METHOD("save_game"), &TilesControl::save_game);
	ClassDB::bind_method(D_METHOD("start"), &TilesControl::start);

	// Non-api functions
	ClassDB::bind_method(D_METHOD("can_move_down_get"), &TilesControl::can_move_down_get);
	ClassDB::bind_method(D_METHOD("can_move_down_set", "newVal"), &TilesControl::can_move_down_set);
	ClassDB::bind_method(D_METHOD("can_move_left_get"), &TilesControl::can_move_left_get);
	ClassDB::bind_method(D_METHOD("can_move_left_set", "newVal"), &TilesControl::can_move_left_set);
	ClassDB::bind_method(D_METHOD("can_move_right_get"), &TilesControl::can_move_right_get);
	ClassDB::bind_method(D_METHOD("can_move_right_set", "newVal"), &TilesControl::can_move_right_set);
	ClassDB::bind_method(D_METHOD("can_move_up_get"), &TilesControl::can_move_up_get);
	ClassDB::bind_method(D_METHOD("can_move_up_set", "newVal"), &TilesControl::can_move_up_set);
	ClassDB::bind_method(D_METHOD("columns_get"), &TilesControl::columns_get);
	ClassDB::bind_method(D_METHOD("columns_set", "newVal"), &TilesControl::columns_set);
	ClassDB::bind_method(D_METHOD("hilite_blank_get"), &TilesControl::hilite_blank_get);
	ClassDB::bind_method(D_METHOD("hilite_blank_set", "newVal"), &TilesControl::hilite_blank_set);
	ClassDB::bind_method(D_METHOD("image_path_get"), &TilesControl::image_path_get);
	ClassDB::bind_method(D_METHOD("image_path_set", "newVal"), &TilesControl::image_path_set);
	ClassDB::bind_method(D_METHOD("keep_aspect_get"), &TilesControl::keep_aspect_get);
	ClassDB::bind_method(D_METHOD("keep_aspect_set", "newVal"), &TilesControl::keep_aspect_set);
	ClassDB::bind_method(D_METHOD("movable_get"), &TilesControl::movable_get);
	ClassDB::bind_method(D_METHOD("movable_set", "newVal"), &TilesControl::movable_set);
	ClassDB::bind_method(D_METHOD("numbers_font_get"), &TilesControl::numbers_font_get);
	ClassDB::bind_method(D_METHOD("numbers_font_set", "newVal"), &TilesControl::numbers_font_set);
	ClassDB::bind_method(D_METHOD("numbers_color_get"), &TilesControl::numbers_color_get);
	ClassDB::bind_method(D_METHOD("numbers_color_set", "newVal"), &TilesControl::numbers_color_set);
	ClassDB::bind_method(D_METHOD("numbers_visible_get"), &TilesControl::numbers_visible_get);
	ClassDB::bind_method(D_METHOD("numbers_visible_set", "newVal"), &TilesControl::numbers_visible_set);
	ClassDB::bind_method(D_METHOD("outlines_color_get"), &TilesControl::outlines_color_get);
	ClassDB::bind_method(D_METHOD("outlines_color_set", "newVal"), &TilesControl::outlines_color_set);
	ClassDB::bind_method(D_METHOD("outlines_visible_get"), &TilesControl::outlines_visible_get);
	ClassDB::bind_method(D_METHOD("outlines_visible_set", "newVal"), &TilesControl::outlines_visible_set);
	ClassDB::bind_method(D_METHOD("rows_get"), &TilesControl::rows_get);
	ClassDB::bind_method(D_METHOD("rows_set", "newVal"), &TilesControl::rows_set);
	ClassDB::bind_method(D_METHOD("spacing_get"), &TilesControl::spacing_get);
	ClassDB::bind_method(D_METHOD("spacing_set", "newVal"), &TilesControl::spacing_set);
	ClassDB::bind_method(D_METHOD("tiles_count_get"), &TilesControl::tiles_count_get);
	ClassDB::bind_method(D_METHOD("tiles_count_set", "newVal"), &TilesControl::tiles_count_set);
	
	// Properties
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "can_move_down"), "can_move_down_set", "can_move_down_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "can_move_left"), "can_move_left_set", "can_move_left_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "can_move_right"), "can_move_right_set", "can_move_right_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "can_move_up"), "can_move_up_set", "can_move_up_get");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "columns"), "columns_set", "columns_get");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "image_path"), "image_path_set", "image_path_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "keep_aspect"), "keep_aspect_set", "keep_aspect_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "movable"), "movable_set", "movable_get");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "number_font"), "numbers_font_set", "numbers_font_get");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "numbers_color"), "numbers_color_set", "numbers_color_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "numbers_visible"), "numbers_visible_set", "numbers_visible_get");
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "outlines_color"), "outlines_color_set", "outlines_color_get");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "outlines_visible"), "outlines_visible_set", "outlines_visible_get");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "rows"), "rows_set", "rows_get");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "spacing"), "spacing_set", "spacing_get");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tiles_count"), "tiles_count_set", "tiles_count_get");

	// Signals
	ADD_SIGNAL(MethodInfo("loaded"));
	ADD_SIGNAL(MethodInfo("moved", PropertyInfo(Variant::INT, "count")));
	ADD_SIGNAL(MethodInfo("saved"));
	ADD_SIGNAL(MethodInfo("won"));
}

TilesControl::TilesControl() :
	_gameComplete(false)
{
	FUNC_("TilesControl::TilesControl");

	_backgroundColor = Color::named("transparent");
	_columns = 4;
	_columns0 = 3;
	_gameComplete = false;
	_imagePath = String();
	_lastSignal = 0.0;
	_lastWinner = 0.0;
	_movedSignal = 0;
	_moves = 0;
	_movesEnabled = false;
	_numTiles = 16;
	_numTiles0 = 15;
	_numbersColor = Color(0.8, 0.8, 0.8, 1);
	_numbersVisible;
	_outlinesColor = Color(0.5, 0.5, 0.5, 1);
	_outlinesVisible;
	_readyToRun = false;
	_rows = 4;
	_rows0 = 3;
	_spacing = Vector2(5, 5);
	_tilesReady = false;
	_tileSize = Vector2();
}

TilesControl::~TilesControl()
{
	FUNC_("TilesControl::~TilesControl");
}

void TilesControl::_draw()
{
	FUNC_("TilesControl::_draw");

	// No drawing before everything is ready
	if (!_readyToRun || !_tilesReady)
		return;

	// Loop through all tiles and draw them
	Vector2 extent;
	int index = 0;
	int tileIndex;
	for (int i = 0; i < _numTiles; ++i)
	{
		// Get actual location of tile
		tileIndex = _tilesOrder[i];

		// If its not the blank tile
		if (tileIndex != _emptyId)
		{
			if (useImage())
			{
				// Copy image to screen
				draw_texture_rect_region(_tilesTexture, _tilesRectScreen[index],
										 _tilesRectTexture[_tilesOrder[index]]);

				// Add the tile number is desired
				if (_numbersVisible)
				{
					String name = String::num_int64(tileIndex + 1);
					extent = _numbersFont->get_string_size(name);

					// First draw a 2 pixel outline
					draw_string_outline(_numbersFont, Vector2(Rect2(_tilesRectScreen[index]).get_position().x + 5,
													  Rect2(_tilesRectScreen[index]).get_position().y + 10 + (extent.y / 2)),
										name, HorizontalAlignment::HORIZONTAL_ALIGNMENT_LEFT, -1, 24, 3, Color::named("black"));
					
					// Draw text in desired color
					draw_string(_numbersFont, Vector2(Rect2(_tilesRectScreen[index]).get_position().x + 5,
													  Rect2(_tilesRectScreen[index]).get_position().y + 10 + (extent.y / 2)),
								name, HorizontalAlignment::HORIZONTAL_ALIGNMENT_LEFT, -1, 24, _numbersColor);
				}

				// Add an outline if desired
				if (_outlinesVisible)
					outline(this, _tilesRectScreen[index], _outlinesColor);
			}
			else			// if (useImage())
			{
				// Draw outline of tile
				Rect2 area = _tilesRectScreen[index];
				draw_rect(area, _outlinesColor, false);

				// Draw tile number of tile
				String name = String::num_int64(tileIndex + 1);
				extent = _numbersFont->get_string_size(name);
				draw_string(_numbersFont, Vector2(area.get_position().x + ((area.get_size().x - extent.x) / 2),
												  area.get_position().y + ((area.get_size().y - extent.y) / 2) + extent.y),
							name, HorizontalAlignment::HORIZONTAL_ALIGNMENT_LEFT, -1, 24, _numbersColor);
			}
		} // Not blank tile
		else
		{
			if (_hiliteBlank)
			{
				Rect2 area = _tilesRectScreen[index];
				if (_outlinesVisible)
					outline(this, area, _hiliteBlankColor);
				String text = "Blank Tile";
				extent = _numbersFont->get_string_size(text);
				draw_string(_numbersFont, Vector2(area.get_position().x + ((area.get_size().x - extent.x) / 2),
												  area.get_position().y + ((area.get_size().y - extent.y) / 2) + extent.y),
							text, HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER, -1, 24, _hiliteBlankColor);
			}
		}
		++index;
	}
}

void TilesControl::_input(const Ref<InputEvent>& ev)
{
	FUNCQ_("TilesControl::_inpuit");
		
	// Do nothing while paused or uninitialized
	if (!_readyToRun || !_tilesReady || _tree->is_paused() || !_movesEnabled)
		return;

	// Only handle mouse clicks here
	Ref<InputEventMouseButton> iemb = static_cast<Ref<InputEventMouseButton>>(ev);
	if (iemb == nullptr)
		return;

	// Only handle left mouse clicks
	if (iemb->get_button_index() != 1)
		return;

	// Don't pay attention to double clicks
	if (iemb->is_double_click())
		return;

	// Only repond to clicks when released
	if (iemb->is_pressed())
		return;

	// Adjust position of click to local coordinates
	Ref<InputEventMouseButton> evmb = static_cast<Ref<InputEventMouseButton>>(make_input_local(iemb));
	Vector2 position = evmb->get_position();
		
	// Take appropriate action if needed
	if (_canMoveDown)
	{
		if (Rect2(_tilesRectScreen[_moveDownIndex]).has_point(position))
		{
			accept_event();
			move_down();
			return;
		}
	}
	if (_canMoveLeft)
	{
		if (Rect2(_tilesRectScreen[_moveLeftIndex]).has_point(position))
		{
			accept_event();
			move_left();
			return;
		}
	}
	if (_canMoveRight)
	{
		if (Rect2(_tilesRectScreen[_moveRightIndex]).has_point(position))
		{
			accept_event();
			move_right();
			return;
		}
	}
	if (_canMoveUp)
	{
		if (Rect2(_tilesRectScreen[_moveUpIndex]).has_point(position))
		{
			accept_event();
			move_up();
			return;
		}
	}
}
	
void TilesControl::_physics_process(double delta)
{
	FUNCQ_("TilesControl::_physics_process");
		
	if (!_readyToRun)
		return;

	// Update tiles if needed.
	if (!_tilesReady)
		recalc_tiles();

	// Emit moved signal if needed @ 1/50th second.
	if (_movedSignal > 0)
	{
		if (_lastSignal + delta > 0.02)
		{
			int m = _moves;
			_movedSignal = 0;
			emit_signal("moved", m);
			_lastSignal = 0.0;
		}
		else
			_lastSignal += delta;
	}

	// Limit winning checks to 1/20 second.
	if (_lastWinner + delta > 0.05f)
	{
		check_complete();
		_lastWinner = 0.0;
	}
	else
		_lastWinner += delta;
}
	
void TilesControl::_ready()
{
	FUNC_("TilesControl::_ready");
		
	_tree = get_tree();
		
	_readyToRun = false;
	_movesEnabled = false;
}
	
void TilesControl::_unhandled_input(const Ref<InputEvent>& ev)
{
	FUNCQ_("TilesControl::_unhandled_input");
		
	if (!_readyToRun || !_tilesReady || _tree->is_paused() || !_movesEnabled
		|| !ev->is_action_type())
	{
		return;
	}

	// Take appropriate action
	if (_canMoveLeft)
	{
		if (ev->is_action_pressed("move_left"))
		{
			accept_event();
			move_left();
			return;
		}
	}
	if (_canMoveRight)
	{
		if (ev->is_action_pressed("move_right"))
		{
			accept_event();
			move_right();
			return;
		}
	}
	if (_canMoveUp)
	{
		if (ev->is_action_pressed("move_up"))
		{
			accept_event();
			move_up();
			return;
		}
	}
	if (_canMoveDown)
	{
		if (ev->is_action_pressed("move_down"))
		{
			accept_event();
			move_down();
			return;
		}
	}
}
	
void TilesControl::calc_movables()
{
	FUNC_("TilesControl::calc_movables");
		
	// Do nothing until initialized
	if (!_readyToRun)
		return;

	// Row and column of blank tile
	const int column = _empty % _columns;
	const int row = _empty / _columns;

	// Determine tiles that can move horizontally
	const bool left_side = (column == 0);
	const bool right_side = (column == _columns0);
	const bool inside_h = (column > 0 && column < _columns0);
	if (left_side)
	{
		_canMoveLeft = true;
		_canMoveRight = false;
	}
	else if (right_side)
	{
		_canMoveLeft = false;
		_canMoveRight = true;
	}
	else if (inside_h)
	{
		_canMoveLeft = true;
		_canMoveRight = true;
	}
	if (_canMoveLeft)
		_moveLeftIndex = (row * _columns) + column + 1;
	if (_canMoveRight)
		_moveRightIndex = (row * _columns) + column - 1;

	// Determine tiles that can move vertically
	const bool top_side = (row == 0);
	const bool bottom_side = (row == _rows0);
	const bool inside_v = (row > 0 && row < _rows0);
	if (top_side)
	{
		_canMoveUp = true;
		_canMoveDown = false;
	}
	else if (bottom_side)
	{
		_canMoveUp = false;
		_canMoveDown = true;
	}
	else if (inside_v)
	{
		_canMoveUp = true;
		_canMoveDown = true;
	}
	if (_canMoveUp)
		_moveUpIndex = ((row + 1) * _columns) + column;
	if (_canMoveDown)
		_moveDownIndex = ((row - 1) * _columns) + column;
}
	
bool TilesControl::check_complete()
{
	FUNCQ_("TilesControl::check_complete");

	// Only signal winner once
	if (_gameComplete)
		return true;
		
#ifdef TEST_WON
	if (_moves < 10)
	{
#endif
		for(int index = 0; index < _numTiles; ++index)
		{
			if (_tilesOrder[index] != index)
				return false;
		}
#ifdef TEST_WON
	}
#endif
	_gameComplete = true;
	_movesEnabled = false;
	emit_signal("won");
	return true;
}
		
bool TilesControl::can_move_down_get() const
{
	return _canMoveDown;
}
	
void TilesControl::can_move_down_set(const bool newVal)
{
	CRASH_NOW();
}
	
bool TilesControl::can_move_left_get() const
{
	return _canMoveLeft;
}
	
void TilesControl::can_move_left_set(const bool newVal)
{
	CRASH_NOW();
}
	
bool TilesControl::can_move_right_get() const
{
	return _canMoveRight;
}
	
void TilesControl::can_move_right_set(const bool newVal)
{
	CRASH_NOW();
}
	
bool TilesControl::can_move_up_get() const
{
	return _canMoveUp;
}
	
void TilesControl::can_move_up_set(const bool newVal)
{
	CRASH_NOW();
}
	
int TilesControl::columns_get() const
{
	return _columns;
}
	
void TilesControl::columns_set(const int newVal)
{
	ERR_FAIL_COND(newVal < 3 || newVal > 99);

	if (_columns != newVal)
	{
		_columns = newVal;
		_columns0 = newVal - 1;
		_numTiles = newVal * _rows;
		_numTiles0 = _numTiles - 1;
		_tilesReady = false;
	}
}

bool TilesControl::hilite_blank_get() const
{
	return _hiliteBlank;
}

void TilesControl::hilite_blank_set(const bool newVal)
{
	if (_hiliteBlank != newVal)
	{
		_hiliteBlank = newVal;
		update();
	}
}

Color TilesControl::hilite_blank_color_get() const
{
	return _hiliteBlankColor;
}
	
void TilesControl::hilite_blank_color_set(const Color newVal)
{
	if (_hiliteBlankColor != newVal)
	{
		_hiliteBlankColor = newVal;
		if (_hiliteBlank)
			update();
	}
}
	
bool TilesControl::keep_aspect_get() const
{
	return _keepAspect;
}

void TilesControl::keep_aspect_set(const bool newVal)
{
	if (_keepAspect != newVal)
	{
		_keepAspect = newVal;
		recalc_tiles(true);
	}
}

String TilesControl::image_path_get() const
{
	return _imagePath;
}
	
void TilesControl::image_path_set(const String newVal)
{
	if (_imagePath != newVal)
	{
		_imagePath = newVal;
		if (newVal.length() > 0)
			_image = load_image(_imagePath);
		else if (_image.is_valid())
			_image.unref();
	}
}
	
void TilesControl::load_game(const String path)
{
	FUNC_("TilesControl::load_game");
		
	_movesEnabled = false;

	Ref<File> inp = memnew(File);
	Error err = inp->open(path, File::ModeFlags::READ);
	ERR_FAIL_COND(err != Error::OK);
		
	// Read file format version.
	int v = inp->get_16();
	ERR_FAIL_COND(v != FILE_VERSION);

	// Load columns and rows.
	int co = inp->get_8();
	int ro = inp->get_8();

	// Load UseImage.
	bool useImage_ = inp->get_8() != 0;

	// Load image path.
	String imagePath_ = inp->get_pascal_string();

	// Load tiles order.
	int numTiles_ = inp->get_16();
	PackedInt32Array order;
	order.resize(numTiles_);
	for (int i = 0; i < numTiles_; ++i)
		order[i] = inp->get_16();

	// Read blank tile index.
	int e = inp->get_16();
	int eid = inp->get_16();

	// Read moves so far
	int m = inp->get_32();

	bool numbersVisible_ = inp->get_8() != 0;
	bool outlinesVisible_ = inp->get_8() != 0;
	bool keepAspect_ = inp->get_8() != 0;
	
	// Done loading data
	inp->close();

	// Re/initialize all settings for loaded game
	if (useImage_)
	{
		_imagePath = imagePath_;
		_image = load_image(_imagePath);
	}
	else
	{
		_imagePath = "";
	}
	columns_set(co);
	rows_set(ro);
	_tilesOrder = order;
	_moves = m;
	_empty = e;
	_emptyId = eid;
	_numbersVisible = numbersVisible_;
	_outlinesVisible = outlinesVisible_;
	_keepAspect = keepAspect_;
	++_movedSignal;

	// Let owner know a game was loaded
	emit_signal("loaded");
		
	// All ready
	_readyToRun = true;
	_movesEnabled = true;
	_tilesReady = false;
	call_deferred("update");
	call_deferred("calc_movables");
}

Ref<Image> TilesControl::load_image(const String path)
{
	FUNC_("TilesControl::load_image");

	Ref<Image> i;
	FUNCP_("Loading image from \"", path, "\"");
	if (path.substr(0, 4) == "res:")
	{
		i = ResourceLoader::get_singleton()->load(path, "Image", ResourceLoader::CacheMode::CACHE_MODE_IGNORE);
	}
	else
	{
		i = Ref(memnew(Image));
		i->load(path);
	}
	return i;
}

bool TilesControl::movable_get() const
{
	return _movesEnabled;
}
	
void TilesControl::movable_set(const bool newVal)
{
	if (_movesEnabled != newVal)
		_movesEnabled = newVal;
}
	
void TilesControl::move_down()
{
	FUNC_("TilesControl::move_down");
		
	int newEmpty = _tilesOrder[_moveDownIndex];
	_tilesOrder[_moveDownIndex] = _emptyId;
	_tilesOrder[_empty] = newEmpty;
	_empty = _moveDownIndex;
	++_moves;
	++_movedSignal;
	calc_movables();
	update();
}

void TilesControl::move_left()
{
	FUNC_("TilesControl::move_up");
		
	int newEmpty = _tilesOrder[_moveLeftIndex];
	_tilesOrder[_moveLeftIndex] = _emptyId;
	_tilesOrder[_empty] = newEmpty;
	_empty = _moveLeftIndex;
	++_moves;
	++_movedSignal;
	calc_movables();
	update();
}

void TilesControl::move_right()
{
	FUNC_("TilesControl::move_right");
		
	int newEmpty = _tilesOrder[_moveRightIndex];
	_tilesOrder[_moveRightIndex] = _emptyId;
	_tilesOrder[_empty] = newEmpty;
	_empty = _moveRightIndex;
	++_moves;
	++_movedSignal;
	calc_movables();
	update();
}
	
void TilesControl::move_up()
{
	FUNC_("TilesControl::move_up");
		
	int newEmpty = _tilesOrder[_moveUpIndex];
	_tilesOrder[_moveUpIndex] = _emptyId;
	_tilesOrder[_empty] = newEmpty;
	_empty = _moveUpIndex;
	++_moves;
	++_movedSignal;
	calc_movables();
	update();
}

Ref<Font> TilesControl::numbers_font_get() const
{
	return _numbersFont;
}
	
void TilesControl::numbers_font_set(Ref<Font> newVal)
{
	_numbersFont = newVal;
	if (_numbersVisible)
		update();
}
	
Color TilesControl::numbers_color_get() const
{
	return _numbersColor;
}
	
void TilesControl::numbers_color_set(const Color newVal)
{
	if (_numbersColor != newVal)
	{
		_numbersColor = newVal;
		if (_numbersVisible)
			update();
	}
}
	
bool TilesControl::numbers_visible_get() const
{
	return _numbersVisible;
}
	
void TilesControl::numbers_visible_set(bool newVal)
{
	if (_numbersVisible != newVal)
	{
		_numbersVisible = newVal;
		update();
	}
}
	
Color TilesControl::outlines_color_get() const
{
	return _outlinesColor;
}
	
void TilesControl::outlines_color_set(const Color newVal)
{
	if (_outlinesColor != newVal)
	{
		_outlinesColor = newVal;
		if (_outlinesVisible)
			update();
	}
}
	
bool TilesControl::outlines_visible_get() const
{
	return _outlinesVisible;
}
	
void TilesControl::outlines_visible_set(const bool newVal)
{
	if (_outlinesVisible != newVal)
	{
		_outlinesVisible = newVal;
		update();
	}
}
	
void TilesControl::recalc_tiles(const bool forced)
{
	FUNC_("TilesControl::recalc_tiles");
		
	if (!_readyToRun)
		return;

	ERR_FAIL_COND(_columns < 3 || _columns > 99);
	ERR_FAIL_COND(_rows < 3 || _rows > 99);

	// Stop processing that relies on tiles being set.
	_tilesReady = false;

	// Reload and resize image if size changed
	Vector2 canvasSize = get_size();
	// In godot 3.x, this can be called before screen/window/canvas is set correctly,
	// so just bail.  It will get called again with the correct settings implemented.
	if (canvasSize.x <= 0 || canvasSize.y <= 0)
	{
		FUNCP_("invalid size = ", canvasSize);
		return;
	}
	if (useImage())
	{
		Ref<Image> i;
		if (forced || !_image.is_valid() || canvasSize.x != _image->get_width()
			|| canvasSize.y != _image->get_height())
		{
			_image = load_image(_imagePath);
			if (_keepAspect)
			{
				const Vector2 imageSize = _image->get_size();
				const float h = canvasSize.x * (imageSize.y / imageSize.x);
				const float w = canvasSize.y * (imageSize.x / imageSize.y);
				const Vector2 newSize = h <= canvasSize.y ? Vector2(canvasSize.x, h) : Vector2(w, canvasSize.y);
				if (newSize.x < canvasSize.x || newSize.y < canvasSize.y)
				{
					// Resize original image
					Ref<Image> tempImage = _image->duplicate();
					tempImage->resize(newSize.x, newSize.y);

					// Create and fill final image with transparency
					i.instantiate();
					i->create(canvasSize.x, canvasSize.y, false, tempImage->get_format());
					i->fill(_backgroundColor);
					
					// Copy resized image into final image, centered
					const Rect2i srcRect(Vector2(0, 0), newSize);
					Vector2i dst(0, 0);
					if (newSize.x < canvasSize.x)
					{
						dst.x = canvasSize.x - newSize.x;
						dst.x /= 2;
					}
					if (newSize.y < canvasSize.y)
					{
						dst.y = canvasSize.y - newSize.y;
						dst.y /= 2;
					}
					i->blit_rect(*tempImage, srcRect, dst);
				}
				else
				{
					i = _image->duplicate();
					i->resize(newSize.x, newSize.y);
				}
			}
			else
			{
				i = _image->duplicate();
				i->resize(canvasSize.x, canvasSize.y);
			}
		}
		Ref<ImageTexture> it = ImageTexture::create_from_image(i);
		_tilesTexture = it;
	}

	// Determine width and height of tiles from our size.
	_tileSize.x = (canvasSize.x - (_columns0 * _spacing.x) - 4) / _columns;
	_tileSize.y = (canvasSize.y - (_rows0 * _spacing.y) - 5) / _rows;
	if (_tileSize.x <= 0 || _tileSize.y <= 0)
	{
		// If this happens, its a fluke at startup and this will get called again
		// which will be succesful.  In Godot 4+, this doesn't happen.
		FUNCP_("invalid _tileSize = ", _tileSize);
		return;
	}

	// Calculate the bounding boxes for each tile for both display and image
	if (!_tilesRectScreen.is_empty())
	{
		_tilesRectScreen.clear();
		_tilesRectTexture.clear();
	}
	_tilesRectScreen.resize(_numTiles);
	_tilesRectTexture.resize(_numTiles);
	for (int row = 0; row < _rows; ++row)
	{
		for (int col = 0; col < _columns; ++col)
		{
			const int idx = (row * _columns) + col;
			_tilesRectScreen[idx] = Rect2(Vector2(2 + (col * _tileSize.x) + (col * _spacing.x),
												  2 + (row * _tileSize.y) + (row * _spacing.y)),
										  _tileSize);
			_tilesRectTexture[idx] = Rect2(Vector2(col * _tileSize.x, row * _tileSize.y), _tileSize);
		}
	}
	_tilesReady = true;
	call_deferred("update");
}

void TilesControl::reset_tiles()
{
	FUNC_("TilesControl::reset_tiles");
		
	// Initial blank tile is last tile.
	_empty = _numTiles0;
	_emptyId = _empty;

	// Need some random numbers.
	Ref<RandomNumberGenerator> rng = memnew(RandomNumberGenerator);
	rng->randomize();

	// Determine tile order.
	if (!_tilesOrder.is_empty())
		_tilesOrder.clear();
	_tilesOrder.resize(_numTiles);
	for (int i = 0; i < _numTiles; ++i)
		_tilesOrder[i] = 9999;
	int order;
	for(int i = 0; i < _numTiles; ++i)
	{
		if (i == _empty)
		{
			_tilesOrder[i] = _emptyId;
		}
		else
		{
			order = rng->randi_range(0, _numTiles0 - 1);
			if (i > 0)
			{
				int n = 0;
				while (n < i)
				{
					for (n = 0; n < i; ++n)
					{
						if (_tilesOrder[n] == order)
						{
							order = rng->randi_range(0, _numTiles0 - 1);
							break;
						}
					}
				}
			}
			else
			{
				if (order == _empty)
				{
					while (order == _empty)
						order = rng->randi_range(0, _numTiles0 - 1);
				}
			}
			_tilesOrder[i] = order;
		}
	}

	// Initialized, but queue movement and tiles calc.
	_readyToRun = true;
	_movesEnabled = true;
	_tilesReady = false;
	call_deferred("calc_movables");
}
	
int TilesControl::rows_get() const
{
	return _rows;
}
	
void TilesControl::rows_set(const int newVal)
{
	ERR_FAIL_COND(newVal < 3 || newVal > 99);
		
	if (_rows != newVal)
	{
		_rows = newVal;
		_rows0 = newVal - 1;
		_numTiles = newVal * _columns;
		_numTiles0 = _numTiles - 1;
		_tilesReady = false;
	}
}
	
void TilesControl::save_game(const String path)
{
	FUNC_("TilesControl::save_game");
		
	if (!_readyToRun)
		return;

	Ref<File> sav = memnew(File);
	sav->open(path, File::ModeFlags::WRITE);

	// Save file version
	sav->store_16(FILE_VERSION);

	// Save TilesSize
	sav->store_8(_columns);
	sav->store_8(_rows);

	// Save UseImage state
	sav->store_8(useImage() ? 1 : 0);

	// Save image path
	sav->store_pascal_string(_imagePath);

	// Save tiles order
	sav->store_16(_numTiles);
	for (int index = 0; index < _numTiles; ++index)
		sav->store_16(_tilesOrder[index]);

	// Save index of empty.
	sav->store_16(_empty);
	sav->store_16(_emptyId);

	// Save number of moves so far.
	sav->store_32(_moves);

	// Visible outlines and number
	sav->store_8(_numbersVisible ? 1 : 0);
	sav->store_8(_outlinesVisible ? 1 : 0);

	// Keep aspect
	sav->store_8(_keepAspect ? 1 : 0);
	
	// All done
	sav->close();

	// Let owner know a game was saved
	emit_signal("saved");
}

Vector2 TilesControl::spacing_get() const
{
	return _spacing;
}

void TilesControl::spacing_set(const Vector2 newVal)
{
	if (_spacing != newVal)
		_spacing = newVal;
}

void TilesControl::start()
{
	FUNC_("TilesControl::start");
		
	reset_tiles();
}
	
int TilesControl::tiles_count_get() const
{
	return _numTiles;
}

void TilesControl::tiles_count_set(const int newVal)
{
	// Not allowed to set this externally, but it can be useful so its
	// read only (gdextension doesn't allow this as far as I can tell).
	CRASH_NOW();
}
