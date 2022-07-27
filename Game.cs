#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146, IDE0044, RCS1169, IDE0052

using Godot;
using System;

public class Game : Control
{
    Button _hintClose;
    WindowDialog _hintDialog;
    bool _hintDialogUsed;
    TextureRect _hintTexture;
    VBoxContainer _gameBoard;
    Globals _globals;
    FileDialog _loadDialog;
    bool _loadDialogUsed;
    LineEdit _moves;
    MenuButton _prefs;
    FileDialog _saveDialog;
    bool _saveDialogUsed;
    TilesControl _tiles;
    SceneTree _tree;
    Button _winnerClose;
    WindowDialog _winnerDialog;
    Label _winnerLabel;

    public override void _Input(InputEvent ev)
    {
        base._Input(ev);
		if (ev.IsActionPressed("quit"))
		{
			AcceptEvent();
            Abort();
			return;
		}
		if (ev.IsActionPressed("refresh"))
		{
			AcceptEvent();
			Update();
			return;
		}
    }

    public override void _Ready()
    {
        base._Ready();
        _hintClose = GetNode<Button>("HintDialog/CloseHint");
        _hintDialog = GetNode<WindowDialog>("HintDialog");
        _hintDialogUsed = false;
        _hintTexture = GetNode<TextureRect>("HintDialog/HintImage");
        _gameBoard = GetNode<VBoxContainer>("GameBoard");
        _globals = GetNode<Globals>("/root/Globals");
        _loadDialog = GetNode<FileDialog>("LoadDialog");
        _loadDialogUsed = false;
        _moves = GetNode<LineEdit>("GameBoard/PanelContainer/HFlowContainer2/HFlowContainer3/Moves");
        _prefs = GetNode<MenuButton>("GameBoard/PanelContainer/HFlowContainer2/HFlowContainer/Prefs");
        _saveDialog = GetNode<FileDialog>("SaveDialog");
        _saveDialogUsed = false;
        _tiles = GetNode<TilesControl>("GameBoard/MarginContainer/Tiles");
        _tree = GetTree();
        _winnerClose = GetNode<Button>("WinnerDialog/CloseWinner");
        _winnerDialog = GetNode<WindowDialog>("WinnerDialog");
        _winnerLabel = GetNode<Label>("WinnerDialog/CenterContainer/Label");

        // Connct to popup menu item selection (no way to do this in editor)
        PopupMenu pm = _prefs.GetPopup();
        pm.Connect("index_pressed", this, "OnPrefsItemSelected");

        // Update GUI for common settings
        _tiles.NumberColor = _globals.Preferences.NumberColor;
        _tiles.OutlineColor = _globals.Preferences.OutlineColor;
        _tiles.ShowNumbers = _globals.Preferences.ShowNumbers;
        pm.SetItemChecked(1, _globals.Preferences.ShowNumbers);
        _tiles.ShowOutlines = _globals.Preferences.ShowOutlines;
        pm.SetItemChecked(0, _globals.Preferences.ShowOutlines);

        // Not all globals are used if loading a game.
        if (_globals.TilesLoading)
        {
            _globals.TilesLoading = false;
            _tiles.LoadGame(_globals.TilesLoadPath);
        }
        else
        {
            _tiles.Columns = _globals.Preferences.Columns;
            _tiles.Rows = _globals.Preferences.Rows;
            if (_globals.TilesUseImage)
            {
                if (_globals.TilesDefaultImage)
                    _tiles.ImagePath = _globals.Preferences.DefaultImage;
                else
                    _tiles.ImagePath = _globals.Preferences.LastImage;
                pm.SetItemDisabled(0, false);
                pm.SetItemDisabled(1, false);
            }
            else
            {
                _tiles.ImagePath = "";
                pm.SetItemDisabled(0, true);
                pm.SetItemDisabled(1, true);
            }
            _tiles.Start();
        }
    }

    private void Abort()
    {
        _tree.ChangeScene("res://Main.tscn");
    }

    void HideHint()
    {
        _gameBoard.Show();
        _hintDialog.Hide();
    }

    Image LoadImage(string path)
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

    void OnAbortPressed()
    {
        Abort();
    }

    void OnHintDialogClosePressed()
    {
        HideHint();
    }

    void OnHintDialogPopupHide()
    {
        HideHint();
    }

    void OnHintDialogResized()
    {
        if (_hintDialog != null)
            UpdateHintDialog();
    }

    void OnHintDialogItemRectChanged()
    {
        if (_hintDialog != null)
            UpdateHintDialog();
    }

