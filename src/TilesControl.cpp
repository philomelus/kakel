#include "TilesControl.hpp"
#include <File.hpp>
#include <Image.hpp>
#include <ImageTexture.hpp>
#include <InputEventMouseButton.hpp>
#include <RandomNumberGenerator.hpp>
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>

using namespace godot;

namespace {
	class TilesControlImpl : public kakel::TilesControl
	{
		friend kakel::TilesControl;
		
	protected:
		// Index within tiles of the empty tile.
		int _empty;

		// Index value of the empty tile (always the last global tile index,
		// i.e. `_numTiles0` below).
		int _emptyId;

		// When false, no movement is allowed.
		bool _movesEnabled;

		// Columns in tiles.
		int _columns;
		int _columns0;

		// Rows in tiles.
		int _rows;
		int _rows0;

		// Total number of tiles.
		int _numTiles;
		int _numTiles0;

		// Spacing between each tile.
		godot::Vector2 _spacing;

		// Font for drawing numerical id's of tiles.
		godot::Ref<godot::Font> _numbersFont;

		// Outline color for non-image tiles.
		godot::Color _outlinesColor;

		// Color of font for tile numbers.
		godot::Color _numbersColor;

		// Whn true, add numbers to all tiles (incuding images).
		bool _numbersVisible;

		// Whn true, add outline to all tiles (incuding images).
		bool _outlinesVisible;

		// Image used for tiles.
		godot::Ref<godot::Image> _image;

		// Path to image file.
		godot::String _imagePath;

		// `_tilesImage` converted to texture.  Only valid when `_tilesReady` is true.
		godot::Ref<godot::ImageTexture> _tilesTexture;

		// Size of each tile in pixels.  Only valid when `_tilesReady` is true.
		Vector2 _tileSize;

		// Data for each tile.  Each tile is a two item Rect array, and this array
		// is _numTiles * Rect2[2].
		godot::Rect2* _tiles;

		// When true, `_tiles`, `_tilesTexture`, and `_tileSize` are valid.
		bool _tilesReady = false;

		// Actual positions of each tile (i.e. index 0 = tile at upper left).
		// Columns before rows (i.e. index 0 = row 0/col 0, index 1 = row 0/col 1,
		// index 2 = row 0/col 2, index 3 = row 1/col 0, etc. if `Columns` is 3).
		int* _tilesOrder;

		// When true, `_moveDownIndex` holds index within `_tiles` that can move down.
		bool _canMoveDown;

		// When true, `_moveLeftIndex` holds index within `_tiles` than can move left.
		bool _canMoveLeft;

		// When true, `_moveRightIndex` holds index within `_tiles` than can move right.
		bool _canMoveRight;

		// When true, `_moveUpIndex` holds index within `_tiles` than can move up.
		bool _canMoveUp;

		// Index within `_tilesOrder` that can move down. Valid only when `_canMoveDown`
		// is true.
		int _moveDownIndex;

		// Index within `_tiles_order` that can moves left. Valid only when `_canMoveLeft`
		// is true.
		int _moveLeftIndex;

		// Index within `_tiles_order` that can move right. Valid only when `_canMoveRight`
		// is true.
		int _moveRightIndex;

		// Index within `_tiles_order` that can move up. Valid only when `_canMoveUp`
		// is true.
		int _moveUpIndex;

		// Number of movements made since initial display.
		int _moves;

		// When > 0, a move has occurred.  This hrottles the `moved` signal.
		int _movedSignal;

		// When true internal variables are initialized and ready to use.
		bool _readyToRun;

		// Time since last EmitSignal for moves.
		float _lastSignal;

		// Time since last CheckComplete().
		float _lastWinner;

		SceneTree* _tree;

		bool useImage() const { return _imagePath.length() > 0; }

	public:
		// For save files, version of file format.
		static const int FILE_VERSION = 1;
		
		TilesControlImpl()
		{
		}

		~TilesControlImpl()
		{
			if (_tiles != nullptr)
			{
				Rect2* tmp = _tiles;
				_tiles = nullptr;
				godot::api->godot_free(tmp);
			}
		}
		
