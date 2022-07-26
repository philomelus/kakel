#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146, IDE0044, RCS1169, IDE0052

using Godot;
using System;

public class Prefs : Control
{
    private CheckButton _autoLoad;
    private CheckButton _autoSave;
    private SpinBox _columns;
    private Globals _globals;
    private ColorPickerButton _outlineColor;
    private ColorPickerButton _numberColor;
    private SpinBox _rows;
    private CheckButton _showNumbers;
    private SceneTree _tree;

    public override void _Ready()
    {
        _autoLoad = GetNode<CheckButton>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/AutoLoad");
        _autoSave = GetNode<CheckButton>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/AutoSave");
        _columns = GetNode<SpinBox>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Columns");
        _globals = GetNode<Globals>("/root/Globals");
        _outlineColor = GetNode<ColorPickerButton>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/OutlineColor");
        _numberColor = GetNode<ColorPickerButton>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/NumberColor");
        _rows = GetNode<SpinBox>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/Rows");
        _showNumbers = GetNode<CheckButton>("PanelContainer/CenterContainer/VBoxContainer/GridContainer/ShowNumbers");
        _tree = GetTree();

        // Pass globals to controls
        _autoLoad.Pressed = _globals.Preferences.AutoLoad;
        _autoSave.Pressed = _globals.Preferences.AutoSave;
        _columns.Value = _globals.Preferences.Columns;
        _numberColor.Color = _globals.Preferences.NumberColor;
        _outlineColor.Color = _globals.Preferences.OutlineColor;
        _rows.Value = _globals.Preferences.Rows;
        _showNumbers.Pressed = _globals.Preferences.ShowNumbers;
    }

    public void OnCancelPressed()
    {
        Quit();
    }

    public void OnSavePressed()
    {
        // Update globals from controls
        int updates = 0;
        if (_globals.Preferences.AutoLoad != _autoLoad.Pressed)
        {
            _globals.Preferences.AutoLoad = _autoLoad.Pressed;
            ++updates;
        }
        if (_globals.Preferences.AutoSave != _autoSave.Pressed)
        {
            _globals.Preferences.AutoSave = _autoSave.Pressed;
            ++updates;
        }
        if (_globals.Preferences.Columns != (int) _columns.Value)
        {
            _globals.Preferences.Columns = (int) _columns.Value;
            ++updates;
        }
        if (_globals.Preferences.NumberColor != _numberColor.Color)
        {
            _globals.Preferences.NumberColor = _numberColor.Color;
            ++updates;
        }
        if (_globals.Preferences.OutlineColor != _outlineColor.Color)
        {
            _globals.Preferences.OutlineColor = _outlineColor.Color;
            ++updates;
        }
        if (_globals.Preferences.Rows != (int) _rows.Value)
        {
            _globals.Preferences.Rows = (int) _rows.Value;
            ++updates;
        }
        if (_globals.Preferences.ShowNumbers != _showNumbers.Pressed)
        {
            _globals.Preferences.ShowNumbers = _showNumbers.Pressed;
            ++updates;
        }

        // Save settings if needed
        if (updates > 0)
            _globals.Preferences.Save(Preferences.P_PREFS);

        // Return to main menu
        Quit();
    }

    private void Quit()
    {
        _tree.ChangeScene("res://Main.tscn");
    }
}