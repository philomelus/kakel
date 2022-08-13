#ifndef PREFS_HPP_INCLUDED
#define PREFS_HPP_INCLUDED

#include <godot_cpp/classes/center_container.hpp>
#include <godot_cpp/classes/check_button.hpp>
#include <godot_cpp/classes/color_picker_button.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/grid_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/spin_box.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include "Globals.hpp"
#include "Preferences.hpp"

namespace godot
{
	class Prefs : public godot::PanelContainer
	{
		GDCLASS(Prefs, PanelContainer);

	private:
		CheckButton* _autoLoad;
		Label* _autoLoadLabel;
		CheckButton* _autoSave;
		Label* _autoSaveLabel;
		Button *_cancel;
		CenterContainer* _centerContainer;
		SpinBox* _columns;
		Label* _columnsLabel;
		Globals* _globals;
		GridContainer* _gridContainer;
		HBoxContainer* _hboxContainer;
		MarginContainer* _marginContainer;
		ColorPickerButton* _numbersColor;
		Label* _numbersColorLabel;
		CheckButton* _numbersVisible;
		Label* _numbersVisibleLabel;
		ColorPickerButton* _outlinesColor;
		Label* _outlinesColorLabel;
		CheckButton* _outlinesVisible;
		Label* _outlinesVisibleLabel;
		Preferences* _preferences;
		SpinBox* _rows;
		Label* _rowsLabel;
		Button* _save;
		SceneTree* _tree;
		VBoxContainer* _vboxContainer;

	protected:
		static void _bind_methods();

	public:
		Prefs();
		~Prefs();

		void _init();
		void _ready() override;
		
	protected:
		void on_cancel_pressed();
		void on_save_pressed();
		void quit();
	};
}
#endif	// PREFS_HPP_INCLUDED
