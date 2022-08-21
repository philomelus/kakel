#ifndef NEWGAME_HPP_INCLUDED
#define NEWGAME_HPP_INCLUDED

#include <Godot.hpp>
#include <Button.hpp>
#include <CenterContainer.hpp>
#include <CheckButton.hpp>
#include <FileDialog.hpp>
#include <GridContainer.hpp>
#include <HBoxContainer.hpp>
#include <Image.hpp>
#include <ImageTexture.hpp>
#include <Label.hpp>
#include <MarginContainer.hpp>
#include <PanelContainer.hpp>
#include <SceneTree.hpp>
#include <TextureRect.hpp>
#include <VBoxContainer.hpp>
#include "AppGlobals.hpp"
#include "AppPreferences.hpp"

namespace godot
{
	class NewGame : public PanelContainer
	{
		GODOT_CLASS(NewGame, PanelContainer)

	private:
		// Common
		AppGlobals* _globals;
		AppPreferences* _preferences;
		SceneTree* _tree;

		// UI
		Button* _browse;
		Button* _cancel;
		CenterContainer* _centerContainer;
		GridContainer* _gridContainer;
		HBoxContainer* _hboxContainer;
		ImageTexture* _imageTexture;
		CheckButton* _keepAspect;
		Label* _keepAspectLabel;
		MarginContainer* _marginContainer;
		Button* _start;
		FileDialog* _tilesImageDialog;
		TextureRect* _tilesImage;
		CheckButton* _useImage;
		Label* _useImageLabel;
		VBoxContainer* _vboxContainer;

		// Internal
		bool _changedImagePath;
		Ref<Image> _image;
		String _imagePath;
		bool _tilesImageDialogUsed;
		
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
		void on_keepAspect_pressed();
		void on_start_pressed();
		void on_tilesImageDialog_fileSelected(const String path);
		void on_useImage_pressed();
		void update_image();
	};
}
#endif	// NEWGAME_HPP_INCLUDED
