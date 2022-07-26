#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146, IDE0044, RCS1169, IDE0052

using Godot;
using System;

public class NewGame : Control
{
    Button _browse;
    Globals _globals;
    Image _image;
    string _imagePath;
    ImageTexture _imageTexture;
    TextureRect _tilesImage;
    FileDialog _tilesImageDialog;
    bool _tilesImageDialogUsed;
    SceneTree _tree;
    CheckButton _useImage;

    bool _changedImagePath;

    public override void _Ready()
    {
        _browse = GetNode<Button>("PanelContainer/CenterContainer/VBoxContainer/Browse");
        _changedImagePath = false;
        _globals = GetNode<Globals>("/root/Globals");
        _tilesImage = GetNode<TextureRect>("PanelContainer/CenterContainer/VBoxContainer/TilesImage");
        _tilesImageDialog = GetNode<FileDialog>("TileImageDialog");
        _tilesImageDialogUsed = false;
        _tree = GetTree();
        _useImage = GetNode<CheckButton>("PanelContainer/CenterContainer/VBoxContainer/HBoxContainer/UseImage");

        // Update variables from globals
        _useImage.Pressed = _globals.TilesUseImage;
        _browse.Disabled = !_globals.TilesUseImage;
        if (_globals.TilesDefaultImage)
            _imagePath = _globals.Preferences.DefaultImage;
        else
            _imagePath = _globals.Preferences.LastImage;
        _image = LoadImage(_imagePath);

		// Resize image and set as texture
        CallDeferred("UpdateImage");
    }

    private Image LoadImage(string path)
    {
        if (path.Substr(0, 4) == "res:")
        {
            return ResourceLoader.Load<Image>(path, "Image", true);
        }
        else
        {
            Image i = new Image();
            i.Load(path);
            return i;
        }
    }

    void OnBrowsePressed()
    {
        if (_tilesImageDialogUsed)
        {
		    _tilesImageDialog.Popup_();
        }
        else
        {
            _tilesImageDialogUsed = true;
		    _tilesImageDialog.PopupCentered();
        }
    }

    void OnCancelPressed()
    {
		_tree.ChangeScene("res://Main.tscn");
    }

    void OnStartPressed()
    {
		if (_useImage.Pressed)
		{
            _globals.TilesUseImage = true;
            if (_changedImagePath)
            {
                _globals.TilesDefaultImage = false;
                _globals.Preferences.LastImage = _imagePath;
                _globals.Preferences.Save(Preferences.P_PREFS);
            }
		}
        else
        {
            _globals.TilesUseImage = false;
        }
		_tree.ChangeScene("res://Game.tscn");
    }

    void OnTileImageDialogFileSelected(string path)
    {
        _imagePath = path;
        _image = LoadImage(_imagePath);
        _changedImagePath = true;
        UpdateImage();
    }

    void OnUseImagePressed()
    {
        _browse.Disabled = !_useImage.Pressed;
    }

    private void UpdateImage()
    {
        _image.Resize((int) _tilesImage.RectSize.x, (int) _tilesImage.RectSize.y);
        _imageTexture = new ImageTexture();
        _imageTexture.CreateFromImage(_image);
        _tilesImage.Texture = _imageTexture;
    }
}
