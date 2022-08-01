#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146, IDE0044, RCS1169, IDE0052

using Godot;
using System;

public partial class NewGame : Control
{
    Button _browse;
    Globals _globals;
    Image _image;
    string _imagePath;
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
        _useImage.ButtonPressed = _globals.TilesUseImage;
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
            return ResourceLoader.Load<Image>(path, "Image", ResourceLoader.CacheMode.Ignore);
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
		    _tilesImageDialog.Popup();
        }
        else
        {
            _tilesImageDialogUsed = true;
            if (_globals.Preferences.LastImage != null)
                _tilesImageDialog.CurrentPath = _globals.Preferences.LastImage;
		    _tilesImageDialog.PopupCentered();
        }
    }

    void OnCancelPressed()
    {
		_tree.ChangeScene("res://Main.tscn");
    }

    void OnStartPressed()
    {
		if (_useImage.ButtonPressed)
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
        _browse.Disabled = !_useImage.ButtonPressed;
    }

    private void UpdateImage()
    {
        _image.Resize((int) _tilesImage.Size.x, (int) _tilesImage.Size.y);
        _tilesImage.Texture = ImageTexture.CreateFromImage(_image);
    }
}
