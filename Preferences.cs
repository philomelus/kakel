#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146, IDE0044, RCS1169, IDE0052

using Godot;
using System;

public class Preferences : Node
{
    private const int PREFS_VERSION = 1;

    // Sections.
    private const string S_GLOBALS = "globals";

    // Values.
    private const string V_VERSION = "version";
    private const string V_AUTOLOAD = "auto_load";
    private const string V_AUTOPATH = "auto_path";
    private const string V_AUTOREMOVEONWIN = "auto_remove_on_win";
    private const string V_AUTOSAVE = "auto_save";
    private const string V_COLUMNS = "columns";
    private const string V_DEFAULTIMAGE = "default_image";
    private const string V_LASTGAME = "last_game";
    private const string V_LASTIMAGE = "last_image";
    private const string V_NUMBERCOLOR = "number_color";
    private const string V_OUTLINECOLOR = "outline_color";
    private const string V_ROWS = "rows";
    private const string V_SHOWNUMBERS = "show_numbers";
    private const string V_SHOWOUTLINES = "show_outlines";

    // Paths.
    public const string P_DEFAULTAUTOPATH = "user://auto.kakel";
    public const string P_DEFAULTIMAGE = "res://default_image.png";
    public const string P_PREFS = "user://prefs.cfg";

    // Preferences.
    public bool AutoLoad { get; set; }
    public string AutoPath { get; set; }
    public bool AutoRemoveOnWin { get; set; }
    public bool AutoSave { get; set; }
    public int Columns { get; set; }
    public string DefaultImage { get; set; }
    public string LastGame { get; set; }
    public string LastImage { get; set; }
    public Color NumberColor { get; set; }
    public Color OutlineColor { get; set; }
    public int Rows { get; set; }
    public bool ShowOutlines { get; set; }
    public bool ShowNumbers { get; set; }

    // Preferences default values.
    // NOTE:  <sigh> Some Godot objects aren't able to be const.
    private const bool default_AutoLoad = true;
    private const string default_AutoPath = P_DEFAULTAUTOPATH;
    private const bool default_AutoRemoveOnWin = true;
    private const bool default_AutoSave = true;
    private const int default_Columns = 4;
    private const string default_DefaultImage = P_DEFAULTIMAGE;
    private const string default_LastGame = null;
    private const string default_LastImage = null;
    private Color default_NumberColor = Colors.LightGray;
    private Color default_OutlineColor = Colors.Gray;
    private const int default_Rows = 4;
    private const bool default_ShowNumbers = true;
    private const bool default_ShowOutlines = false;

    public override void _Ready()
    {
        base._Ready();

        // Initialize all default values here
        AutoLoad = default_AutoLoad;
        AutoPath = default_AutoPath;
        AutoRemoveOnWin = default_AutoRemoveOnWin;
        AutoSave = default_AutoSave;
        Columns = default_Columns;
        DefaultImage = default_DefaultImage;
        LastGame = default_LastGame;
        LastImage = default_LastImage;
        NumberColor = default_NumberColor;
        OutlineColor = default_OutlineColor;
        Rows = default_Rows;
        ShowOutlines = default_ShowOutlines;
        ShowNumbers = default_ShowNumbers;
    }

    // Load preferences from file
    public void Load(string path)
    {
        ConfigFile cf = new ConfigFile();
        cf.Load(path);
        if ((int) cf.GetValue(S_GLOBALS, V_VERSION, PREFS_VERSION) != PREFS_VERSION)
            throw new ArgumentOutOfRangeException();
        AutoLoad = (int) cf.GetValue(S_GLOBALS, V_AUTOLOAD, default_AutoLoad ? 1 : 0) == 1;
        AutoPath = (string) cf.GetValue(S_GLOBALS, V_AUTOPATH, default_AutoPath);
        AutoRemoveOnWin = (int) cf.GetValue(S_GLOBALS, V_AUTOREMOVEONWIN, default_AutoRemoveOnWin ? 1 : 0) == 1;
        AutoSave = (int) cf.GetValue(S_GLOBALS, V_AUTOSAVE, default_AutoSave ? 1 : 0) == 1;
        Columns = (int) cf.GetValue(S_GLOBALS, V_COLUMNS, default_Columns);
        DefaultImage = (string) cf.GetValue(S_GLOBALS, V_DEFAULTIMAGE, default_DefaultImage);
        LastGame = (string) cf.GetValue(S_GLOBALS, V_LASTGAME, default_LastGame);
        LastImage = (string) cf.GetValue(S_GLOBALS, V_LASTIMAGE, default_LastImage);
        NumberColor = new Color((string) cf.GetValue(S_GLOBALS, V_NUMBERCOLOR, default_NumberColor.ToHtml(true)));
        OutlineColor = new Color((string) cf.GetValue(S_GLOBALS, V_OUTLINECOLOR, default_OutlineColor.ToHtml(true)));
        Rows = (int) cf.GetValue(S_GLOBALS, V_ROWS, default_Rows);
        ShowOutlines = (int) cf.GetValue(S_GLOBALS, V_SHOWOUTLINES, default_ShowOutlines ? 1 : 0) == 1;
        ShowNumbers = (int) cf.GetValue(S_GLOBALS, V_SHOWNUMBERS, default_ShowNumbers ? 1 : 0) == 1;
    }

    // Save preferences to file
    public void Save(string path)
    {
        ConfigFile cf = new ConfigFile();
        cf.SetValue(S_GLOBALS, V_VERSION, PREFS_VERSION);
        cf.SetValue(S_GLOBALS, V_AUTOLOAD, AutoLoad ? 1 : 0);
        cf.SetValue(S_GLOBALS, V_AUTOPATH, AutoPath);
        cf.SetValue(S_GLOBALS, V_AUTOREMOVEONWIN, AutoRemoveOnWin ? 1 : 0);
        cf.SetValue(S_GLOBALS, V_AUTOSAVE, AutoSave ? 1 : 0);
        cf.SetValue(S_GLOBALS, V_COLUMNS, (int) Columns);
        cf.SetValue(S_GLOBALS, V_DEFAULTIMAGE, DefaultImage);
        cf.SetValue(S_GLOBALS, V_LASTGAME, LastGame);
        cf.SetValue(S_GLOBALS, V_LASTIMAGE, LastImage);
        cf.SetValue(S_GLOBALS, V_NUMBERCOLOR, NumberColor.ToHtml(true));
        cf.SetValue(S_GLOBALS, V_OUTLINECOLOR, OutlineColor.ToHtml(true));
        cf.SetValue(S_GLOBALS, V_ROWS, (int) Rows);
        cf.SetValue(S_GLOBALS, V_SHOWOUTLINES, ShowOutlines ? 1 : 0);
        cf.SetValue(S_GLOBALS, V_SHOWNUMBERS, ShowNumbers ? 1 : 0);
        cf.Save(path);
    }
}
