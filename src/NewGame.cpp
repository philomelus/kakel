#include "NewGame.hpp"
#include <GlobalConstants.hpp>
#include <ResourceLoader.hpp>

using namespace godot;

namespace godot
{
	void NewGame::_register_methods()
	{
		// API
		register_method("_ready", &NewGame::_ready);
		register_method("load_image", &NewGame::load_image);
		register_method("on_browse_pressed", &NewGame::on_browse_pressed);
		register_method("on_cancel_pressed", &NewGame::on_cancel_pressed);
		register_method("on_start_pressed", &NewGame::on_start_pressed);
		register_method("on_tilesImageDialog_fileSelected", &NewGame::on_tilesImageDialog_fileSelected);
		register_method("on_useImage_pressed", &NewGame::on_useImage_pressed);
	}

	NewGame::NewGame()
		: _changedImagePath(false),
		  _tilesImageDialogUsed(false)
	{
	}

	NewGame::~NewGame()
	{
	}

	void NewGame::_init()
	{
		_browse = Button::_new();
		_cancel = Button::_new();
		_centerContainer = CenterContainer::_new();
		_hboxContainer = HBoxContainer::_new();
		_hboxContainer2 = HBoxContainer::_new();
		_marginContainer = MarginContainer::_new();
		_start = Button::_new();
		_tilesImageDialog = FileDialog::_new();
		_tilesImage = TextureRect::_new();
		_useImageLabel = Label::_new();
		_useImage = CheckButton::_new();
		_vboxContainer = VBoxContainer::_new();
	}

	void NewGame::_ready()
	{
		_globals = get_node<Globals>("/root/Globals");
		_tree = get_tree();

		ERR_FAIL_COND(_globals == nullptr);
		
		Ref<Theme> theme = _globals->theme_get();

		// Set our settings
		set_anchors_preset(Control::LayoutPreset::PRESET_WIDE);
		set_margin(GlobalConstants::MARGIN_LEFT, 0);
		set_margin(GlobalConstants::MARGIN_RIGHT, 0);
		set_margin(GlobalConstants::MARGIN_TOP, 0);
		set_margin(GlobalConstants::MARGIN_BOTTOM, 0);
		set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		set_h_grow_direction(Control::GrowDirection::GROW_DIRECTION_BOTH);
		set_v_grow_direction(Control::GrowDirection::GROW_DIRECTION_BOTH);

		// Add center container
		add_child(_centerContainer);

		// Add v box container
		_centerContainer->add_child(_vboxContainer);
		_centerContainer->set_theme(theme);

		// Add h box container
		_vboxContainer->add_child(_hboxContainer);

		// Add use image label
		_hboxContainer->add_child(_useImageLabel);
		_useImageLabel->set_text("Use Image");

		// Add use image
		_hboxContainer->add_child(_useImage);

		// Add tiles image
		_vboxContainer->add_child(_tilesImage);
		_tilesImage->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		_tilesImage->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		_tilesImage->set_custom_minimum_size(Vector2(200, 200));
		_tilesImage->set_stretch_mode(TextureRect::StretchMode::STRETCH_KEEP_ASPECT_CENTERED);

		// Add browse button
		_vboxContainer->add_child(_browse);
		_browse->set_text("Browse");
		_browse->set_v_size_flags(0);
		_browse->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
		_browse->set_text_align(Button::TextAlign::ALIGN_CENTER);
		
		// Add margin around command buttons
		_vboxContainer->add_child(_marginContainer);

		// Add h box container
		_marginContainer->add_child(_hboxContainer2);
		_hboxContainer2->set_alignment(BoxContainer::AlignMode::ALIGN_CENTER);
		
		// Add cancel button
		_hboxContainer2->add_child(_cancel);
		_cancel->set_text("Cancel");

		// Add start button
		_hboxContainer2->add_child(_start);
		_start->set_text("Start");
		
		// Add tile image dialog
		add_child(_tilesImageDialog);
		_tilesImageDialog->set_pause_mode(Node::PauseMode::PAUSE_MODE_PROCESS);
		_tilesImageDialog->set_custom_minimum_size(Vector2(480, 320));
		_tilesImageDialog->set_exclusive(true);
		_tilesImageDialog->set_title("Open Image");
		_tilesImageDialog->set_resizable(true);
		_tilesImageDialog->set_mode_overrides_title(false);
		_tilesImageDialog->set_mode(FileDialog::Mode::MODE_OPEN_FILE);
		_tilesImageDialog->set_access(FileDialog::Access::ACCESS_FILESYSTEM);
		PoolStringArray filters;
		filters.append("*.bmp,*.jpg,*.jpeg,*.png;Images");
		filters.append("*.bmp;Bitmap Images");
		filters.append("*.jpg;*.jpeg;JPEG Images");
		filters.append("*.png;PNG Images");
		_tilesImageDialog->set_filters(filters);
		
		// Signals
		_browse->connect("pressed", this, "on_browse_pressed");
		_cancel->connect("pressed", this, "on_cancel_pressed");
		_start->connect("pressed", this, "on_save_pressed");
		_tilesImageDialog->connect("file_selected", this, "on_tilesImageDialog_fileSelected");
		_useImage->connect("pressed", this, "on_useImage_pressed");

        // Update variables from globals
		const bool tilesUseImage = _globals->tiles_use_image_get();
        _useImage->set_pressed(tilesUseImage);
        _browse->set_disabled(!tilesUseImage);
        if (_globals->tiles_default_image_get())
            _imagePath = _globals->preferences_get()->default_image_get();
        else
            _imagePath = _globals->preferences_get()->last_image_get();
        _image = load_image(_imagePath);

		// Resize image and set as texture
        call_deferred("update_image");
	}
	
	Ref<Image> NewGame::load_image(const String path)
	{
		if (path.substr(0, 4) == "res:")
			return ResourceLoader::get_singleton()->load(path, "Image", true);
		else
		{
			Ref<Image> i = Image::_new();
			i->load(path);
			return i;
		}
	}
	
	void NewGame::on_browse_pressed()
	{
        if (_tilesImageDialogUsed)
        {
		    _tilesImageDialog->popup();
        }
        else
        {
            _tilesImageDialogUsed = true;
			String lastImage = _globals->preferences_get()->last_image_get();
            if (lastImage.length() > 0)
                _tilesImageDialog->set_current_path(lastImage);
		    _tilesImageDialog->popup_centered();
        }
	}
	
	void NewGame::on_cancel_pressed()
	{
		_tree->change_scene("res://Main.tscn");
	}
	
	void NewGame::on_start_pressed()
	{
		if (_useImage->is_pressed())
		{
            _globals->tiles_use_image_set(true);
            if (_changedImagePath)
            {
                _globals->tiles_default_image_set(false);
                _globals->preferences_get()->last_image_set(_imagePath);
                _globals->preferences_get()->save(_globals->preferences_get()->P_PREFS);
            }
		}
        else
        {
            _globals->tiles_use_image_set(false);
        }
		_tree->change_scene("res://Game.tscn");
	}
	
	void NewGame::on_tilesImageDialog_fileSelected(const String path)
	{
        _imagePath = path;
        _image = load_image(_imagePath);
        _changedImagePath = true;
        update_image();
	}
	
	void NewGame::on_useImage_pressed()
	{
        _browse->set_disabled(!_useImage->is_pressed());
	}
	
	void NewGame::update_image()
	{
		Vector2 size = _tilesImage->get_size();
		_image->resize(size.x, size.y);
		_imageTexture = ImageTexture::_new();
		_imageTexture->create_from_image(_image);
		_tilesImage->set_texture(_imageTexture);
	}
}
