#include "TilesControl.hpp"
#include <File.hpp>
#include <InputEventMouseButton.hpp>
#include <OS.hpp>
#include <RandomNumberGenerator.hpp>
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>
#include "auto_free.hpp"
#include "function.hpp"

// #define TEST_WON 1

using namespace godot;

namespace godot
{
	const int TilesControl::FILE_VERSION = 1;
	
	void TilesControl::_register_methods()
	{
		FUNC_("TilesControl::_register_methods");
		
		// API
		register_method("_draw", &TilesControl::_draw);
		register_method("_init", &TilesControl::_init);
		register_method("_input", &TilesControl::_input);
		register_method("_physics_process", &TilesControl::_physics_process);
		register_method("_ready", &TilesControl::_ready);
		register_method("_unhandled_input", &TilesControl::_unhandled_input);
		register_method("calc_movables", &TilesControl::calc_movables);
		register_method("load_game", &TilesControl::load_game);
		register_method("recalc_tiles", &TilesControl::recalc_tiles);
		register_method("save_game", &TilesControl::save_game);
		register_method("start", &TilesControl::start);
		
		// Properties
		register_property<TilesControl, bool>("can_move_down", &TilesControl::can_move_down_set, &TilesControl::can_move_down_get, false);
		register_property<TilesControl, bool>("can_move_left", &TilesControl::can_move_left_set, &TilesControl::can_move_left_get, false);
		register_property<TilesControl, bool>("can_move_right", &TilesControl::can_move_right_set, &TilesControl::can_move_right_get, false);
		register_property<TilesControl, bool>("can_move_up", &TilesControl::can_move_up_set, &TilesControl::can_move_up_get, false);
		register_property<TilesControl, int>("columns", &TilesControl::columns_set, &TilesControl::columns_get, 4);
		register_property<TilesControl, String>("image_path", &TilesControl::image_path_set, &TilesControl::image_path_get, String());
		register_property<TilesControl, bool>("movable", &TilesControl::movable_set, &TilesControl::movable_get, false);
		register_property<TilesControl, Ref<Font>>("number_font", &TilesControl::numbers_font_set, &TilesControl::numbers_font_get, nullptr);
		register_property<TilesControl, Color>("numbers_color", &TilesControl::numbers_color_set, &TilesControl::numbers_color_get, Color(0.8, 0.8, 0.8, 1));
		register_property<TilesControl, bool>("numbers_visible", &TilesControl::numbers_visible_set, &TilesControl::numbers_visible_get, true);
		register_property<TilesControl, Color>("outlines_color", &TilesControl::outlines_color_set, &TilesControl::outlines_color_get, Color(0.5, 0.5, 0.5, 1));
		register_property<TilesControl, bool>("outlines_visible", &TilesControl::outlines_visible_set, &TilesControl::outlines_visible_get, false);
		register_property<TilesControl, int>("rows", &TilesControl::rows_set, &TilesControl::rows_get, 4);
		register_property<TilesControl, Vector2>("spacing", &TilesControl::spacing_set, &TilesControl::spacing_get, Vector2());
		register_property<TilesControl, int>("tiles_count", &TilesControl::tiles_count_set, &TilesControl::tiles_count_get, 16);

		// Signals
		register_signal<TilesControl>((char*) "loaded");
		register_signal<TilesControl>((char*) "moved", "count", GODOT_VARIANT_TYPE_INT);
		register_signal<TilesControl>((char*) "saved");
		register_signal<TilesControl>((char*) "won");
	}

	TilesControl::TilesControl() :
		_gameComplete(false),
		_tilesOrder(nullptr),
		_tilesRectScreen(nullptr),
		_tilesRectTexture(nullptr)
	{
		FUNC_("TilesControl::TilesControl");
	}

	TilesControl::~TilesControl()
	{
		FUNC_("TilesControl::~TilesControl");
		if (_tilesRectScreen != nullptr)
		{
			auto_free tilesRectScreen(_tilesRectScreen);
			_tilesRectScreen = nullptr;
		}
		if (_tilesRectTexture != nullptr)
		{
			auto_free tilesRectTexture(_tilesRectTexture);
			_tilesRectTexture = nullptr;
		}
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
						draw_string(_numbersFont, Vector2(_tilesRectScreen[index].get_position().x + 5,
														  _tilesRectScreen[index].get_position().y + 10 + (extent.y / 2)),
									name, _numbersColor);
					}

