#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/center_container.hpp>
#include <godot_cpp/classes/file_dialog.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include "KakelGlobals.hpp"
#include "KakelPreferences.hpp"

namespace godot
{
	class Main : public PanelContainer
	{
		GDCLASS(Main, PanelContainer);

	private:
		CenterContainer* _centerContainer;
		KakelGlobals* _globals;
		Label* _label;
		Button* _load;
		FileDialog* _loadDialog;
		bool _loadDialogUsed;
		Button* _options;
		KakelPreferences* _preferences;
		Button* _quit;
		Button* _start;
		SceneTree* _tree;
		VBoxContainer* _vboxContainer;

	protected:
		static void _bind_methods();

	public:
		Main();
		~Main();

		void _ready() override;
		void _unhandled_input(const Ref<InputEvent> &event) override;
			
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
