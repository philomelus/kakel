#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146, IDE0044, RCS1169, IDE0052

using Godot;
using System;

public class NewGame : Control
{
    Button _browse;
    SpinBox _columns;
    CheckButton _defaultImage;
    Globals _globals;
    Image _image;
    string _imagePath;
    ImageTexture _imageTexture;
    ColorPickerButton _numberColor;
    ColorPickerButton _outlineColor;
    SpinBox _rows;
    CheckButton _showNumbers;
    TextureRect _tilesImage;
    FileDialog _tilesImageDialog;
    bool _tilesImageDialogUsed;
    SceneTree _tree;
    CheckButton _useImage;

    public override void _Ready()
    {
        _browse = GetNode<Button>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Right/Browse");
        _columns = GetNode<SpinBox>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Left/Columns");
        _defaultImage = GetNode<CheckButton>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Right/DefaultImage");
        _globals = GetNode<Globals>("/root/Globals");
        _numberColor = GetNode<ColorPickerButton>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Left/NumberColor");
        _outlineColor = GetNode<ColorPickerButton>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Left/OutlineColor");
        _rows = GetNode<SpinBox>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Left/Rows");
        _showNumbers = GetNode<CheckButton>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Left/ShowNumbers");
        _tilesImage = GetNode<TextureRect>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Right/TilesImage");
        _tilesImageDialog = GetNode<FileDialog>("TileImageDialog");
        _tilesImageDialogUsed = false;
        _tree = GetTree();
        _useImage = GetNode<CheckButton>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Right/UseImage");

        // Update variables from globals
        _useImage.Pressed = _globals.TilesUseImage;
        if (_globals.TilesDefaultImage)
        {
            _imagePath = _globals.TilesImageDefault;
			_imageTexture = null;
			_defaultImage.Pressed = true;
			_browse.Disabled = true;
        }
        else
        {
			_imagePath = _globals.TilesImagePath;
			_defaultImage.Pressed = false;
			_browse.Disabled = false;
        }
        _image = LoadImage(_imagePath);
        _showNumbers.Pressed = _globals.TilesShowNumbers;
		_columns.Value = _globals.TilesColumns;
		_rows.Value = _globals.TilesRows;
        _numberColor.Color = _globals.TilesNumberColor;
        _outlineColor.Color = _globals.TilesOutlineColor;

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

    void OnDefaultImagePressed()
    {
		_browse.Disabled = _defaultImage.Pressed;
        if (_defaultImage.Pressed)
        {
            _imagePath = _globals.TilesImageDefault;
            _image = LoadImage(_imagePath);
            UpdateImage();
        }
    }

    void OnStartPressed()
    {
        _globals.TilesColumns = (int) _columns.Value;
        _globals.TilesRows = (int) _rows.Value;
		if (_useImage.Pressed)
		{
            _globals.TilesUseImage = true;
			if (_defaultImage.Pressed)
            {
                _globals.TilesDefaultImage = true;
				_globals.TilesImagePath = _globals.TilesImageDefault;
            }
			else
            {
                _globals.TilesDefaultImage = false;
				_globals.TilesImagePath = _imagePath;
            }
		}
        else
        {
            _globals.TilesUseImage = false;
            _globals.TilesDefaultImage = true;
            _globals.TilesImagePath = _globals.TilesImageDefault;
        }
        _globals.TilesNumberColor = _numberColor.Color;
        _globals.TilesOutlineColor = _outlineColor.Color;
        _globals.TilesShowNumbers = _showNumbers.Pressed;
		_tree.ChangeScene("res://Game.tscn");
    }

    void OnTileImageDialogFileSelected(string path)
    {
        _imagePath = path;
        _image = LoadImage(_imagePath);
        UpdateImage();
    }

    void OnUseImagePressed()
    {
		_defaultImage.Disabled = !_useImage.Pressed;
        if (!_useImage.Pressed)
            _browse.Disabled = true;
        else
		    _browse.Disabled = _defaultImage.Pressed;
    }

    private void UpdateImage()
    {
        _image.Resize((int) _tilesImage.RectSize.x, (int) _tilesImage.RectSize.y);
        _imageTexture = new ImageTexture();
        _imageTexture.CreateFromImage(_image);
        _tilesImage.Texture = _imageTexture;
    }
}
