using Godot;
using System;

public class Main : Control
{
	// Declare member variables here. Examples:
	AcceptDialog _acceptDialog;//:  = get_node("AcceptDialog")
	Control _control;// = get_node(".")
	FileDialog _fileDialog;//  = get_node("FileDialog")
	SceneTree _tree;

	private void _on_AcceptDialog_confirmed()
	{
		_tree.Paused = false;
	}

	private void _on_FileDialog_file_selected(string path)
	{
		Globals globals = GetNode<Globals>("/root/Globals");
		globals.TilesLoading = true;
		globals.TilesLoadPath = path;
		_tree.Paused = false;
		_tree.ChangeScene("res://Game.tscn");
	}

	private void _on_FileDialog_modal_closed()
	{
		_tree.Paused = false;
	}

	private void _on_FileDialog_popup_hide()
	{
		_tree.Paused = false;
	}

	private void _on_Load_pressed()
	{
		_tree.Paused = true;
		_fileDialog.PopupCentered();
	}

	private void _on_New_pressed()
	{
		NewGame();
	}

	private void _on_Quit_pressed()
	{
		_tree.Quit();
	}

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		base._Ready();
		_acceptDialog = GetNode<AcceptDialog>("AcceptDialog");
		_control = GetNode<Control>(".");
		_fileDialog = GetNode<FileDialog>("FileDialog");
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

	public override void _UnhandledInput(InputEvent @evnt)
	{
		base._UnhandledInput(evnt);
		if (evnt.IsActionPressed("quit"))
		{
			AcceptEvent();
			_tree.Quit();
			return;
		}
		if (evnt.IsActionPressed("new"))
		{
			AcceptEvent();
			NewGame();
			return;
		}
	}

	protected void NewGame()
	{
		_tree.ChangeScene("res://NewGame.tscn");
	}
}
