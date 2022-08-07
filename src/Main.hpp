#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <Godot.hpp>
#include <Button.hpp>
#include <CenterContainer.hpp>
#include <FileDialog.hpp>
#include <Label.hpp>
#include <PanelContainer.hpp>
#include <SceneTree.hpp>
#include <VBoxContainer.hpp>

namespace godot
{
	class Main : public PanelContainer
	{
		GODOT_CLASS(Main, PanelContainer)

	private:
		CenterContainer* _centerContainer;
		VBoxContainer* _vboxContainer;
		Label* _label;
		Button* _load;
		FileDialog* _loadDialog;
		Button* _prefs;
		Button* _quit;
		Button* _start;
		SceneTree* _tree;

	public:
		static void _register_methods();

		Main();
		~Main();

		void _init();
		void _ready();

		void on_load();
		void on_loadDialog_fileSelected(const String path);
		void on_new();
		void on_prefs();
		void on_quit();
	};
}

#endif	// MAIN_HPP_INCLUDED