    void OnHintDialogSizeChanged()
    {
        if (_hintDialog != null)
            UpdateHintDialog();
    }

    void OnHintPressed()
    {
        _gameBoard.Hide();
        if (_hintTexture.Texture == null)
        {
            Image image;
            if (_globals.TilesDefaultImage)
                image = LoadImage(_globals.Preferences.DefaultImage);
            else
                image = LoadImage(_globals.Preferences.LastImage);
            ImageTexture it = new ImageTexture();
            it.CreateFromImage(image);
            _hintTexture.Texture = it;
        }
        if (_hintDialogUsed)
        {
            _hintDialog.Popup_();
        }
        else
        {
            _hintDialogUsed = true;
           _hintDialog.PopupCentered();
        }
    }

    void OnLoadDialogFileSelected(string path)
    {
        if (_globals.Preferences.LastGame != path)
        {
            _globals.Preferences.LastGame = path;
            _globals.Preferences.Save(Preferences.P_PREFS);
        }
        _tiles.LoadGame(path);
        _tiles.Movable = true;
    }

    void OnLoadDialogPopupHide()
    {
        _tiles.Movable = true;
    }

    void OnLoadPressed()
    {
        _tiles.Movable = false;
        if (_loadDialogUsed)
        {
            _loadDialog.Popup_();
        }
        else
        {
            _loadDialogUsed = true;
			if (_globals.Preferences.LastGame != null)
				_loadDialog.CurrentPath = _globals.Preferences.LastGame;
            _loadDialog.PopupCentered();
        }
    }

    void OnPrefsItemSelected(int index)
    {
        PopupMenu pm = _prefs.GetPopup();
        pm.ToggleItemChecked(index);
        bool on = pm.IsItemChecked(index);
        switch (index)
        {
        case 0: // Outlines
            _tiles.ShowOutlines = on;
            break;

        case 1: // Numbers
            _tiles.ShowNumbers = on;
            break;
        }
    }

    void OnQuitPressed()
    {
        // Save game if desired.
        if (_globals.Preferences.AutoSave)
            _tiles.SaveGame(_globals.Preferences.AutoPath);

        // Get outta here.
        _tree.Quit();
    }

    void OnSaveDialogFileSelected(string path)
    {
        _tiles.SaveGame(path);
        _tiles.Movable = true;
    }

    void OnSaveDialogPopupHide()
    {
        _tiles.Movable = true;
    }

    void OnSavePressed()
    {
        _tiles.Movable = false;
        if (_saveDialogUsed)
        {
            _saveDialog.Popup_();
        }
        else
        {
            _saveDialogUsed = true;
            _saveDialog.PopupCentered();
        }
    }

    void OnTilesItemRectChanged()
    {
        if (_tiles != null)
            _tiles.RecalcTiles();
    }

    void OnTilesMoved(int count)
    {
        _moves.Text = count.ToString();
    }

    void OnTilesWon()
    {
        // Update message.
        _winnerLabel.Text = $"Congratulations!\nYou won in\n{_moves.Text}\nmoves!";

        // Size everything correctly.
        _winnerLabel.RectPosition = new Vector2(10, 10);
        _winnerLabel.RectSize = new Vector2(_winnerDialog.RectSize.x - 20,
                _winnerDialog.RectSize.y - 30 - _winnerClose.RectSize.y);
        _winnerClose.RectPosition = new Vector2((_winnerDialog.RectSize.x - _winnerClose.RectSize.x) / 2,
                _winnerDialog.RectSize.y - 10 - _winnerClose.RectSize.y);

        // If game was auto started, then remove auto save if desired
        if (_globals.AutoStarted && _globals.Preferences.AutoRemoveOnWin)
        {
            Directory d = new Directory();
            if (d.FileExists(_globals.Preferences.AutoPath))
                d.Remove(_globals.Preferences.AutoPath);
        }

        // Notify user.
        _winnerDialog.PopupCentered();
    }

    void OnWinnerDialogClosePressed()
    {
        Abort();
    }

    void UpdateHintDialog()
    {
        Vector2 ws = _hintDialog.RectSize;
        Vector2 cs = _hintClose.RectSize;
        _hintTexture.RectPosition = new Vector2(10, 10);
        _hintTexture.RectSize = new Vector2(ws.x - 20, ws.y - 25 - cs.y);
        _hintClose.RectPosition = new Vector2((ws.x - cs.x) / 2, ws.y - cs.y - 10);
    }
}
