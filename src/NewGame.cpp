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
	ClassDB::bind_method(D_METHOD("on_keepAspect_pressed"), &NewGame::on_keepAspect_pressed);
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
}

NewGame::~NewGame()
{
	FUNC_("NewGame::~NewGame");
}

void NewGame::_ready()
{
	FUNC_("NewGame::_ready");
		
	_globals = get_node<AppGlobals>("/root/Globals");
	ERR_FAIL_COND(_globals == nullptr);

	_preferences = get_node<AppPreferences>("/root/Preferences");
	ERR_FAIL_COND(_preferences == nullptr);

	_browse = get_node<Button>("CenterContainer/VBoxContainer/Browse");
	ERR_FAIL_COND(_browse == nullptr);

	_cancel = get_node<Button>("CenterContainer/VBoxContainer/MarginContainer/HBoxContainer/Cancel");
	ERR_FAIL_COND(_cancel == nullptr);

	_keepAspect = get_node<CheckButton>("CenterContainer/VBoxContainer/GridContainer/KeepAspect");
	ERR_FAIL_COND(_keepAspect == nullptr);

	_start = get_node<Button>("CenterContainer/VBoxContainer/MarginContainer/HBoxContainer/Start");
	ERR_FAIL_COND(_start == nullptr);

	_tilesImage = get_node<TextureRect>("CenterContainer/VBoxContainer/TilesImage");
	ERR_FAIL_COND(_tilesImage == nullptr);
	
	_tilesImageDialog = get_node<FileDialog>("TilesImageDialog");
	ERR_FAIL_COND(_tilesImageDialog == nullptr);

	_useImage = get_node<CheckButton>("CenterContainer/VBoxContainer/GridContainer/UseImage");
	ERR_FAIL_COND(_useImage == nullptr);
	
	_tree = get_tree();
	   
	// Signals
	_browse->connect("pressed", Callable(this, "on_browse_pressed"));
	_cancel->connect("pressed", Callable(this, "on_cancel_pressed"));
	_keepAspect->connect("pressed", Callable(this, "on_keepAspect_pressed"));
	_start->connect("pressed", Callable(this, "on_start_pressed"));
	_tilesImageDialog->connect("file_selected", Callable(this, "on_tilesImageDialog_fileSelected"));
	_useImage->connect("pressed", Callable(this, "on_useImage_pressed"));

	// Update variables from globals
	const bool useImage = _globals->tiles_use_image_get();
	_useImage->set_pressed(useImage == true);
	_keepAspect->set_pressed(_globals->tiles_keep_aspect_get());
	_keepAspect->set_disabled(useImage == false);
	_browse->set_disabled(useImage == false);
	if (_globals->tiles_default_image_get())
	{
		_imagePath = _preferences->default_image_get();
		_globals->tiles_default_image_set(true);
	}
	else
		_imagePath = _preferences->last_image_get();
	_image = load_image(_imagePath);
	ERR_FAIL_COND(_image == nullptr);
	_tilesImage->set_stretch_mode(_globals->tiles_keep_aspect_get()
								  ? TextureRect::StretchMode::STRETCH_KEEP_ASPECT_CENTERED
								  : TextureRect::StretchMode::STRETCH_SCALE);
	
	// Resize image and set as texture
	call_deferred("update_image");
}
	
Ref<Image> NewGame::load_image(const String path)
{
	FUNC_("NewGame::load_image");
		
	FUNCP_("loading image from \"", path, "\"");
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
	
void NewGame::on_keepAspect_pressed()
{
	FUNC_("NewGame::on_keepAspect_pressed");
		
	_tilesImage->set_stretch_mode(_keepAspect->is_pressed() ?
								  TextureRect::StretchMode::STRETCH_KEEP_ASPECT_CENTERED
								  : TextureRect::StretchMode::STRETCH_SCALE);
	update_image();
}
	
void NewGame::on_start_pressed()
{
	FUNC_("NewGame::on_start_pressed");

	if (_useImage->is_pressed())
	{
		_globals->tiles_use_image_set(true);
		if (_changedImagePath)
		{
			_globals->tiles_default_image_set(false);
			_preferences->last_image_set(_imagePath);
			_preferences->save(_preferences->P_PREFS);
		}
	}
	else
		_globals->tiles_use_image_set(false);
	_globals->tiles_keep_aspect_set(_keepAspect->is_pressed());
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

	const bool pressed = _useImage->is_pressed();
	_browse->set_disabled(!pressed);
	_keepAspect->set_disabled(!pressed);
}
	
void NewGame::update_image()
{
	FUNC_("NewGame::update_image");
		
	ERR_FAIL_COND(_image == nullptr);

	Ref<Image> i = _image->duplicate();
	const Vector2 size = _tilesImage->get_size();
	if (_keepAspect->is_pressed())
	{
		const Vector2 imageSize = i->get_size();
		const float h = size.x * (imageSize.y / imageSize.x);
		const float w = size.y * (imageSize.x / imageSize.y);
		if( h <= size.y)
			i->resize(size.x, h);
		else
			i->resize(w, size.y);
	}
	else
		i->resize(size.x, size.y);
	_imageTexture = ImageTexture::create_from_image(i);
	_tilesImage->set_texture(_imageTexture);
}
