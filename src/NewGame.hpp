#ifndef NEWGAME_HPP_INCLUDED
#define NEWGAME_HPP_INCLUDED

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/check_button.hpp>
#include <godot_cpp/classes/file_dialog.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include "AppGlobals.hpp"
#include "AppPreferences.hpp"

namespace godot
{
	class NewGame : public PanelContainer
	{
		GDCLASS(NewGame, PanelContainer);

	private:
		// User interface
		Button* _browse;
		Button* _cancel;
		AppGlobals* _globals;
		Ref<Image> _image;
		Ref<ImageTexture> _imageTexture;
		CheckButton* _keepAspect;
		AppPreferences* _preferences;
		Button* _start;
		FileDialog* _tilesImageDialog;
		TextureRect* _tilesImage;
		SceneTree* _tree;
		CheckButton* _useImage;

		// Non-ui
		bool _changedImagePath;
		String _imagePath;
		bool _tilesImageDialogUsed;
		
	protected:
		static void _bind_methods();
		
	public:
		NewGame();
		~NewGame();

		void _ready() override;
		
	protected:
		Ref<Image> load_image(const String path);
		void on_browse_pressed();
		void on_cancel_pressed();
		void on_keepAspect_pressed();
		void on_start_pressed();
		void on_tilesImageDialog_fileSelected(const String path);
		void on_useImage_pressed();
		void update_image();
	};
}
#endif	// NEWGAME_HPP_INCLUDED
