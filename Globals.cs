#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146

using Godot;
using System;

public class Globals : Node
{
	// Color for tile edges
	public Color TilesColor = new Color(0, 0, 1, 1);

	// Color for tile fonts
	public Color TilesFontColor = new Color(0, 1, 0, 1);

	// Image used on tiles
	public Image TilesImage  = null;
	public string TilesImagePath = "";

	// Default image for tiles (used as TilesImage when not overridden)
	public Image TilesImageDefault = null;

	// Size of the tile board
	public Vector2 TilesSize = new Vector2(4, 4);

	// When false, draw rect for tiles instead of image
	public bool TilesUseImage = true;

	// When true, game is loading rather than starting new so
	// TileControl should skip initialization as load will do
	// it after construction.
	public bool TilesLoading = false;
	public string TilesLoadPath = "";

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		TilesImageDefault = ResourceLoader.Load<Image>("res://default_image.png", "Image", false);
	}
}
