#include "NewGame.hpp"
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include "function.hpp"

using namespace godot;

void NewGame::_bind_methods()
{
	FUNC_("NewGame::_bind_methods");

	// API
	ClassDB::bind_method(D_METHOD("on_browse_pressed"), &NewGame::on_browse_pressed);
	ClassDB::bind_method(D_METHOD("on_cancel_pressed"), &NewGame::on_cancel_pressed);
	ClassDB::bind_method(D_METHOD("on_start_pressed"), &NewGame::on_start_pressed);
	ClassDB::bind_method(D_METHOD("on_tilesImageDialog_fileSelected"), &NewGame::on_tilesImageDialog_fileSelected);
	ClassDB::bind_method(D_METHOD("on_useImage_pressed"), &NewGame::on_useImage_pressed);
	ClassDB::bind_method(D_METHOD("update_image"), &NewGame::update_image);
}

NewGame::NewGame()
	: _changedImagePath(false),
	  _tilesImageDialogUsed(false)
{
	FUNC_("NewGame::NewGame");
	
	_browse = memnew(Button);
	_cancel = memnew(Button);
	_centerContainer = memnew(CenterContainer);
	_hboxContainer = memnew(HBoxContainer);
	_hboxContainer2 = memnew(HBoxContainer);
	_marginContainer = memnew(MarginContainer);
	_start = memnew(Button);
	_tilesImageDialog = memnew(FileDialog);
	_tilesImage = memnew(TextureRect);
	_useImageLabel = memnew(Label);
	_useImage = memnew(CheckButton);
	_vboxContainer = memnew(VBoxContainer);
}

NewGame::~NewGame()
{
	FUNC_("NewGame::~NewGame");
}

void NewGame::_ready()
{
	FUNC_("NewGame::_ready");
		
	_globals = get_node<AppGlobals>("/root/Globals");
	_preferences = get_node<AppPreferences>("/root/Preferences");
	_tree = get_tree();

	ERR_FAIL_COND(_globals == nullptr);
	ERR_FAIL_COND(_preferences == nullptr);
		
	Ref<Theme> theme = _globals->theme_get();

	ERR_FAIL_COND(theme == nullptr);

	// Set our settings
	set_anchors_preset(Control::LayoutPreset::PRESET_FULL_RECT);
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
	_browse->set_text_alignment(HorizontalAlignment::HORIZONTAL_ALIGNMENT_CENTER);
	_vboxContainer->add_child(_browse);
		
	// Add margin around command buttons
	_vboxContainer->add_child(_marginContainer);

	// Add h box container
	_hboxContainer2->set_alignment(BoxContainer::AlignmentMode::ALIGNMENT_CENTER);
	_marginContainer->add_child(_hboxContainer2);
		
	// Add cancel button
	_cancel->set_text("Cancel");
	_hboxContainer2->add_child(_cancel);

	// Add start button
	_start->set_text("Start");
	_hboxContainer2->add_child(_start);
		
	// Add tile image dialog
	_tilesImageDialog->set_process_mode(Node::ProcessMode::PROCESS_MODE_PAUSABLE);
	_tilesImageDialog->set_min_size(Vector2(480, 320));
	_tilesImageDialog->set_exclusive(true);
	_tilesImageDialog->set_title("Open Image");
	_tilesImageDialog->set_mode_overrides_title(false);
	_tilesImageDialog->set_file_mode(FileDialog::FileMode::FILE_MODE_OPEN_FILE);
	_tilesImageDialog->set_access(FileDialog::Access::ACCESS_FILESYSTEM);
	PackedStringArray filters;
	filters.append("*.bmp,*.jpg,*.jpeg,*.png;Images");
	filters.append("*.bmp;Bitmap Images");
	filters.append("*.jpg;*.jpeg;JPEG Images");
	filters.append("*.png;PNG Images");
	_tilesImageDialog->set_filters(filters);
	add_child(_tilesImageDialog);
		
	// Signals
	_browse->connect("pressed", Callable(this, "on_browse_pressed"));
	_cancel->connect("pressed", Callable(this, "on_cancel_pressed"));
	_start->connect("pressed", Callable(this, "on_start_pressed"));
	_tilesImageDialog->connect("file_selected", Callable(this, "on_tilesImageDialog_fileSelected"));
	_useImage->connect("pressed", Callable(this, "on_useImage_pressed"));

	// Update variables from globals
	const bool useImage = _preferences->use_image_get();
	_useImage->set_pressed(useImage == true);
	_browse->set_disabled(useImage == false);
	if (_globals->tiles_default_image_get())
		_imagePath = _preferences->default_image_get();
	else
		_imagePath = _preferences->last_image_get();
	_image = load_image(_imagePath);

	ERR_FAIL_COND(_image == nullptr);
	
	// Resize image and set as texture
	call_deferred("update_image");
}
	
Ref<Image> NewGame::load_image(const String path)
{
	FUNC_("NewGame::load_image");
		
	UtilityFunctions::print("NewGame::load_image: loading image from \"", path, "\"");
	Ref<Image> i;
	if (path.substr(0, 4) == "res:")
		i = ResourceLoader::get_singleton()->load(path, "Image", ResourceLoader::CacheMode::CACHE_MODE_IGNORE);
	else
	{
		i = Ref(memnew(Image));
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
	_imageTexture = ImageTexture::create_from_image(_image);
	_tilesImage->set_texture(_imageTexture);
}