	protected:
		void calc_movables()
		{
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

		bool check_complete()
		{
			for(int index = 0; index < _numTiles; ++index)
			{
				if (_tilesOrder[index] != index)
					return false;
			}
			_movesEnabled = false;
			emit_signal("won");
			return true;
		}
		
		void draw()
		{
			if (!_readyToRun || !_tilesReady)
				return;
			Vector2 extent;
			int index = 0;
			int tileIndex;
			for (int i = 0; i < _numTiles; ++i)
			{
				tileIndex = _tilesOrder[i];
				if (tileIndex != _emptyId)
				{
					if (useImage())
					{
						draw_texture_rect_region(_tilesTexture, _tiles[index * 2],
												 _tiles[(_tilesOrder[index] * 2) + 1]);
						if (_numbersVisible)
						{
							String name(tileIndex + 1);
							extent = _numbersFont->get_string_size(name);
							draw_string(_numbersFont, Vector2(_tiles[index * 2].get_position().x + 5,
															  _tiles[index * 2].get_position().y + 10 + (extent.y / 2)),
									   name, _numbersColor);
						}

						if (_outlinesVisible)
						{
							Rect2 area = _tiles[index * 2];
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
					else
					{
						Rect2 area = _tiles[index * 2];
						String name(tileIndex + 1);
						extent = _numbersFont->get_string_size(name);
						draw_rect(area, _outlinesColor, false);
						draw_string(_numbersFont, Vector2(area.get_position().x + ((area.get_size().x - extent.x) / 2),
														  area.get_position().y + ((area.get_size().y - extent.y) / 2) + extent.y),
									name, _numbersColor);
					}
				}
				++index;
			}
		}
		
		void input(const godot::Ref<godot::InputEvent> ev)
		{
			// Do nothing while paused or uninitialized
			if (!_readyToRun || !_tilesReady || _tree->is_paused() || !_movesEnabled)
				return;

			// Only handle mouse clicks here
			// TODO:  Don't like using a (dangerous) c-style cast ... but I couldn't
			//        get anything else to work!
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

			// Take appropriate action if needed
			if (_canMoveDown)
			{
				if (_tiles[_moveDownIndex * (2 * sizeof(Rect2))].has_point(evmb->get_position()))
				{
					accept_event();
					move_down();
					return;
				}
			}
			if (_canMoveLeft)
			{
				if (_tiles[_moveLeftIndex * (2 * sizeof(Rect2))].has_point(evmb->get_position()))
				{
					accept_event();
					move_left();
					return;
				}
			}
			if (_canMoveRight)
			{
				if (_tiles[_moveRightIndex * (2 * sizeof(Rect2))].has_point(evmb->get_position()))
				{
					accept_event();
					move_right();
					return;
				}
			}
			if (_canMoveUp)
			{
				if (_tiles[_moveUpIndex * (2 * sizeof(Rect2))].has_point(evmb->get_position()))
				{
					accept_event();
					move_up();
					return;
				}
			}
		}
	
		void load_game(String path)
		{
			_movesEnabled = false;

			Ref<File> inp = File::_new();
			inp->open(path, File::ModeFlags::READ);

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
			int* order = reinterpret_cast<int*>(godot::api->godot_alloc(numTiles_ * sizeof(int)));
			for (int i = 0; i < numTiles_; ++i)
				order[i] = inp->get_16();

			// Read blank tile index.
			int e = inp->get_16();
			int eid = inp->get_16();

			// Read moves so far
			int m = inp->get_32();

			bool numbersVisible_ = inp->get_8() != 0;

			// Done loading data
			inp->close();

			if (useImage())
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
			++_movedSignal;

			// Reset tiles
			call_deferred("recalc_tiles");
			call_deferred("calc_movables");

			// All ready
			_readyToRun = true;
			_movesEnabled = true;
			_tilesReady = false;
			call_deferred("update");
		}
		
		Ref<Image> load_image(String& path)
		{
			if (path.substr(0, 4) == "res:")
			{
				return Ref(ResourceLoader::get_singleton()->load(path, "Image", true));
			}
			else
			{
				Ref<Image> i = Image::_new();
				i->load(path);
				return i;
			}
		}

		void move_down()
		{
			int newEmpty = _tilesOrder[_moveDownIndex];
			_tilesOrder[_moveDownIndex] = _emptyId;
			_tilesOrder[_empty] = newEmpty;
			_empty = _moveDownIndex;
			++_moves;
			++_movedSignal;
			calc_movables();
			update();
		}

		void move_left()
		{
			int newEmpty = _tilesOrder[_moveLeftIndex];
			_tilesOrder[_moveLeftIndex] = _emptyId;
			_tilesOrder[_empty] = newEmpty;
			_empty = _moveLeftIndex;
			++_moves;
			++_movedSignal;
			calc_movables();
			update();
		}

		void move_right()
		{
			int newEmpty = _tilesOrder[_moveRightIndex];
			_tilesOrder[_moveRightIndex] = _emptyId;
			_tilesOrder[_empty] = newEmpty;
			_empty = _moveRightIndex;
			++_moves;
			++_movedSignal;
			calc_movables();
			update();
		}
		void move_up()
		{
			int newEmpty = _tilesOrder[_moveUpIndex];
			_tilesOrder[_moveUpIndex] = _emptyId;
			_tilesOrder[_empty] = newEmpty;
			_empty = _moveUpIndex;
			++_moves;
			++_movedSignal;
			calc_movables();
			update();
		}

		void physics_process(const float delta)
		{
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
	
		void ready()
		{
			_readyToRun = false;
			_movesEnabled = false;
		}
	
		void recalc_tiles()
		{
			if (!_readyToRun)
				return;

			ERR_FAIL_COND(_columns == 0);
			ERR_FAIL_COND(_rows == 0);

			// Stop processing that relies on tiles being set.
			_tilesReady = false;

			// Reload and resize image if size changed
			Vector2 rect_size = get_size();
			if (useImage())
			{
				if (!_image.is_valid() || rect_size.x != _image->get_width() || rect_size.y != _image->get_height())
				{
					_image = load_image(_imagePath);
					_image->resize(rect_size.x, rect_size.y);
				}
				Ref<ImageTexture> it = ImageTexture::_new();
				it->create_from_image(_image);
				_tilesTexture = it;
			}

			// Determine width and height of tiles from our size.
			_tileSize.x = (rect_size.x - (_columns0 * _spacing.x)) / _columns;
			_tileSize.y = (rect_size.y - (_rows0 * _spacing.y)) / _rows;
			if (_tileSize.x <= 0 || _tileSize.y <= 0)
			{
				// If this happens, its a fluke at startup and this will get called again
				// which will be succesful.  In Godot 4+, this doesn't happen.
				return;
			}

			// Calculate the bounding boxes for each tile for both display and image
			if (_tiles != nullptr)
			{
				Rect2* tmp = _tiles;
				_tiles = nullptr;
				godot::api->godot_free(_tiles);
			}
			_tiles = reinterpret_cast<Rect2*>(godot::api->godot_alloc(_numTiles * (2 * sizeof(Rect2))));
			for (int row = 0; row < _rows; ++row)
			{
				for (int col = 0; col < _columns; ++col)
				{
					const int idx = (row * _columns) + col;
					_tiles[idx] = Rect2(Vector2((col * _tileSize.x) + (col * _spacing.x),
												(row * _tileSize.y) + (row * _spacing.y)),
										_tileSize);
					_tiles[idx + 1] = Rect2(Vector2(col * _tileSize.x, row * _tileSize.y), _tileSize);
				}
			}
			_tilesReady = true;
		}
		
		void reset_tiles()
		{
			// Initial blank tile is last tile.
			_empty = _numTiles0;
			_emptyId = _empty;

			// Need some random numbers.
			Ref<RandomNumberGenerator> rng = RandomNumberGenerator::_new();
			rng->randomize();

			// Determine tile order.
			if (_tilesOrder != nullptr)
			{
				int* tmp = _tilesOrder;
				_tilesOrder = nullptr;
				godot::api->godot_free(tmp);
			}
			_tilesOrder = reinterpret_cast<int*>(godot::api->godot_alloc(sizeof(int) * _numTiles));
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
			call_deferred("RecalcTiles");
			call_deferred("calc_movables");
		}

		void save_game(String path)
		{
			if (!_readyToRun)
				return;

			Ref<File> sav = File::_new();
			sav->open(path, File::ModeFlags::WRITE);

			// Save file version
			sav->store_16(TilesControlImpl::FILE_VERSION);

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

			// Save show numbers.
			sav->store_8(_numbersVisible ? 1 : 0);
	   
			// All done
			sav->close();
		}
		
		void unhandled_input(const godot::Ref<godot::InputEvent> ev)
		{
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
	};
}

// register_property<TilesControl, float>("amplitude", &TilesControl::amplitude, 10.0);
// register_property<TilesControl, float>("speed", &TilesControl::set_speed, &TilesControl::get_speed, 1.0);
// emit_signal("position_changed", this, new_position);

namespace kakel
{
	void TilesControl::_register_methods()
	{
		// API
		register_method("_draw", &TilesControl::_draw);
		register_method("_init", &TilesControl::_init);
		register_method("_input", &TilesControl::_input);
		register_method("_physics_process", &TilesControl::_physics_process);
		register_method("_ready", &TilesControl::_ready);
		register_method("_unhandled_input", &TilesControl::_unhandled_input);
		register_method("load_game", &TilesControl::LoadGame);
		register_method("recalc_tiles", &TilesControl::RecalcTiles);
		register_method("save_game", &TilesControl::SaveGame);
		register_method("start", &TilesControl::Start);
		
		// Properties
		register_property<TilesControl, bool>("can_move_down", &TilesControl::can_move_down_set, &TilesControl::can_move_down_get, false);
		register_property<TilesControl, bool>("can_move_left", &TilesControl::can_move_left_set, &TilesControl::can_move_left_get, false);
		register_property<TilesControl, bool>("can_move_right", &TilesControl::can_move_right_set, &TilesControl::can_move_right_get, false);
		register_property<TilesControl, bool>("can_move_up", &TilesControl::can_move_up_set, &TilesControl::can_move_up_get, false);
		register_property<TilesControl, int>("columns", &TilesControl::columns_set, &TilesControl::columns_get, 4);
		register_property<TilesControl, String>("image_path", &TilesControl::image_path_set, &TilesControl::image_path_get, godot::String());
		register_property<TilesControl, bool>("movable", &TilesControl::movable_set, &TilesControl::movable_get, false);
		register_property<TilesControl, godot::Ref<Font>>("number_font", &TilesControl::numbers_font_set, &TilesControl::numbers_font_get, nullptr);
		register_property<TilesControl, Color>("numbers_color", &TilesControl::numbers_color_set, &TilesControl::numbers_color_get, godot::Color(0.8, 0.8, 0.8, 1));
		register_property<TilesControl, bool>("numbers_visible", &TilesControl::numbers_visible_set, &TilesControl::numbers_visible_get, true);
		register_property<TilesControl, Color>("outlines_color", &TilesControl::outlines_color_set, &TilesControl::outlines_color_get, godot::Color(0.5, 0.5, 0.5, 1));
		register_property<TilesControl, bool>("outlines_visible", &TilesControl::outlines_visible_set, &TilesControl::outlines_visible_get, false);
		register_property<TilesControl, int>("rows", &TilesControl::rows_set, &TilesControl::rows_get, 4);
		register_property<TilesControl, Vector2>("spacing", &TilesControl::spacing_set, &TilesControl::spacing_get, godot::Vector2());
		register_property<TilesControl, int>("tiles_count", &TilesControl::tiles_count_set, &TilesControl::tiles_count_get, 16);

		// Signals
		register_signal<TilesControl>((char*) "moved", "count", GODOT_VARIANT_TYPE_INT);
		register_signal<TilesControl>((char*) "won");
	}

	TilesControl::TilesControl()
	{
		_impl = TilesControlImpl::_new();
	}

	TilesControl::~TilesControl()
	{
		// TODO: Is this how I am supposed to do this?
		godot::api->godot_free(_impl);
		_impl = nullptr;
	}

	void TilesControl::_draw()
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		self->draw();
	}
	
	void TilesControl::_init()
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		self->_columns = 4;
		self->_columns0 = 3;
		self->_imagePath = String();
		self->_lastSignal = 0.0;
		self->_lastWinner = 0.0;
		self->_movedSignal = 0;
		self->_moves = 0;
		self->_movesEnabled = false;
		self->_numTiles = 16;
		self->_numTiles0 = 15;
		self->_numbersColor = Color(0.8, 0.8, 0.8, 1);
		self->_numbersVisible;
		self->_outlinesColor = Color(0, 0, 0, 1);
		self->_outlinesVisible;
		self->_readyToRun = false;
		self->_rows = 4;
		self->_rows0 = 3;
		self->_spacing = Vector2(5, 5);
		self->_tilesOrder = nullptr;
		self->_tilesReady = false;
		self->_tileSize = Vector2();
		self->_tree = get_tree();
		
		//some_other_node->connect("the_signal", this, "my_method");
	}

	void TilesControl::_input(const godot::Ref<godot::InputEvent> ev)
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		self->input(ev);
	}
	
	void TilesControl::_physics_process(const float delta)
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		self->physics_process(delta);
	}
	
