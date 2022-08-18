#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/file_dialog.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/menu_button.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/window.hpp>
#include "AppGlobals.hpp"
#include "AppPreferences.hpp"
#include "TilesControl.hpp"

namespace godot
{
	class Game : public Control
	{
		GDCLASS(Game, Control);
		
	private:
		// Common
		AppGlobals* _globals;
		AppPreferences* _preferences;
		SceneTree* _tree;

		// UI
		Button* _abort;
		VBoxContainer* _gameBoard;
		Button* _hint;
		Button* _hintClose;
		Window* _hintDialog;
		TextureRect* _hintImage;
		Button* _load;
		FileDialog* _loadDialog;
		LineEdit* _moves;
		MenuButton* _options;
		Button* _quit;
		Button* _save;
		FileDialog* _saveDialog;
		TilesControl* _tiles;
		Button* _winnerClose;
		Window* _winnerDialog;
		Label* _winnerLabel;

		// Internal
		bool _hintDialogUsed;
		bool _loadDialogUsed;
		bool _saveDialogUsed;

	protected:
		static void _bind_methods();

	public:
		Game();
		~Game();

		void _input(const godot::Ref<godot::InputEvent>& ev) override;
		void _ready() override;
		
	protected:
		void abort();
		void hide_hint();
		Ref<Image> load_image(const String path);
		void on_abort_pressed();
		void on_hintDialog_close_pressed();
		void on_hintDialog_closeRequested();
		void on_hintDialog_sizeChanged();
		void on_hint_pressed();
		void on_loadDialog_cancelled();
		void on_loadDialog_fileSelected(const String path);
		void on_load_pressed();
		void on_options_itemSelected(const int index);
		void on_quit_pressed();
		void on_saveDialog_cancelled();
		void on_saveDialog_fileSelected(const String path);
		void on_save_pressed();
		void on_tiles_itemRectChanged();
		void on_tiles_loaded();
		void on_tiles_moved(int count);
		void on_tiles_won();
		void on_winnerDialog_close_pressed();
		void update_hintDialog();
	};
}
#endif	// GAME_HPP_INCLUDED
