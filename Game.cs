#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146, IDE0052

using Godot;
using System;

public partial class Game : Control
{
    Button _hintClose;
    Window _hintDialog;
    bool _hintDialogUsed;
    TextureRect _hintTexture;
    VBoxContainer _gameBoard;
    Globals _globals;
    FileDialog _loadDialog;
    bool _loadDialogUsed;
    LineEdit _moves;
    FileDialog _saveDialog;
    bool _saveDialogUsed;
    TilesControl _tiles;
    SceneTree _tree;
    Button _winnerClose;
    Window _winnerDialog;
    Label _winnerLabel;

    public override void _Ready()
    {
        _hintClose = GetNode<Button>("HintDialog/CloseHint");
        _hintDialog = GetNode<Window>("HintDialog");
        _hintDialogUsed = false;
        _hintTexture = GetNode<TextureRect>("HintDialog/HintImage");
        _gameBoard = GetNode<VBoxContainer>("GameBoard");
        _globals = GetNode<Globals>("/root/Globals");
        _loadDialog = GetNode<FileDialog>("LoadDialog");
        _loadDialogUsed = false;
        _moves = GetNode<LineEdit>("GameBoard/HFlowContainer/Moves");
        _saveDialog = GetNode<FileDialog>("SaveDialog");
        _saveDialogUsed = false;
        _tiles = GetNode<TilesControl>("GameBoard/MarginContainer/Tiles");
        _tree = GetTree();
        _winnerClose = GetNode<Button>("WinnerDialog/CloseWinner");
        _winnerDialog = GetNode<Window>("WinnerDialog");
        _winnerLabel = GetNode<Label>("WinnerDialog/Label");

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
        _tiles.NumberColor = _globals.TilesNumberColor;
        _tiles.NumberFont = _globals.TilesNumberFont;
        _tiles.OutlineColor = _globals.TilesOutlineColor;
        _tiles.ShowNumbers = _globals.TilesShowNumbers;
    }

    void HideHint()
    {
        _gameBoard.Show();
        _hintDialog.Hide();
    }

    void OnAbortPressed()
    {
        _tree.ChangeScene("res://Main.tscn");
    }

    void OnHintDialogClosePressed()
    {
        HideHint();
    }

    void OnHintDialogSizeChanged()
    {
        Vector2 ws = _hintDialog.Size;
        Vector2 cs = _hintClose.Size;
        _hintTexture.Position = new Vector2(10, 10);
        _hintTexture.Size = new Vector2(ws.x - 20, ws.y - 25 - cs.y);
        _hintClose.Position = new Vector2((ws.x - cs.x) / 2, ws.y - cs.y - 10);
    }

    void OnHintPressed()
    {
        _gameBoard.Hide();
        if (_hintTexture.Texture == null)
        {
            Image image = new();
            if (_globals.TilesDefaultImage)
                image.Load(_globals.TilesImageDefault);
            else
                image.Load(_globals.TilesImagePath);
            _hintTexture.Texture = ImageTexture.CreateFromImage(image);
        }
        if (_hintDialogUsed)
        {
            _hintDialog.Popup();
        }
        else
        {
            _hintDialogUsed = true;
           _hintDialog.PopupCentered();
        }
    }

    void OnLoadDialogFileSelected(string path)
    {
        _tiles.Load(path);
    }

    void OnLoadPressed()
    {
        if (_loadDialogUsed)
        {
            _loadDialog.Popup();
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
        _tiles.Save(path);
    }

    void OnSavePressed()
    {
        if (_saveDialogUsed)
        {
            _saveDialog.Popup();
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
        _winnerLabel.Position = new Vector2(10, 10);
        _winnerLabel.Size = new Vector2(_winnerDialog.Size.x - 20,
                _winnerDialog.Size.y - 30 - _winnerClose.Size.y);
        _winnerClose.Position = new Vector2((_winnerDialog.Size.x - _winnerClose.Size.x) / 2,
                _winnerDialog.Size.y - 10 - _winnerClose.Size.y);
        _winnerDialog.PopupCentered();
    }

    void OnWinnerDialogClosePressed()
    {
        _tree.ChangeScene("res://Main.tscn");
    }
}
