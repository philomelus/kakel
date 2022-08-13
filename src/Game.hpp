#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <Godot.hpp>
#include <Button.hpp>
#include <Control.hpp>
#include <FileDialog.hpp>
#include <GridContainer.hpp>
#include <HFlowContainer.hpp>
#include <Image.hpp>
#include <InputEvent.hpp>
#include <Label.hpp>
#include <LineEdit.hpp>
#include <MarginContainer.hpp>
#include <MenuButton.hpp>
#include <PanelContainer.hpp>
#include <SceneTree.hpp>
#include <TextureRect.hpp>
#include <VBoxContainer.hpp>
#include <VSeparator.hpp>
#include <WindowDialog.hpp>
#include "Globals.hpp"
#include "Preferences.hpp"
#include "TilesControl.hpp"

namespace godot
{
	class Game : public Control
	{
		GODOT_CLASS(Game, Control)
		
	private:
		Button* _abort;
		VBoxContainer* _gameBoard;
		Globals* _globals;
		GridContainer* _gridContainer;
		HFlowContainer* _hflowContainer;
		HFlowContainer* _hflowContainer2;
		Button* _hint;
		Button* _hintClose;
		WindowDialog* _hintDialog;
		bool _hintDialogUsed;
		TextureRect* _hintImage;
		Button* _load;
		FileDialog* _loadDialog;
		bool _loadDialogUsed;
		MarginContainer* _marginContainer;
		LineEdit* _moves;
		Label* _movesLabel;
		MenuButton* _options;
		PanelContainer* _panelContainer;
		Preferences* _preferences;
		Button* _quit;
		Button* _save;
		FileDialog* _saveDialog;
		bool _saveDialogUsed;
		TilesControl* _tiles;
		SceneTree* _tree;
		VSeparator* _vseparator;
		VSeparator* _vseparator2;
		VSeparator* _vseparator3;
		VSeparator* _vseparator4;
		VSeparator* _vseparator5;
		Button* _winnerClose;
		WindowDialog* _winnerDialog;
		Label* _winnerLabel;
		
	public:
		Game();
		~Game();

		void _init();
		void _input(const godot::Ref<godot::InputEvent> ev);
		void _ready();
		
		static void _register_methods();

	protected:
		void abort();
		void hide_hint();
		Ref<Image> load_image(const String path);
		void on_abort_pressed();
		void on_hintDialog_close_pressed();
		void on_hintDialog_popupHide();
		void on_hintDialog_resized();
		void on_hint_pressed();
		void on_loadDialog_fileSelected(const String path);
		void on_loadDialog_popupHide();
		void on_load_pressed();
		void on_options_itemSelected(const int index);
		void on_quit_pressed();
		void on_saveDialog_fileSelected(const String path);
		void on_saveDialog_popupHide();
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
