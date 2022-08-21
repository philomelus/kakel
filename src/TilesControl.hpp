#ifndef TILE_CONTROL_HPP
#define TILE_CONTROL_HPP

#include <Godot.hpp>
#include <Color.hpp>
#include <Control.hpp>
#include <Font.hpp>
#include <ImageTexture.hpp>
#include <InputEvent.hpp>
#include <InputEventKey.hpp>
#include <Mutex.hpp>
#include <Rect2.hpp>
#include <String.hpp>
#include <Vector2.hpp>

namespace godot
{
	class TilesControl : public Control
	{
		GODOT_CLASS(TilesControl, Control)

	public:	
		static const int FILE_VERSION;

		// Color used for unused area of tiles when keeping aspect ratio
		Color background_color_get() const;
		void background_color_set(const Color newVal);
		
		// When true, a tile can be moved down.
		bool can_move_down_get() const;

		// When true, a tile can be moved left.
		bool can_move_left_get() const;

		// When true, a tile can be moved right.
		bool can_move_right_get() const;

		// When true, a tile can be moved up.
		bool can_move_up_get() const;

		// Number of columns in tiles.
		int columns_get() const;
		void columns_set(const int newVal);

		// When true, the blank tiles is hilited
		bool hilite_blank_get() const;
		void hilite_blank_set(const bool newVal);

		// Color used to hilite the blank tile
		Color hilite_blank_color_get() const;
		void hilite_blank_color_set(const Color newVal);

		// Path to image for tiles.
		String image_path_get() const;
		void image_path_set(const String newVal);

		// When true, image will maintain its aspect ratio
		bool keep_aspect_get() const;
		void keep_aspect_set(const bool newVal);
		
		// When true, user movements are allowed.
		bool movable_get() const;
		void movable_set(const bool newVal);

		// Color to use for drawing tile numbers.
		Color numbers_color_get() const;
		void numbers_color_set(const Color newVal);

		// Font to use for drawing tile numbers.
		Ref<Font> numbers_font_get() const;
		void numbers_font_set(Ref<Font> newVal);

		// When true, show tile number in upper left.
		bool numbers_visible_get() const;
		void numbers_visible_set(const bool newVal);

		// Color to use for drawing tile outlines.
		Color outlines_color_get() const;
		void outlines_color_set(const Color newVal);

		// When true, outline each tile.
		bool outlines_visible_get() const;
		void outlines_visible_set(const bool newVal);

		// Number of rows in tiles.
		int rows_get() const;
		void rows_set(const int newVal);

		// Distance between tiles in pixels.
		Vector2 spacing_get() const;
		void spacing_set(const Vector2 newVal);

		// Total number of tiles in tiles.
		int tiles_count_get() const;

	private:
		Color _backgroundColor;
		bool _canMoveDown;
		bool _canMoveLeft;
		bool _canMoveRight;
		bool _canMoveUp;
		int _columns;
		int _columns0;
		int _empty;
		int _emptyId;
		bool _gameComplete;
		bool _hiliteBlank;
		Color _hiliteBlankColor;
		Ref<Image> _image;
		String _imagePath;
		bool _keepAspect;
		float _lastSignal;
		float _lastWinner;
		int _moveDownIndex;
		int _movedSignal;
		int _moveLeftIndex;
		int _moveRightIndex;
		int _moves;
		bool _movesEnabled;
		int _moveUpIndex;
		int _numTiles;
		int _numTiles0;
		Ref<Font> _numbersFont;
		Color _numbersColor;
		bool _numbersVisible;
		Color _outlinesColor;
		bool _outlinesVisible;
		bool _readyToRun;
		int _rows;
		int _rows0;
		Vector2 _spacing;
		Vector2 _tileSize;		// Pixels of tiles
		int* _tilesOrder;
		bool _tilesReady = false;
		Rect2* _tilesRectScreen;  // Rect2 of tiles in window/display
		Rect2* _tilesRectTexture; // Rect2 of tiles from tiles image
		Ref<ImageTexture> _tilesTexture;
		SceneTree* _tree;

	private:
		void can_move_down_set(bool);
		void can_move_left_set(bool);
		void can_move_right_set(bool);
		void can_move_up_set(bool);
		void tiles_count_set(int);
		
	public:
		TilesControl();
		~TilesControl();

		void _draw();
		void _init();
		void _input(const Ref<InputEvent> ev);
		void _physics_process(const float delta);
		void _ready();
		void _unhandled_input(const Ref<InputEvent> ev);
		
		static void _register_methods();

		// Load a game from file.
		void load_game(const String path);

		// Recalculate the tiles.  Should be called after containing window is
		// resized, to adapt the tile image to new size.
		void recalc_tiles(const bool forced=false);

		// Save current game to file.
		void save_game(const String path);

		// User inter-action isn't allowed before this is called.
		void start();

	protected:
		void calc_movables();
		bool check_complete();
		Ref<Image> load_image(const String path);
		void move_down();
		void move_left();
		void move_right();
		void move_up();
		void reset_tiles();
		
		bool useImage() const { return _imagePath.length() > 0; }
	};
}
#endif
