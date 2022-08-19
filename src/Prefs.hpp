#ifndef PREFS_HPP_INCLUDED
#define PREFS_HPP_INCLUDED

#include <Godot.hpp>
#include <CenterContainer.hpp>
#include <CheckButton.hpp>
#include <ColorPickerButton.hpp>
#include <HBoxContainer.hpp>
#include <GridContainer.hpp>
#include <Label.hpp>
#include <MarginContainer.hpp>
#include <PanelContainer.hpp>
#include <SpinBox.hpp>
#include <VBoxContainer.hpp>
#include "AppGlobals.hpp"
#include "AppPreferences.hpp"

namespace godot
{
	class Prefs : public godot::PanelContainer
	{
		GODOT_CLASS(Prefs, PanelContainer)

	private:
		// Common
		AppGlobals* _globals;
		AppPreferences* _preferences;
		SceneTree* _tree;

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
		VBoxContainer* _vboxContainer;

	public:
		Prefs();
		~Prefs();

		void _init();
		void _ready();
		void _uninit();
		
		static void _register_methods();

	protected:
		void on_cancel_pressed();
		void on_save_pressed();
		void quit();
	};
}
#endif	// PREFS_HPP_INCLUDED
