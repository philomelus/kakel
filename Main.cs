#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146, IDE0052

using Godot;
using System;

public partial class Main : Control
{
	private Globals _globals;
	private FileDialog _loadDialog;
	private SceneTree _tree;

	public override void _Ready()
	{
		_globals = GetNode<Globals>("/root/Globals");
		_loadDialog = GetNode<FileDialog>("LoadDialog");
		_tree = GetTree();

		// Initialize global default font if not overridden
		if (_globals.TilesNumberFont == null)
			_globals.TilesNumberFont = GetThemeDefaultFont();
	}

	public override void _UnhandledInput(InputEvent ev)
	{
		if (ev.IsActionPressed("quit"))
		{
			AcceptEvent();
			Quit();
		}
		else if (ev.IsActionPressed("new"))
		{
			AcceptEvent();
			NewGame();
		}
	}

	private void NewGame()
	{
		_tree.ChangeScene("res://NewGame.tscn");
	}

	private void OnLoadDialogFileSelected(string path)
	{
		_tree.Paused = false;
		_globals.TilesLoading = true;
		_globals.TilesLoadPath = path;
		_tree.ChangeScene("res://Game.tscn");
	}

	private void OnLoadPressed()
	{
		_tree.Paused = true;
		_loadDialog.PopupCentered();
	}

	private void OnNewPressed()
	{
		_tree.ChangeScene("res://NewGame.tscn");
	}

	private void OnQuitPressed()
	{
		Quit();
	}

	private void Quit()
	{
		_tree.Quit();
	}
}
