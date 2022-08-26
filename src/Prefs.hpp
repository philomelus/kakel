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
#include "AppGlobals.hpp"
#include "AppPreferences.hpp"

namespace godot
{
	class Prefs : public godot::PanelContainer
	{
		GDCLASS(Prefs, PanelContainer);

	private:
		// Common
		AppPreferences* _preferences;
		AppGlobals* _globals;

		// UI
		CheckButton* _autoLoad;
		Label* _autoLoadLabel;
		CheckButton* _autoSave;
		Label* _autoSaveLabel;
		Button *_cancel;
		CenterContainer* _centerContainer;
		SpinBox* _columns;
		Label* _columnsLabel;
		GridContainer* _gridContainer;
		HBoxContainer* _hboxContainer;
		CheckButton* _hiliteBlank;
		ColorPickerButton* _hiliteBlankColor;
		Label* _hiliteBlankColorLabel;
		Label* _hiliteBlankLabel;
		CheckButton* _keepAspect;
		Label* _keepAspectLabel;
		MarginContainer* _marginContainer;
		ColorPickerButton* _numbersColor;
		Label* _numbersColorLabel;
		CheckButton* _numbersVisible;
		Label* _numbersVisibleLabel;
		ColorPickerButton* _outlinesColor;
		Label* _outlinesColorLabel;
		CheckButton* _outlinesVisible;
		Label* _outlinesVisibleLabel;
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

		void _ready() override;
		
	protected:
		void on_cancel_pressed();
		void on_save_pressed();
		void quit();
	};
}
#endif	// PREFS_HPP_INCLUDED
