#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146

using Godot;
using System;

public class Game : Control
{
    Button _closeHint;
    Globals _globals;
    WindowDialog _hintDialog;
    FileDialog _loadDialog;
    LineEdit _moves;
    FileDialog _saveDialog;
    TextureRect _textureRect;
    TilesControl _tiles;
    SceneTree _tree;
    VBoxContainer _vboxContainer;
    WindowDialog _win;

    public override void _Ready()
    {
        _closeHint = GetNode<Button>("HintDialog/CloseHint");
        _hintDialog = GetNode<WindowDialog>("HintDialog");
        _globals = GetNode<Globals>("/root/Globals");
        _loadDialog = GetNode<FileDialog>("LoadDialog");
        _moves = GetNode<LineEdit>("PanelContainer/VBoxContainer/HFlowContainer/Moves");
        _saveDialog = GetNode<FileDialog>("SaveDialog");
        _textureRect = GetNode<TextureRect>("HintDialog/TextureRect");
        _tiles = GetNode<TilesControl>("PanelContainer/VBoxContainer/Tiles");
        _tree = GetTree();
        _vboxContainer = GetNode<VBoxContainer>("PanelContainer/VBoxContainer");
        _win = GetNode<WindowDialog>("WinnerDialog");
    }

    private void HideHint()
    {
        _tree.Paused = false;
        _vboxContainer.Show();
        _hintDialog.Hide();
    }

    private void OnAbortPressed()
    {
        _tree.ChangeScene("res://Main.tscn");
    }

    private void OnHintDialogClosePressed()
    {
        HideHint();
    }

    private void OnHintDialogItemRectChanged()
    {
        Vector2 s = _hintDialog.RectSize;
        Vector2 cs = _closeHint.RectSize;
        _textureRect.RectSize = new Vector2(s.x - 20, s.y - 50);
        _closeHint.RectPosition = new Vector2((s.x - cs.x) / 2, s.y - cs.y - 10);
    }

    private void OnHintDialogModalClosed()
    {
        HideHint();
    }

    private void OnHintDialogPopupHide()
    {
        HideHint();
    }

    private void OnHintPressed()
    {
        _tree.Paused = true;
        _vboxContainer.Hide();
        if (_textureRect.Texture == null)
        {
            Image image;
            if (_globals.TilesImage == null)
                image = _globals.TilesImageDefault.Duplicate() as Image;
            else
                image = _globals.TilesImage.Duplicate() as Image;
            ImageTexture t = new ImageTexture();
            t.CreateFromImage(image);
            _textureRect.Texture = t;
        }
        _hintDialog.PopupCentered();
    }

    private void OnLoadDialogFileSelected(string path)
    {
        _tiles.Load(path);
        _tree.Paused = false;
    }

    private void OnLoadDialogModalClosed()
    {
        _tree.Paused = false;
    }

    private void OnLoadDialogPopupHide()
    {
        _tree.Paused = false;
    }

    private void OnLoadPressed()
    {
        _tree.Paused = true;
        _loadDialog.PopupCentered();
    }

    private void OnQuitPressed()
    {
        _tree.Quit();
    }

    private void OnSaveDialogFileSelected(string path)
    {
        _tiles.Save(path);
        _tree.Paused = false;
    }

    private void OnSaveDialogModalClosed()
    {
        _tree.Paused = false;
    }

    private void OnSaveDialogPopupHide()
    {
        _tree.Paused = false;
    }

    private void OnSavePressed()
    {
        _tree.Paused = true;
        _saveDialog.PopupCentered();
    }

    private void OnTilesItemRectChanged()
    {
        if (_tiles != null)
            _tiles.RecalcTiles();
    }

    private void OnTilesMoved(int count)
    {
        _moves.Text = count.ToString();
    }

    private void OnTilesWon()
    {
        _tree.Paused = true;
        _win.PopupCentered();
    }

    private void OnWinnerDialogClosePressed()
    {
    }
}
