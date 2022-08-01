#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146, IDE0044, RCS1169, IDE0052

using Godot;
using System;

public partial class Main : Control
{
	private Globals _globals;
	private FileDialog _loadDialog;
	private bool _loadDialogUsed = false;
	private SceneTree _tree;

	public override void _Ready()
	{
		_globals = GetNode<Globals>("/root/Globals");
		_loadDialog = GetNode<FileDialog>("LoadDialog");
		_tree = GetTree();

		// Load or initialize preferences
		_globals.Preferences.Load(Preferences.P_PREFS);

		// If an image has been loaded before, then default image is gone
		_globals.TilesDefaultImage = string.IsNullOrEmpty(_globals.Preferences.LastImage);

		// Queue call to auto start if desired
		CallDeferred("CheckAutoStart");
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
		else if (ev.IsActionPressed("load"))
		{
			AcceptEvent();
			Load();
		}
		else if (ev.IsActionPressed("prefs"))
		{
			AcceptEvent();
			Prefs();
		}
	}

	private void CheckAutoStart()
	{
		if (_globals.Preferences.AutoLoad)
		{
			Directory d = new Directory();
			if (d.FileExists(_globals.Preferences.AutoPath))
			{
				_globals.AutoStarted = true;
				LoadGame(_globals.Preferences.AutoPath);
			}
		}
	}

	private void Load()
	{
		_globals.AutoStarted = false;
		if (_loadDialogUsed)
		{
			_loadDialog.Popup();
		}
		else
		{
			_loadDialogUsed = true;
			if (_globals.Preferences.LastGame != null)
				_loadDialog.CurrentPath = _globals.Preferences.LastGame;
			_loadDialog.PopupCentered();
		}
	}

	private void LoadGame(string path)
	{
		_globals.TilesLoading = true;
		_globals.TilesLoadPath = path;
		_tree.ChangeScene("res://Game.tscn");
	}

	private void NewGame()
	{
		_globals.AutoStarted = false;
		_tree.ChangeScene("res://NewGame.tscn");
	}

	private void OnLoadDialogFileSelected(string path)
	{
		// Save path if different than last loaded game
        if (_globals.Preferences.LastGame != path)
        {
            _globals.Preferences.LastGame = path;
            _globals.Preferences.Save(Preferences.P_PREFS);
        }
		LoadGame(path);
	}

	private void OnLoadPressed()
	{
		Load();
	}

	private void OnNewPressed()
	{
		NewGame();
	}

	private void OnPrefsPressed()
	{
		Prefs();
	}

	private void OnQuitPressed()
	{
		Quit();
	}

	private void Prefs()
	{
		_tree.ChangeScene("res://Prefs.tscn");
	}

	private void Quit()
	{
		_tree.Quit();
	}
}
