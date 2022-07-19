#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146

using Godot;
using System;

public class Main : Control
{
	AcceptDialog _acceptDialog;
	Control _control;
	FileDialog _fileDialog;
	SceneTree _tree;
	Globals _globals;

	public override void _Ready()
	{
		base._Ready();

		_acceptDialog = GetNode<AcceptDialog>("AcceptDialog");
		_control = GetNode<Control>(".");
		_fileDialog = GetNode<FileDialog>("FileDialog");
		_globals = GetNode<Globals>("/root/Globals");
		_tree = GetTree();

		switch(OS.GetName())
		{
		case "Windows":
			OS.MinWindowSize = new Vector2(640, 480);
			break;
		default:
			OS.MinWindowSize = new Vector2(240, 240);
			break;
		}
	}

	public override void _UnhandledInput(InputEvent @ev)
	{
		base._UnhandledInput(ev);
		if (ev.IsActionPressed("quit"))
		{
			AcceptEvent();
			_tree.Quit();
			return;
		}
		if (ev.IsActionPressed("new"))
		{
			AcceptEvent();
			NewGame();
			return;
		}
	}

	private void NewGame()
	{
		_tree.ChangeScene("res://NewGame.tscn");
	}

	private void OnAcceptDialogConfirmed()
	{
		_tree.Paused = false;
	}

	private void OnFileDialogFileSelected(string path)
	{
		_globals.TilesLoading = true;
		_globals.TilesLoadPath = path;
		_tree.Paused = false;
		_tree.ChangeScene("res://Game.tscn");
	}

	private void OnFileDialogModalClosed()
	{
		_tree.Paused = false;
	}

	private void OnFileDialogPopupHide()
	{
		_tree.Paused = false;
	}

	private void OnLoadPressed()
	{
		_tree.Paused = true;
		_fileDialog.PopupCentered();
	}

	private void OnNewPressed()
	{
		NewGame();
	}

	private void OnQuitPressed()
	{
		_tree.Quit();
	}
}
