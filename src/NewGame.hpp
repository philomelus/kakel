#ifndef NEWGAME_HPP_INCLUDED
#define NEWGAME_HPP_INCLUDED

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/center_container.hpp>
#include <godot_cpp/classes/check_button.hpp>
#include <godot_cpp/classes/file_dialog.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include "Globals.hpp"
#include "Preferences.hpp"

namespace godot
{
	class NewGame : public PanelContainer
	{
		GDCLASS(NewGame, PanelContainer);

	private:
		Button* _browse;
		Button* _cancel;
		CenterContainer* _centerContainer;
		bool _changedImagePath;
		Globals* _globals;
		HBoxContainer* _hboxContainer;
		HBoxContainer* _hboxContainer2;
		Ref<Image> _image;
		String _imagePath;
		ImageTexture* _imageTexture;
		MarginContainer* _marginContainer;
		Preferences* _preferences;
		Button* _start;
		FileDialog* _tilesImageDialog;
		bool _tilesImageDialogUsed;
		TextureRect* _tilesImage;
		SceneTree* _tree;
		CheckButton* _useImage;
		Label* _useImageLabel;
		VBoxContainer* _vboxContainer;

	protected:
		static void _bind_methods();
		
	public:
		NewGame();
		~NewGame();

		void _init();
		void _ready() override;
		
	protected:
		Ref<Image> load_image(const String path);
		void on_browse_pressed();
		void on_cancel_pressed();
		void on_start_pressed();
		void on_tilesImageDialog_fileSelected(const String path);
		void on_useImage_pressed();
		void update_image();
	};
}
#endif	// NEWGAME_HPP_INCLUDED