					// Add an outline if desired
					if (_outlinesVisible)
					{
						Rect2 area = _tilesRectScreen[index];
						Vector2 v = area.get_position();
						--v.x;
						++v.y;
						area.set_position(v);
						v = area.get_size();
						++v.x;
						++v.y;
						area.set_size(v);
						draw_rect(area, _outlinesColor, false);
					}
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
								name, _numbersColor);
				}
			} // Not blank tile
			++index;
		}
	}
	
	void TilesControl::_init()
	{
		FUNC_("TilesControl::_init");

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
		_outlinesColor = Color(0, 0, 0, 1);
		_outlinesVisible;
		_readyToRun = false;
		_rows = 4;
		_rows0 = 3;
		_spacing = Vector2(5, 5);
		_tilesOrder = nullptr;
		_tilesReady = false;
		_tileSize = Vector2();
	}

	void TilesControl::_input(const Ref<InputEvent> ev)
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
		if (iemb->is_doubleclick())
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
			if (_tilesRectScreen[_moveDownIndex].has_point(position))
			{
				accept_event();
				move_down();
				return;
			}
		}
		if (_canMoveLeft)
		{
			if (_tilesRectScreen[_moveLeftIndex].has_point(position))
			{
				accept_event();
				move_left();
				return;
			}
		}
		if (_canMoveRight)
		{
			if (_tilesRectScreen[_moveRightIndex].has_point(position))
			{
				accept_event();
				move_right();
				return;
			}
		}
		if (_canMoveUp)
		{
			if (_tilesRectScreen[_moveUpIndex].has_point(position))
			{
				accept_event();
				move_up();
				return;
			}
		}
	}
	
	void TilesControl::_physics_process(const float delta)
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
	
	void TilesControl::_unhandled_input(const Ref<InputEvent> ev)
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
		const int row = _empty / _columns;
		const int column = _empty % _columns;

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
	
	void TilesControl::can_move_down_set(bool newVal)
	{
		CRASH_NOW();
	}
	
	bool TilesControl::can_move_left_get() const
	{
		return _canMoveLeft;
	}
	
	void TilesControl::can_move_left_set(bool newVal)
	{
		CRASH_NOW();
	}
	
	bool TilesControl::can_move_right_get() const
	{
		return _canMoveRight;
	}
	
	void TilesControl::can_move_right_set(bool newVal)
	{
		CRASH_NOW();
	}
	
	bool TilesControl::can_move_up_get() const
	{
		return _canMoveUp;
	}
	
	void TilesControl::can_move_up_set(bool newVal)
	{
		CRASH_NOW();
	}
	
	int TilesControl::columns_get() const
	{
		return _columns;
	}
	
	void TilesControl::columns_set(int newVal)
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
	
	String TilesControl::image_path_get() const
	{
		return _imagePath;
	}
	
	void TilesControl::image_path_set(String newVal)
	{
		if (_imagePath != newVal)
		{
			_imagePath = newVal;
			if (newVal.length() > 0)
				_image = Ref(load_image(_imagePath));
			else if (_image.is_valid())
				_image.unref();
		}
	}
	
	void TilesControl::load_game(const String path)
	{
		FUNC_("TilesControl::load_game");
		
		_movesEnabled = false;

		Ref<File> inp(File::_new());
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
		int* order = reinterpret_cast<int*>(api->godot_alloc(numTiles_ * sizeof(int)));
		for (int i = 0; i < numTiles_; ++i)
			order[i] = inp->get_16();

		// Read blank tile index.
		int e = inp->get_16();
		int eid = inp->get_16();

		// Read moves so far
		int m = inp->get_32();

		bool numbersVisible_ = inp->get_8() != 0;
		bool outlinesVisible_ = inp->get_8() != 0;
		
		// Done loading data
		inp->close();

		if (useImage_)
		{
			_imagePath = imagePath_;
			_image = Ref(load_image(_imagePath));
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
		++_movedSignal;

		// Let owner know a game was loaded
		emit_signal("loaded");
		
		// Reset tiles
		call_deferred("recalc_tiles");
		call_deferred("calc_movables");

		// All ready
		_readyToRun = true;
		_movesEnabled = true;
		_tilesReady = false;
		call_deferred("update");
	}

	Ref<Image> TilesControl::load_image(const String path)
	{
		FUNC_("TilesControl::load_image");

		Ref<Image> i;
		Godot::print("TilesControl::load_image: Loading image from \"{0}\"", path);
		if (path.substr(0, 4) == "res:")
		{
			i = ResourceLoader::get_singleton()->load(path, "Image", true);
		}
		else
		{
			i = Ref(Image::_new());
			i->load(path);
		}
		return i;
	}

	bool TilesControl::movable_get() const
	{
		return _movesEnabled;
	}
	
	void TilesControl::movable_set(bool newVal)
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
	
	void TilesControl::numbers_color_set(Color newVal)
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
	
	void TilesControl::outlines_color_set(Color newVal)
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
	
	void TilesControl::outlines_visible_set(bool newVal)
	{
		if (_outlinesVisible != newVal)
		{
			_outlinesVisible = newVal;
			update();
		}
	}
	
	void TilesControl::recalc_tiles()
	{
		FUNC_("TilesControl::recalc_tiles");
		
		if (!_readyToRun)
			return;

		ERR_FAIL_COND(_columns < 3 || _columns > 99);
		ERR_FAIL_COND(_rows < 3 || _rows > 99);

		// Stop processing that relies on tiles being set.
		_tilesReady = false;

		// Reload and resize image if size changed
		Vector2 size = get_size();
		// In godot 3.x, this can be called before screen/window/canvas is set correctly,
		// so just bail.  It will get called again with the correct settings implemented.
		if (size.x <= 0 || size.y <= 0)
		{
			Godot::print("TilesControl::recalc_tiles:  invalid size = {0}", size);
			return;
		}
		if (useImage())
		{
			if (!_image.is_valid() || size.x != _image->get_width() || size.y != _image->get_height())
			{
				_image = load_image(_imagePath);
				_image->resize(size.x, size.y);
			}
			Ref<ImageTexture> it = ImageTexture::_new();
			it->create_from_image(_image);
			_tilesTexture = it;
		}

		// Determine width and height of tiles from our size.
		_tileSize.x = (size.x - (_columns0 * _spacing.x)) / _columns;
		_tileSize.y = (size.y - (_rows0 * _spacing.y)) / _rows;
		if (_tileSize.x <= 0 || _tileSize.y <= 0)
		{
			// If this happens, its a fluke at startup and this will get called again
			// which will be succesful.  In Godot 4+, this doesn't happen.
			Godot::print("TilesControl::recalc_tiles:  invalid _tileSize = {0}", _tileSize);
			return;
		}

		// Calculate the bounding boxes for each tile for both display and image
		if (_tilesRectScreen != nullptr)
		{
			auto_free tilesRectScreen(_tilesRectScreen);
			_tilesRectScreen = nullptr;
		}
		if (_tilesRectTexture != nullptr)
		{
			auto_free tilesRectTexture(_tilesRectTexture);
			_tilesRectTexture = nullptr;
		}
		_tilesRectScreen = reinterpret_cast<Rect2*>(api->godot_alloc(_numTiles * sizeof(Rect2)));
		_tilesRectTexture = reinterpret_cast<Rect2*>(api->godot_alloc(_numTiles * sizeof(Rect2)));
		for (int row = 0; row < _rows; ++row)
		{
			for (int col = 0; col < _columns; ++col)
			{
				const int idx = (row * _columns) + col;
				_tilesRectScreen[idx] = Rect2(Vector2((col * _tileSize.x) + (col * _spacing.x),
													  (row * _tileSize.y) + (row * _spacing.y)),
											  _tileSize);
				_tilesRectTexture[idx] = Rect2(Vector2(col * _tileSize.x, row * _tileSize.y), _tileSize);
			}
		}
		_tilesReady = true;
	}
		
	void TilesControl::reset_tiles()
	{
		FUNC_("TilesControl::reset_tiles");
		
		// Initial blank tile is last tile.
		_empty = _numTiles0;
		_emptyId = _empty;

		// Need some random numbers.
		Ref<RandomNumberGenerator> rng = RandomNumberGenerator::_new();
		rng->randomize();

		// Determine tile order.
		if (_tilesOrder != nullptr)
		{
			auto_free tilesOrder(_tilesOrder);
			_tilesOrder = nullptr;
		}
		_tilesOrder = reinterpret_cast<int*>(api->godot_alloc(sizeof(int) * _numTiles));
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
		call_deferred("recalc_tiles");
		call_deferred("calc_movables");
	}
	
	int TilesControl::rows_get() const
	{
		return _rows;
	}
	
	void TilesControl::rows_set(int newVal)
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

		Ref<File> sav = File::_new();
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
	   
		// All done
		sav->close();

		// Let owner know a game was saved
		emit_signal("saved");
	}

	Vector2 TilesControl::spacing_get() const
	{
		return _spacing;
	}

	void TilesControl::spacing_set(Vector2 newVal)
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

	void TilesControl::tiles_count_set(int newVal)
	{
		// Not allowed to set this externally, but it can be useful
		CRASH_NOW();
	}
}
