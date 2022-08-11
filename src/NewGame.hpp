#ifndef NEWGAME_HPP_INCLUDED
#define NEWGAME_HPP_INCLUDED

#include <Godot.hpp>
#include <Button.hpp>
#include <CenterContainer.hpp>
#include <CheckButton.hpp>
#include <FileDialog.hpp>
#include <HBoxContainer.hpp>
#include <Image.hpp>
#include <ImageTexture.hpp>
#include <Label.hpp>
#include <MarginContainer.hpp>
#include <PanelContainer.hpp>
#include <SceneTree.hpp>
#include <TextureRect.hpp>
#include <VBoxContainer.hpp>
#include "Globals.hpp"
#include "Preferences.hpp"

namespace godot
{
	class NewGame : public PanelContainer
	{
		GODOT_CLASS(NewGame, PanelContainer)

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
		
	public:
		NewGame();
		~NewGame();

		void _init();
		void _ready();
		
		static void _register_methods();

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
