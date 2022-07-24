#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146, IDE0052

using Godot;
using System;

public partial class NewGame : Control
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
    Window _tilesImageDialog;
    bool _tilesImageDialogUsed;
    SceneTree _tree;
    CheckButton _useImage;

    public override void _Ready()
    {
        _browse = GetNode<Button>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/VBoxContainer2/GridContainer2/Browse");
        _columns = GetNode<SpinBox>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/VBoxContainer/GridContainer/Columns");
        _defaultImage = GetNode<CheckButton>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/VBoxContainer2/GridContainer2/DefaultImage");
        _globals = GetNode<Globals>("/root/Globals");
        _numberColor = GetNode<ColorPickerButton>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/VBoxContainer/GridContainer/NumberColor");
        _outlineColor = GetNode<ColorPickerButton>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/VBoxContainer/GridContainer/OutlineColor");
        _rows = GetNode<SpinBox>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/VBoxContainer/GridContainer/Rows");
        _showNumbers = GetNode<CheckButton>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/VBoxContainer/GridContainer/ShowNumbers");
        _tilesImage = GetNode<TextureRect>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/VBoxContainer2/GridContainer2/TilesImage");
        _tilesImageDialog = GetNode<Window>("TileImageDialog");
        _tilesImageDialogUsed = false;
        _tree = GetTree();
        _useImage = GetNode<CheckButton>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/VBoxContainer2/GridContainer2/UseImage");

        // Update variables from globals
        _useImage.ButtonPressed = _globals.TilesUseImage;
        if (_globals.TilesDefaultImage)
        {
            _imagePath = _globals.TilesImageDefault;
			_imageTexture = null;
			_defaultImage.ButtonPressed = true;
			_browse.Disabled = true;
        }
        else
        {
			_imagePath = _globals.TilesImagePath;
			_defaultImage.ButtonPressed = false;
			_browse.Disabled = false;
        }
        if (_imagePath.Substr(0, 4) == "res:")
        {
            _image = GD.Load<Image>(_imagePath);
        }
        else
        {
            _image = new();
            _image.Load(_imagePath);
        }
        _showNumbers.ButtonPressed = _globals.TilesShowNumbers;
		_columns.Value = _globals.TilesColumns;
		_rows.Value = _globals.TilesRows;
        _numberColor.Color = _globals.TilesNumberColor;
        _outlineColor.Color = _globals.TilesOutlineColor;

		// Resize image and set as texture
        UpdateImage();
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
		    _tilesImageDialog.PopupCentered();
        }
    }

    void OnCancelPressed()
    {
		_tree.ChangeScene("res://Main.tscn");
    }

    void OnDefaultImagePressed()
    {
		_browse.Disabled = _defaultImage.ButtonPressed;
        if (_defaultImage.ButtonPressed)
        {
            _imagePath = _globals.TilesImageDefault;
            if (_imagePath.Substr(0, 4) == "res:")
            {
                _image = GD.Load<Image>(_imagePath);
            }
            else
            {
			    _image = new();
                _image.Load(_imagePath);
            }
            UpdateImage();
        }
    }

    void OnStartPressed()
    {
        _globals.TilesColumns = (int) _columns.Value;
        _globals.TilesRows = (int) _rows.Value;
		if (_useImage.ButtonPressed)
		{
            _globals.TilesUseImage = true;
			if (_defaultImage.ButtonPressed)
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
        _globals.TilesShowNumbers = _showNumbers.ButtonPressed;
		_tree.ChangeScene("res://Game.tscn");
    }

    void OnTileImageDialogFileSelected(string path)
    {
        _imagePath = path;
        if (_imagePath.Substr(0, 4) == "res:")
        {
            _image = GD.Load<Image>(_imagePath);
        }
        else
        {
		    _image = new Image();
		    _image.Load(path);
        }
        UpdateImage();
    }

    void OnUseImagePressed()
    {
		_defaultImage.Disabled = !_useImage.ButtonPressed;
		_browse.Disabled = !_useImage.ButtonPressed;
    }

    void UpdateImage()
    {
        _image.Resize((int) _tilesImage.Size.x, (int) _tilesImage.Size.y);
        _imageTexture = ImageTexture.CreateFromImage(_image);
        _tilesImage.Texture = _imageTexture;
    }
}