	void TilesControl::_ready()
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		self->ready();
	}
	
	void TilesControl::_unhandled_input(const godot::Ref<godot::InputEvent> ev)
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		self->unhandled_input(ev);
	}
	
	bool TilesControl::can_move_down_get() const
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		return self->_canMoveDown;
	}
	
	void TilesControl::can_move_down_set(bool newVal)
	{
		CRASH_NOW();
	}
	
	bool TilesControl::can_move_left_get() const
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		return self->_canMoveLeft;
	}
	
	void TilesControl::can_move_left_set(bool newVal)
	{
		CRASH_NOW();
	}
	
	bool TilesControl::can_move_right_get() const
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		return self->_canMoveRight;
	}
	
	void TilesControl::can_move_right_set(bool newVal)
	{
		CRASH_NOW();
	}
	
	bool TilesControl::can_move_up_get() const
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		return self->_canMoveUp;
	}
	
	void TilesControl::can_move_up_set(bool newVal)
	{
		CRASH_NOW();
	}
	
	int TilesControl::columns_get() const
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		return self->_columns;
	}
	
	void TilesControl::columns_set(int newVal)
	{
		ERR_FAIL_COND(newVal < 3 || newVal > 99);

		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		if (self->_columns != newVal)
		{
			self->_columns = newVal;
			self->_columns0 = newVal - 1;
			self->_numTiles = newVal * self->_rows;
			self->_numTiles0 = self->_numTiles - 1;
			self->_tilesReady = false;
		}
	}
	
	String TilesControl::image_path_get() const
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		return self->_imagePath;
	}
	
	void TilesControl::image_path_set(String newVal)
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		if (self->_imagePath != newVal)
		{
			self->_imagePath = newVal;
			if (newVal.length() > 0)
				self->load_image(self->_imagePath);
		}
	}
	
	void TilesControl::LoadGame(String path)
	{
	}

	bool TilesControl::movable_get() const
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		return self->_movesEnabled;
	}
	
	void TilesControl::movable_set(bool newVal)
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		if (self->_movesEnabled != newVal)
			self->_movesEnabled = newVal;
	}
	
	Ref<Font> TilesControl::numbers_font_get() const
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		return self->_numbersFont;
	}
	
	void TilesControl::numbers_font_set(Ref<Font> newVal)
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		self->_numbersFont = newVal;
		if (self->_numbersVisible)
			update();
	}
	
	Color TilesControl::numbers_color_get() const
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		return self->_numbersColor;
	}
	
	void TilesControl::numbers_color_set(Color newVal)
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		if (self->_numbersColor != newVal)
		{
			self->_numbersColor = newVal;
			if (self->_numbersVisible)
				update();
		}
	}
	
	bool TilesControl::numbers_visible_get() const
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		return self->_numbersVisible;
	}
	
	void TilesControl::numbers_visible_set(bool newVal)
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		if (self->_numbersVisible != newVal)
		{
			self->_numbersVisible = newVal;
			update();
		}
	}
	
	Color TilesControl::outlines_color_get() const
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		return self->_outlinesColor;
	}
	
	void TilesControl::outlines_color_set(Color newVal)
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		if (self->_outlinesColor != newVal)
		{
			self->_outlinesColor = newVal;
			if (self->_outlinesVisible)
				update();
		}
	}
	
	bool TilesControl::outlines_visible_get() const
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		return self->_outlinesVisible;
	}
	
	void TilesControl::outlines_visible_set(bool newVal)
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		if (self->_outlinesVisible != newVal)
		{
			self->_outlinesVisible = newVal;
			update();
		}
	}
	
	void TilesControl::RecalcTiles()
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		self->recalc_tiles();
	}

	int TilesControl::rows_get() const
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		return self->_rows;
	}
	
	void TilesControl::rows_set(int newVal)
	{
		ERR_FAIL_COND(newVal < 3 || newVal > 99);
		
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		if (self->_rows != newVal)
		{
			self->_rows = newVal;
			self->_rows0 = newVal - 1;
			self->_numTiles = newVal * self->_columns;
			self->_numTiles0 = self->_numTiles - 1;
			self->_tilesReady = false;
		}
	}
	
	void TilesControl::SaveGame(String path)
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		self->save_game(path);
	}

	Vector2 TilesControl::spacing_get() const
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		return self->_spacing;
	}

	void TilesControl::spacing_set(Vector2 newVal)
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		if (self->_spacing != newVal)
		{
			self->_spacing = newVal;
		}
	}

	void TilesControl::Start()
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		self->reset_tiles();
	}
	
	int TilesControl::tiles_count_get() const
	{
		TilesControlImpl* self = Object::cast_to<TilesControlImpl>(_impl);
		return self->_numTiles;
	}

	void TilesControl::tiles_count_set(int newVal)
	{
		CRASH_NOW();
	}
}
