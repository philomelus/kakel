#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146, IDE0052

using Godot;
using System;

public partial class Globals : Node
{
	// Number of columns in tiles.
	public int TilesColumns = 4;

	// If true, use default image.
	public bool TilesDefaultImage = true;

	// Image to use for default image.
	public string TilesImageDefault = "res://default_image.png";

	// If `TilesDefaultImage` is false and TilesUseImag is true, this
	// contains the path of the image to use.
	public string TilesImagePath;

	// When true, the main display will load the game in `TilesLoadPath` rather
	// than using TilesColumns/TilesRows/TilesImage/TilesDefaultImage, etc.
	public bool TilesLoading = false;

	// When TilesLoading is true, path to game file to load.
	public string TilesLoadPath;

	// Color used to draw tile numbers.
	public Color TilesNumberColor = Colors.White;

	// Font used to draw tile numbers.
	public Font TilesNumberFont;

	// Color to draw tile outlines when TilesUseImage is false.
	public Color TilesOutlineColor = Colors.White;

	// Number of rows in tiles.
	public int TilesRows = 4;

	// When true, show tile numbers on tiles.
	public bool TilesShowNumbers = true;

	// When true, use image for tiles.  Otherwise draw outlines with numbers.
	public bool TilesUseImage = true;

	public override void _Ready()
	{
	}

	// Load global preferences from file.
	public void Load(string path)
	{
		File inp = new();
		inp.Open(path, File.ModeFlags.Read);
	}

	// Save global preferences into file.
	public void Save(string path)
	{
		File outp = new();
		outp.Open(path, File.ModeFlags.Write);
	}
}
