#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146

using Godot;
using System;

public class NewGame : Control
{
	Button _browse;
	SpinBox _columns;
	Button _default;
    Globals _globals;
	FileDialog _openTileImage;
	SpinBox _rows;
	TextureRect _tiles;
    SceneTree _tree;
	CheckButton _useImage;

	string imagePath = "";
	Image image = null;
	ImageTexture imageTexture = null;

    public override void _Ready()
    {
		base._Ready();

		_browse = GetNode<Button>("PanelContainer/CenterBox/VerticalBox/GridContainer/Browse");
		_columns = GetNode<SpinBox>("PanelContainer/CenterBox/VerticalBox/GridContainer/Columns");
		_default = GetNode<Button>("PanelContainer/CenterBox/VerticalBox/GridContainer/Default");
        _globals = GetNode<Globals>("/root/Globals");
		_openTileImage = GetNode<FileDialog>("OpenTileImage");
		_rows = GetNode<SpinBox>("PanelContainer/CenterBox/VerticalBox/GridContainer/Rows");
		_tiles = GetNode<TextureRect>("PanelContainer/CenterBox/VerticalBox/GridContainer/Tiles");
        _tree = GetTree();
		_useImage = GetNode<CheckButton>("PanelContainer/CenterBox/VerticalBox/GridContainer/UseImage");

		// Set up image usage controls
		if (_globals.TilesUseImage)
		{
			_useImage.Pressed = true;
		}
		else
		{
			_useImage.Pressed = false;
			_default.Disabled = true;
			_browse.Disabled = true;
		}

		// Set up image
		if (_globals.TilesImagePath.Length == 0)
		{
			imagePath = "";
			image = _globals.TilesImageDefault.Duplicate() as Image;
			imageTexture = null;
			_default.Pressed = true;
			_browse.Disabled = true;
		}
		else
		{
			imagePath = _globals.TilesImagePath;
			image = _globals.TilesImage.Duplicate() as Image;
			_default.Pressed = false;
			_browse.Disabled = false;
		}

		// Resize image and set as texture
		image.Resize((int) _tiles.RectSize.x, (int) _tiles.RectSize.y);
		imageTexture = new ImageTexture();
		imageTexture.CreateFromImage(image, 0);
		_tiles.Texture = imageTexture;

		// Initialize tile grid size
		_columns.Value = _globals.TilesSize.x;
		_rows.Value = _globals.TilesSize.y;
    }

    private void OnBrowsePressed()
    {
		switch(OS.GetName())
		{
		case "Windows":
			OS.MinWindowSize = new Vector2(480, 320);
			break;
		default:
			OS.MinWindowSize = new Vector2(240, 240);
			break;
		}

		_openTileImage.PopupCentered();
    }

    private void OnCancelPressed()
    {
		_tree.ChangeScene("res://Main.tscn");
    }

    private void OnDefaultPressed()
    {
		_browse.Disabled = _default.Pressed;
    }

    private void OnOpenTileImageFileSelected(string path)
    {
        imagePath = path;
		image = new Image();
		image.Load(path);
		image.Resize((int) _tiles.RectSize.x, (int) _tiles.RectSize.y);
		imageTexture = new ImageTexture();
		imageTexture.CreateFromImage(image, 0);
		_tiles.Texture = imageTexture;
    }

    private void OnStartPressed()
    {
		_globals.TilesSize = new Vector2((float) _columns.Value, (float) _rows.Value);
		_globals.TilesUseImage = _useImage.Pressed;
		if (_useImage.Pressed)
		{
			if (_default.Pressed)
			{
				_globals.TilesImage = null;
				_globals.TilesImagePath = "";
			}
			else
			{
				_globals.TilesImagePath = imagePath;
				_globals.TilesImage = new Image();
				_globals.TilesImage.Load(_globals.TilesImagePath);
			}
		}
		_tree.ChangeScene("res://Game.tscn");
    }

    private void OnUseImagePressed()
    {
		_default.Disabled = !_useImage.Pressed;
		_browse.Disabled = !_useImage.Pressed;
    }
}
