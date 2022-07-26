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
    FileDialog _saveDialog;
    bool _saveDialogUsed;
    CheckButton _showNumbers;
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
        _moves = GetNode<LineEdit>("GameBoard/PanelContainer/HFlowContainer/Moves");
        _saveDialog = GetNode<FileDialog>("SaveDialog");
        _saveDialogUsed = false;
        _showNumbers = GetNode<CheckButton>("GameBoard/PanelContainer/HFlowContainer/ShowNumbers");
        _tiles = GetNode<TilesControl>("GameBoard/MarginContainer/Tiles");
        _tree = GetTree();
        _winnerClose = GetNode<Button>("WinnerDialog/CloseWinner");
        _winnerDialog = GetNode<WindowDialog>("WinnerDialog");
        _winnerLabel = GetNode<Label>("WinnerDialog/Label");

        // If loading a game, no need to pass globals on
        _tiles.NumberColor = _globals.TilesNumberColor;
        if (_tiles.NumberFont == null)
            _tiles.NumberFont = _globals.TilesNumberFont;
        _tiles.OutlineColor = _globals.TilesOutlineColor;
        if (_globals.TilesLoading)
        {
            _globals.TilesLoading = false;
            _tiles.LoadGame(_globals.TilesLoadPath);
            CallDeferred("SetShowNumbers");
        }
        else
        {
            // Pass on global settings to tiles control.
            _tiles.Columns = _globals.TilesColumns;
            _tiles.Rows = _globals.TilesRows;
            if (_globals.TilesUseImage)
            {
                if (_globals.TilesDefaultImage)
                    _tiles.ImagePath = _globals.TilesImageDefault;
                else
                    _tiles.ImagePath = _globals.TilesImagePath;
            }
            else
            {
                _tiles.ImagePath = "";
            }
            _tiles.ShowNumbers = _globals.TilesShowNumbers;
            _showNumbers.Pressed = _globals.TilesShowNumbers;
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
                image = LoadImage(_globals.TilesImageDefault);
            else
                image = LoadImage(_globals.TilesImagePath);
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
        _tiles.LoadGame(path);
    }

    void OnLoadPressed()
    {
        if (_loadDialogUsed)
        {
            _loadDialog.Popup_();
        }
        else
        {
            _loadDialogUsed = true;
            _loadDialog.PopupCentered();
        }
    }

     void OnQuitPressed()
    {
        _tree.Quit();
    }

    void OnSaveDialogFileSelected(string path)
    {
        _tiles.SaveGame(path);
    }

    void OnSavePressed()
    {
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

    void OnShowNumbersPressed()
    {
        _tiles.ShowNumbers = _showNumbers.Pressed;
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
        _winnerLabel.RectPosition = new Vector2(10, 10);
        _winnerLabel.RectSize = new Vector2(_winnerDialog.RectSize.x - 20,
                _winnerDialog.RectSize.y - 30 - _winnerClose.RectSize.y);
        _winnerClose.RectPosition = new Vector2((_winnerDialog.RectSize.x - _winnerClose.RectSize.x) / 2,
                _winnerDialog.RectSize.y - 10 - _winnerClose.RectSize.y);
        _winnerDialog.PopupCentered();
    }

    void OnWinnerDialogClosePressed()
    {
        Abort();
    }

    private void SetShowNumbers()
    {
        _showNumbers.Pressed = _tiles.ShowNumbers;
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
