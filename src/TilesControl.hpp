#ifndef TILE_CONTROL_HPP
#define TILE_CONTROL_HPP

#include <Godot.hpp>
#include <Color.hpp>
#include <Control.hpp>
#include <Font.hpp>
#include <ImageTexture.hpp>
#include <InputEvent.hpp>
#include <InputEventKey.hpp>
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
		void columns_set(int newVal);

		// Path to image for tiles.
		String image_path_get() const;
		void image_path_set(String newVal);

		// When true, user movements are allowed.
		bool movable_get() const;
		void movable_set(bool newVal);

		// Color to use for drawing tile numbers.
		Color numbers_color_get() const;
		void numbers_color_set(Color newVal);

		// Font to use for drawing tile numbers.
		Ref<Font> numbers_font_get() const;
		void numbers_font_set(Ref<Font> newVal);

		// When true, show tile number in upper left.
		bool numbers_visible_get() const;
		void numbers_visible_set(bool newVal);

		// Color to use for drawing tile outlines.
		Color outlines_color_get() const;
		void outlines_color_set(Color newVal);

		// When true, outline each tile.
		bool outlines_visible_get() const;
		void outlines_visible_set(bool newVal);

		// Number of rows in tiles.
		int rows_get() const;
		void rows_set(int newVal);

		// Distance between tiles in pixels.
		Vector2 spacing_get() const;
		void spacing_set(Vector2 newVal);

		// Total number of tiles in tiles.
		int tiles_count_get() const;

	private:
		bool _canMoveDown;
		bool _canMoveLeft;
		bool _canMoveRight;
		bool _canMoveUp;
		int _columns;
		int _columns0;
		int _empty;
		int _emptyId;
		Ref<Image> _image;
		String _imagePath;
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
		void recalc_tiles();

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
