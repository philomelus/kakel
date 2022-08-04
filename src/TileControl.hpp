#ifndef TILE_CONTROL_HPP
#define TILE_CONTROL_HPP

#include <Godot.hpp>
#include <Color.hpp>
#include <Control.hpp>
#include <Font.hpp>
#include <InputEvent.hpp>
#include <InputEventKey.hpp>
#include <Rect2.hpp>
#include <String.hpp>
#include <Vector2.hpp>

namespace kakel
{
	class TileControl : public godot::Control
	{
		GODOT_CLASS(TileControl, godot::Control)

	public:	
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
		godot::String image_path_get() const;
		void image_path_set(godot::Ref<godot::String> const newVal);

		// When true, user movements are allowed.
		bool movable_get() const;
		void movable_set(bool newVal);

		// Color to use for drawing tile numbers.
		godot::Color numbers_color_get() const;
		void numbers_color_set(godot::Color newVal);

		// Font to use for drawing tile numbers.
		godot::Ref<godot::Font> numbers_font_get() const;
		void numbers_font_set(godot::Ref<godot::Font> newVal);

		// When true, show tile number in upper left.
		bool numbers_visible_get() const;
		void numbers_visible_set(bool newVal);

		// Color to use for drawing tile outlines.
		godot::Color outlines_color_get() const;
		void outlines_color_set(godot::Color newVal);

		// When true, outline each tile.
		bool outlines_visible_get() const;
		void outlines_visible_set(bool newVal);

		// Number of rows in tiles.
		int rows_get() const;
		void rows_set(int newVal);

		// Distance between tiles in pixels.
		godot::Ref<godot::Vector2> spacing_get() const;
		void spacing_set(godot::Ref<godot::Vector2> newVal);

		// Total number of tiles in tiles.
		int tiles_count_get() const;

	public:
		TileControl();
		~TileControl();

		void _draw();
		void _init();
		void _input(const godot::Ref<godot::InputEvent> ev);
		void _physics_process(const float delta);
		void _process(const float delta);
		void _ready();
		void _unhandled_input(const godot::Ref<godot::InputEvent> ev);
		void _unhandled_key_input(const godot::Ref<godot::InputEventKey> ev);
		
		static void _register_methods();

		// Load a game from file.
		void LoadGame(godot::Ref<godot::String> path);

		// Recalculate the tiles.  Should be called after containing window is
		// resized, to adapt the tile image to new size.
		void RecalcTiles();

		// Save current game to file.
		void SaveGame(godot::Ref<godot::String> path);

	private:
		TileControl* _impl;
		TileControl(const TileControl&);
		TileControl& operator=(const TileControl&);
	};
}
#endif
