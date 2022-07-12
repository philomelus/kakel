using Godot;
using System;

public class NewGame : Control
{
	Button _browse;
	SpinBox _columns;
	Button _default;
	FileDialog _openTileImage;
	SpinBox _rows;
	TextureRect _tiles;
	CheckButton _useImage;

	string imagePath = "";
	Image image = null;
	ImageTexture imageTexture = null;

	private void _on_Browse_pressed()
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

	private void _on_Cancel_pressed()
	{
		GetTree().ChangeScene("res://Main.tscn");
	}

	private void _on_Default_pressed()
	{
		_browse.Disabled = _default.Pressed;
	}

	private void _on_OpenTileImage_file_selected(string path)
	{
		imagePath = path;
		image = new Image();
		image.Load(path);
		image.Resize((int) _tiles.RectSize.x, (int) _tiles.RectSize.y);
		imageTexture = new ImageTexture();
		imageTexture.CreateFromImage(image, 0);
		_tiles.Texture = imageTexture;
	}

	private void _on_Start_pressed()
	{
		Globals globals = GetNode<Globals>("/root/Globals");
		globals.TilesSize = new Vector2((float) _columns.Value, (float) _rows.Value);
		globals.TilesUseImage = _useImage.Pressed;
		if (_useImage.Pressed)
		{
			if (_default.Pressed)
			{
				globals.TilesImage = null;
				globals.TilesImagePath = "";
			}
			else
			{
				globals.TilesImagePath = imagePath;
				globals.TilesImage = new Image();
				globals.TilesImage.Load(globals.TilesImagePath);
			}
		}
		GetTree().ChangeScene("res://Game.tscn");
	}

	private void _on_UseImage_pressed()
	{
		_default.Disabled = !_useImage.Pressed;
		_browse.Disabled = !_useImage.Pressed;
	}

	public override void _Ready()
	{
		base._Ready();

		_browse = GetNode<Button>("PanelContainer/CenterBox/VerticalBox/GridContainer/Browse");
		_columns = GetNode<SpinBox>("PanelContainer/CenterBox/VerticalBox/GridContainer/Columns");
		_default = GetNode<Button>("PanelContainer/CenterBox/VerticalBox/GridContainer/Default");
		_openTileImage = GetNode<FileDialog>("OpenTileImage");
		_rows = GetNode<SpinBox>("PanelContainer/CenterBox/VerticalBox/GridContainer/Rows");
		_tiles = GetNode<TextureRect>("PanelContainer/CenterBox/VerticalBox/GridContainer/Tiles");
		_useImage = GetNode<CheckButton>("PanelContainer/CenterBox/VerticalBox/GridContainer/UseImage");

		// Set up image usage controls
		Globals globals = GetNode<Globals>("/root/Globals");
		if (globals.TilesUseImage)
			_useImage.Pressed = true;
		else
		{
			_useImage.Pressed = false;
			_default.Disabled = true;
			_browse.Disabled = true;
		}

		// Set up image
		if (globals.TilesImagePath == "")
		{
			imagePath = "";
			image = globals.TilesImageDefault.Duplicate() as Image;
			imageTexture = null;
			_default.Pressed = true;
			_browse.Disabled = true;
		}
		else
		{
			imagePath = globals.TilesImagePath;
			image = globals.TilesImage.Duplicate() as Image;
			_default.Pressed = false;
			_browse.Disabled = false;
		}

		// Resize image and set as texture
		image.Resize((int) _tiles.RectSize.x, (int) _tiles.RectSize.y);
		imageTexture = new ImageTexture();
		imageTexture.CreateFromImage(image, 0);
		_tiles.Texture = imageTexture;

		// Initialize tile grid size
		_columns.Value = globals.TilesSize.x;
		_rows.Value = globals.TilesSize.y;
	}
}
