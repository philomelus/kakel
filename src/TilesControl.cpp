#include "TileControl.hpp"
#include <Image.hpp>
#include <ImageTexture.hpp>
#include <SceneTree.hpp>

using namespace godot;

namespace {
	class TileControlImpl : public kakel::TileControl
	{
		friend kakel::TileControl;
		
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
		godot::Vector2* _spacing;

		// Font for drawing numerical id's of tiles.
		godot::Font* _numbersFont;

		// Outline color for non-image tiles.
		godot::Color _outlinesColor;

		// Color of font for tile numbers.
		godot::Color _numbersColor;

		// Whn true, add numbers to all tiles (incuding images).
		bool _numbersVisible;

		// Whn true, add outline to all tiles (incuding images).
		bool _outlinesVisible;

		// Image used for tiles.
		godot::Image* _image;

		// Path to image file.
		godot::String* _imagePath;

		// `_tilesImage` converted to texture.  Only valid when `_tilesReady` is true.
		godot::ImageTexture* _tilesTexture;

		// Size of each tile in pixels.  Only valid when `_tilesReady` is true.
		Vector2 _tileSize;

		// Data for each tile.  Each tile is a two item Rect array.
		godot::Rect2** _tiles;

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

		SceneTree _tree;

		bool useImage() const { return (_imagePath != nullptr) && (_imagePath->length() > 0); }

	public:
		// For save files, version of file format.
		static const int FILE_VERSION = 1;
		
        // Indexes within `_tiles`[...][].
		static const int IDX_DEST = 0;
		static const int IDX_SRC = 1;
	};
}

// register_property<TileControl, float>("amplitude", &TileControl::amplitude, 10.0);
// register_property<TileControl, float>("speed", &TileControl::set_speed, &TileControl::get_speed, 1.0);
// emit_signal("position_changed", this, new_position);

namespace kakel
{
	void TileControl::_register_methods()
	{
		// API
		register_method("_process", &TileControl::_process);

		// Properties
#if 0
		register_property<TileControl, bool>("can_move_down", nullptr, &TileControl::can_move_down_get(), false);
		register_property<TileControl, bool>("can_move_left", nullptr, &TileControl::can_move_left_get, false);
		register_property<TileControl, bool>("can_move_right", nullptr, &TileControl::can_move_right_get, false);
		register_property<TileControl, bool>("can_move_up", nullptr, &TileControl::can_move_up_get(), false);
		register_property<TileControl, int>("columns", &TileControl::columns_set, &TileControl::columns_get, 4);
		register_property<TileControl, String>("image_path", &TileControl::image_path_set, &TileControl::image_path_get, "");
		register_property<TileControl, bool>("movable", &TileControl::movable_set, &TileControl::movable_get, false);
		register_property<TileControl, Font>("number_font", &TileControl::number_font_set, &TileControl::number_font_get, nullptr);
		register_property<TileControl, Color>("numbers_color", &TileControl::numbers_color_set, &TileControl::numbers_color_get, Color(0.8, 0.8, 0.8, 1));
		register_property<TileControl, bool>("numbers_visible", &TileControl::numbers_visible_set, &TileControl::numbers_visible_get, true);
		register_property<TileControl, Color>("outlines_color", &TileControl::outlines_color_set, &TileControl::outline_color_get, Color(0.5, 0.5, 0.5, 1));
		register_property<TileControl, bool>("outlines_visible", &TileControl::outlines_visible_set, &TileControl::outlines_visible_get, false);
		register_property<TileControl, int>("rows", &TileControl::rows_set, &TileControl::rows_get, 4);
		register_property<TileControl, Vector2>("spacing", &TileControl::spacing_set, &TileControl::spacing_get, Vector2(5, 5));
		register_property<TileControl, int>("tiles_count", &TileControl::tiles_count_set, &TileControl::tiles_count_get, 16);

		// Signals
		register_signal<TileControl>((char*) "moved", "count", GODOT_VARIANT_TYPE_INT);
		register_signal<TileControl>((char*) "won");
#endif
	}

	TileControl::TileControl()
	{
		_impl = reinterpret_cast<TileControlImpl*>(godot::api->godot_alloc(sizeof(TileControlImpl)));
	}

	TileControl::~TileControl()
	{
		godot::api->godot_free(_impl);
		_impl = nullptr;
	}

#if 0
	void TileControl::_init()
	{
		TileControlImpl* self = static_cast<TileControlImpl*>(_impl);
		self->_columns = 4;
		self->_columns0 = 3;
		self->_image = nullptr;
		self->_imagePath = nullptr;
		self->_lastSignal = 0.0;
		self->_lastWinner = 0.0;
		self->_movedSignal = 0;
		self->_moves = 0;
		self->_movesEnabled = false;
		self->_numTiles = 16;
		self->_numTiles0 = 15;
		self->_numbersFont = nullptr;
		self->_numbersColor = Colors.White;
		self->_numbersVisible;
		self->_outlinesColor = Colors.Black;
		self->_outlinesVisible;
		self->_readyToRun = false;
		self->_rows = 4;
		self->_rows0 = 3;
		self->_spacing = nullptr;
		self->_tilesOrder = nullptr;
		self->_tilesReady = false;
		self->_tilesTexture = nullptr;
		self->_tileSize = nullptr;
		self->_tree = godot::api->get_tree();
		
		//some_other_node->connect("the_signal", this, "my_method");
	}

	void TileControl::_process(float delta)
	{
	}

	bool TileControl::can_move_down_get() const
	{
		return _impl->_canMoveDown;
	}
	
	bool TileControl::can_move_left_get() const
	{
		return _impl->_canMoveLeft;
	}
	
	bool TileControl::can_move_right_get() const
	{
		return _impl->_canMoveRight;
	}
	
	bool TileControl::can_move_up_get() const
	{
		return _impl->_canMoveUp;
	}
	
	int TileControl::columns_get() const
	{
		return _impl->_columns;
	}
	
	void TileControl::columns_set(int newVal)
	{
	}
	
	String TileControl::image_path_get() const
	{
		return _impl->_imagePath;
	}
	
	void TileControl::image_path_set(Ref<String> const newVal)
	{
	}
	
	bool TileControl::movable_get() const
	{
		return _impl->_movable;
	}
	
	void TileControl::movable_set(bool newVal)
	{
	}
	
	Font TileControl::number_font_get() const
	{
		return _impl->_numberFont;
	}
	
	void TileControl::number_font_set(Ref<Font> newVal)
	{
	}
	
	Color TileControl::numbers_color_get() const
	{
		return _impl->_numbersColor;
	}
	
	void TileControl::numbers_color_set(Color newVal)
	{
	}
	
	bool TileControl::numbers_visible_get() const
	{
		return _impl->_numbersVisible;
	}
	
	void TileControl::numbers_visible_set(bool newVal)
	{
	}
	
	Color TileControl::outline_color_get() const
	{
		return _impl->_outlinesColor;
	}
	
	void TileControl::outline_color_set(Color newVal)
	{
	}
	
	bool TileControl::outlines_visible_get() const
	{
		return _impl->_outlinesVisible;
	}
	
	void TileControl::outlines_visible_set(bool newVal)
	{
	}
	
	int TileControl::rows_get() const
	{
		return _impl->_rows;
	}
	
	void TileControl::rows_set(int newVal)
	{
	}
	
	Vector2 TileControl::spacing_get() const
	{
		return _impl->_spacing;
	}
	
	void TileControl::spacing_set(Ref<Vector2> newVal)
	{
	}
	
	int TileControl::tiles_count_get() const
	{
		return _impl->_tilesCount;
	}
#endif
}
