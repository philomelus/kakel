#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <Godot.hpp>
#include <Button.hpp>
#include <CenterContainer.hpp>
#include <FileDialog.hpp>
#include <InputEvent.hpp>
#include <Label.hpp>
#include <PanelContainer.hpp>
#include <SceneTree.hpp>
#include <VBoxContainer.hpp>
#include "AppGlobals.hpp"
#include "AppPreferences.hpp"

namespace godot
{
	class Main : public PanelContainer
	{
		GODOT_CLASS(Main, PanelContainer)

	private:
		// Common
		AppGlobals* _globals;
		AppPreferences* _preferences;

		// UI
		CenterContainer* _centerContainer;
		Label* _label;
		Button* _load;
		FileDialog* _loadDialog;
		Button* _options;
		Button* _quit;
		Button* _start;
		SceneTree* _tree;
		VBoxContainer* _vboxContainer;

		// Internal
		bool _loadDialogUsed;
   
	public:
		static void _register_methods();

		Main();
		~Main();

		void _init();
		void _ready();
		void _unhandled_input(Ref<InputEvent> ev);
		
	protected:
		void check_auto_start();
		void load();
		void load_game(const String path);
		void new_game();
		void on_load_pressed();
		void on_loadDialog_fileSelected(const String path);
		void on_new_pressed();
		void on_options_pressed();
		void on_quit_pressed();
		void options();
		void quit();
	};
}

#endif	// MAIN_HPP_INCLUDED
