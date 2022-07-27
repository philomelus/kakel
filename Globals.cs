#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146, IDE0044, RCS1169, IDE0052

using Godot;
using System;

public class Globals : Node
{
	// When true, the game was started from AutoPath because AutoLoad was true
	public bool AutoStarted = false;

	// Preferences that are saved/loaded
	public Preferences Preferences { get; set; }

	// If true, use default image.
	public bool TilesDefaultImage = true;

	// When true, the game in `TilesLoadPath` is loaded and restarted
	public bool TilesLoading = false;

	// When TilesLoading is true, path to game file to load.
	public string TilesLoadPath;

	// When true, use image for tiles.  Otherwise draw outlines with numbers.
	public bool TilesUseImage = true;

    public override void _Ready()
    {
        base._Ready();
		Preferences = new Preferences();
    }
}
