#include "NewGame.hpp"
#include <GlobalConstants.hpp>
#include <ResourceLoader.hpp>
#include "function.hpp"

using namespace godot;

namespace godot
{
	void NewGame::_register_methods()
	{
		FUNC_("NewGame::_register_methods");

		// API
		register_method("_ready", &NewGame::_ready);
		register_method("on_browse_pressed", &NewGame::on_browse_pressed);
		register_method("on_cancel_pressed", &NewGame::on_cancel_pressed);
		register_method("on_start_pressed", &NewGame::on_start_pressed);
		register_method("on_tilesImageDialog_fileSelected", &NewGame::on_tilesImageDialog_fileSelected);
		register_method("on_useImage_pressed", &NewGame::on_useImage_pressed);
		register_method("update_image", &NewGame::update_image);
	}

	NewGame::NewGame()
		: _changedImagePath(false),
		  _tilesImageDialogUsed(false)
	{
		FUNC_("NewGame::NewGame");
	}

	NewGame::~NewGame()
	{
		FUNC_("NewGame::~NewGame");
	}

	void NewGame::_init()
	{
		FUNC_("NewGame::_init");

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
		FUNC_("NewGame::_ready");
		
		_globals = get_node<Globals>("/root/Globals");
		_preferences = get_node<Preferences>("/root/Preferences");
		_tree = get_tree();

		ERR_FAIL_COND(_globals == nullptr);
		ERR_FAIL_COND(_preferences == nullptr);
		
		Ref<Theme> theme = _globals->theme_get();

		ERR_FAIL_COND(theme == nullptr);

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
		_centerContainer->set_theme(theme);
		_centerContainer->add_child(_vboxContainer);

		// Add h box container
		_vboxContainer->add_child(_hboxContainer);

		// Add use image label
		_useImageLabel->set_text("Use Image");
		_hboxContainer->add_child(_useImageLabel);

		// Add use image
		_hboxContainer->add_child(_useImage);

		// Add tiles image
		_tilesImage->set_h_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		_tilesImage->set_v_size_flags(Control::SizeFlags::SIZE_EXPAND_FILL);
		_tilesImage->set_custom_minimum_size(Vector2(200, 200));
		_tilesImage->set_stretch_mode(TextureRect::StretchMode::STRETCH_KEEP_ASPECT_CENTERED);
		_vboxContainer->add_child(_tilesImage);

		// Add browse button
		_browse->set_text("Browse");
		_browse->set_v_size_flags(0);
		_browse->set_h_size_flags(Control::SizeFlags::SIZE_SHRINK_CENTER);
		_browse->set_text_align(Button::TextAlign::ALIGN_CENTER);
		_vboxContainer->add_child(_browse);
		
		// Add margin around command buttons
		_vboxContainer->add_child(_marginContainer);

		// Add h box container
		_hboxContainer2->set_alignment(BoxContainer::AlignMode::ALIGN_CENTER);
		_marginContainer->add_child(_hboxContainer2);
		
		// Add cancel button
		_cancel->set_text("Cancel");
		_hboxContainer2->add_child(_cancel);

		// Add start button
		_start->set_text("Start");
		_hboxContainer2->add_child(_start);
		
		// Add tile image dialog
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
		add_child(_tilesImageDialog);
		
		// Signals
		_browse->connect("pressed", this, "on_browse_pressed");
		_cancel->connect("pressed", this, "on_cancel_pressed");
		_start->connect("pressed", this, "on_start_pressed");
		_tilesImageDialog->connect("file_selected", this, "on_tilesImageDialog_fileSelected");
		_useImage->connect("pressed", this, "on_useImage_pressed");

        // Update variables from globals
		const bool useImage = _preferences->use_image_get();
        _useImage->set_pressed(useImage == true);
        _browse->set_disabled(useImage == false);
        if (_globals->tiles_default_image_get())
            _imagePath = _preferences->default_image_get();
        else
            _imagePath = _preferences->last_image_get();
        _image = load_image(_imagePath);

		// Resize image and set as texture
        call_deferred("update_image");
	}
	
	Ref<Image> NewGame::load_image(const String path)
	{
		FUNC_("NewGame::load_image");
		
		Godot::print("NewGame::load_image: loading image from \"{0}\"", path);
		Ref<Image> i;
		if (path.substr(0, 4) == "res:")
			i = Ref(ResourceLoader::get_singleton()->load(path, "Image", true));
		else
		{
			i = Ref(Image::_new());
			i->load(path);
		}
		return i;
	}
	
	void NewGame::on_browse_pressed()
	{
		FUNC_("NewGame::on_browse_pressed");
		
        if (_tilesImageDialogUsed)
        {
		    _tilesImageDialog->popup();
        }
        else
        {
            _tilesImageDialogUsed = true;
			String lastImage = _preferences->last_image_get();
            if (lastImage.length() > 0)
                _tilesImageDialog->set_current_path(lastImage);
		    _tilesImageDialog->popup_centered();
        }
	}
	
	void NewGame::on_cancel_pressed()
	{
		FUNC_("NewGame::on_cancel_pressed");
		
		_tree->change_scene("res://Main.tscn");
	}
	
	void NewGame::on_start_pressed()
	{
		FUNC_("NewGame::on_start_pressed");

		if (_useImage->is_pressed())
		{
            _preferences->use_image_set(true);
            if (_changedImagePath)
            {
                _globals->tiles_default_image_set(false);
                _preferences->last_image_set(_imagePath);
                _preferences->save(_preferences->P_PREFS);
            }
		}
        else
        {
			if (_preferences->use_image_get())
			{
				_preferences->use_image_set(false);
                _preferences->save(_preferences->P_PREFS);
			}
        }
		_tree->change_scene("res://Game.tscn");
	}
	
	void NewGame::on_tilesImageDialog_fileSelected(const String path)
	{
		FUNC_("NewGame::on_tilesImageDialog_fileSelected");
		
        _imagePath = path;
        _image = load_image(_imagePath);
        _changedImagePath = true;
		call_deferred("update_image");
	}
	
	void NewGame::on_useImage_pressed()
	{
		FUNC_("NewGame::on_useImage_pressed");
		
        _browse->set_disabled(!_useImage->is_pressed());
	}
	
	void NewGame::update_image()
	{
		FUNC_("NewGame::update_image");
		
		ERR_FAIL_COND(_image == nullptr);
		
		Vector2 size = _tilesImage->get_size();
		
		ERR_FAIL_COND(size.x <= 0 && size.y <= 0);
		
		_image->resize(size.x, size.y);
		_imageTexture = ImageTexture::_new();
		_imageTexture->create_from_image(_image);
		_tilesImage->set_texture(_imageTexture);
	}
}
