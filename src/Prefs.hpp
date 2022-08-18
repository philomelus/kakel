#ifndef PREFS_HPP_INCLUDED
#define PREFS_HPP_INCLUDED

#include <godot_cpp/classes/check_button.hpp>
#include <godot_cpp/classes/color_picker_button.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/spin_box.hpp>
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
		CheckButton* _autoSave;
		Button *_cancel;
		SpinBox* _columns;
		CheckButton* _hiliteBlank;
		ColorPickerButton* _hiliteBlankColor;
		CheckButton* _keepAspect;
		ColorPickerButton* _numbersColor;
		CheckButton* _numbersVisible;
		ColorPickerButton* _outlinesColor;
		CheckButton* _outlinesVisible;
		SpinBox* _rows;
		Button* _save;
		SceneTree* _tree;

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
