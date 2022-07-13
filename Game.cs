using Godot;
using System;

public class Game : Control
{
	Button _closeHint;
	WindowDialog _hintDialog;
	FileDialog _loadDialog;
	LineEdit _moves;
	FileDialog _saveDialog;
	TextureRect _textureRect;
	TilesControl _tiles;
	SceneTree _tree;
	VBoxContainer _vboxContainer;
	WindowDialog _win;

	private void _on_Abort_pressed()
	{
		_tree.ChangeScene("res://Main.tscn");
	}

	private void _on_CloseHint_pressed()
	{
		HideHint();
	}

	private void _on_HintDialog_item_rect_changed()
	{
		Vector2 s = _hintDialog.RectSize;
		Vector2 cs = _closeHint.RectSize;
		_textureRect.RectSize = new Vector2(s.x - 20, s.y - 50);
		_closeHint.RectPosition = new Vector2((s.x - cs.x) / 2, s.y - cs.y - 10);
	}

	private void _on_HintDialog_modal_closed()
	{
		HideHint();
	}

	private void _on_HintDialog_popup_hide()
	{
		HideHint();
	}

	private void _on_Hint_pressed()
	{
		_tree.Paused = true;
		_vboxContainer.Hide();
		if (_textureRect.Texture == null)
		{
			Image image;
			Globals globals = GetNode<Globals>("/root/Globals");
			if (globals.TilesImage == null)
				image = globals.TilesImageDefault.Duplicate() as Image;
			else
				image = globals.TilesImage.Duplicate() as Image;
			ImageTexture t = new ImageTexture();
			t.CreateFromImage(image);
			_textureRect.Texture = t;
		}
		_hintDialog.PopupCentered();
	}

	private void _on_LoadDialog_file_selected(string path)
	{
		_tiles.Load(path);
		_tree.Paused = false;
	}

	private void _on_LoadDialog_modal_closed()
	{
		_tree.Paused = false;
	}

	private void _on_LoadDialog_popup_hide()
	{
		_tree.Paused = false;
	}

	private void _on_Load_pressed()
	{
		_tree.Paused = true;
		_loadDialog.PopupCentered();
	}

	private void _on_Quit_pressed()
	{
		_tree.Quit();
	}

	private void _on_SaveDialog_file_selected(string path)
	{
		_tiles.Save(path);
		_tree.Paused = false;
	}

	private void _on_SaveDialog_modal_closed()
	{
		_tree.Paused = false;
	}


	private void _on_SaveDialog_popup_hide()
	{
		_tree.Paused = false;
	}

	private void _on_Save_pressed()
	{
		_tree.Paused = true;
		_saveDialog.PopupCentered();
	}

	private void _on_Tiles_item_rect_changed()
	{
		if (_tiles != null)
			_tiles.RecalcTiles();
	}

	private void _on_Tiles_moved(TilesControl.DIRECTION _direction, int count)
	{
		_moves.Text = count.ToString();
	}

	private void _on_Tiles_won()
	{
		_tree.Paused = true;
		_win.PopupCentered();
	}

	protected void HideHint()
	{
		_tree.Paused = false;
		_vboxContainer.Show();
		_hintDialog.Hide();
	}

	public override void _Ready()
	{
		_closeHint = GetNode<Button>("HintDialog/CloseHint");
		_hintDialog = GetNode<WindowDialog>("HintDialog");
		_loadDialog = GetNode<FileDialog>("LoadDialog");
		_moves = GetNode<LineEdit>("PanelContainer/VBoxContainer/HFlowContainer/Moves");
		_saveDialog = GetNode<FileDialog>("SaveDialog");
		_textureRect = GetNode<TextureRect>("HintDialog/TextureRect");
		_tiles = GetNode<TilesControl>("PanelContainer/VBoxContainer/Tiles");
		_tree = GetTree();
		_vboxContainer = GetNode<VBoxContainer>("PanelContainer/VBoxContainer");
		_win = GetNode<WindowDialog>("WinnerDialog");
	}
}
