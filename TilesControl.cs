#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146

using Godot;
using System;
using System.Collections.Generic;

public class TilesControl : Control
{
	// For save files, version of file format
	public const int FILE_VERSION = 1;

	// When false, no movement is allowed
	bool moves_enabled = true;

	// Precalc these as they are used several times
	int columns;	// columns in tiles
	int columns0;	// columns in tile minus 1
	int rows;		// rows in tiles
	int rows0;		// rows in tiles minus 1

	// Total number of tiles
	int num_tiles;
	int num_tiles0;

	// Spacing between each tile
	[Export]
	Vector2 tile_spacing = new Vector2(5, 5);

	// Font for drawing numerical id's of tiles
	DynamicFont tile_font;

	// Image/Texture for display
	Image tiles_image;
	ImageTexture tiles_texture;

	// Size of each tile in pixels
	Vector2 tile_size;

	// Data for each tile.  Each tile is a two item Rect array
	Rect2[][] tiles;
	const int IDX_DEST = 0;
	const int IDX_SRC = 1;
    bool tiles_ready = false;

	// Actual positions of each tile (i.e. index 0 = tile at upper left)
	int[] tiles_order;

	// When true, move_*_index holds index of tile than can move there
	bool can_move_down = false;
	bool can_move_left = false;
	bool can_move_right = false;
	bool can_move_up = false;

	// Index within tiles_order that moves in direction
	int move_down_index;
	int move_left_index;
	int move_right_index;
	int move_up_index;

	// Number of movements made since initial display
	int moves = 0;

	// This is signalled every time a tile is moved
	[Signal]
	public delegate void moved(int count);

	// Generated when Load is called, and it fails
	public enum WHY { UNKNOWN, BAD_VERSION };
	[Signal]
	public delegate void load_failed(string path, WHY why);

	// This is signalled when the tiles are all in correct order
	[Signal]
	public delegate void won();

	// Index within tiles of the empty tile
	int empty;

	// Index value of the empty tile (always the last global tile index)
	int empty_id;

	SceneTree _tree;
	Globals _globals;
    bool readyToRun = false;

    public override void _Draw()
    {
        base._Draw();
        if (!readyToRun || !tiles_ready)
            return;
 		Vector2 extent;
		if (_globals.TilesLoading)
		{
			const string message = "Loading ... one moment please";
			extent = tile_font.GetStringSize(message);
			DrawString(tile_font, new Vector2(RectPosition.x + ((RectSize.x - extent.x) / 2),
											   RectPosition.y + ((RectSize.y - extent.y) / 2) + extent.y),
						message, _globals.TilesFontColor);
			return;
		}

		int index = 0;
		foreach (int tile_index in tiles_order)
		{
			if (tile_index != empty_id)
			{
				if (_globals.TilesUseImage)
				{
					DrawTextureRectRegion(tiles_texture, tiles[index][IDX_DEST],
										  tiles[tiles_order[index]][IDX_SRC]);
				}
				else
				{
					Rect2[] tile = tiles[index];
					Rect2 area = tile[IDX_DEST];
					string name = (tile_index + 1).ToString();
					extent = tile_font.GetStringSize(name);
					DrawRect(area, _globals.TilesColor, false);
					DrawString(tile_font, new Vector2(area.Position.x + ((area.Size.x - extent.x) / 2),
													   area.Position.y + ((area.Size.y - extent.y) / 2) + extent.y),
								name, _globals.TilesFontColor);
				}
			}
			++index;
		}
   }

    public override void _Input(InputEvent ev)
    {
		base._Input(ev);
        if (!readyToRun || !tiles_ready)
            return;

		// Do nothing while paused
		if (_tree.Paused || !moves_enabled)
			return;

		// Only handle mouse clicks here
		if (!(ev is InputEventMouseButton))
			return;
		InputEventMouseButton iemb = ev as InputEventMouseButton;

		// Only handle left mouse clicks
		if (iemb.ButtonIndex != 1)
			return;

		// Don't pay attention to double clicks
		if (iemb.Doubleclick)
			return;

		// Only repond to clicks when released
		if (iemb.Pressed)
			return;

		// Adjust position of click for our position
		Vector2 p = new Vector2(iemb.Position);
		p -= RectPosition;

		// If click was valid, take appropriate action
		if (can_move_down)
		{
			if (tiles[move_down_index][IDX_DEST].HasPoint(p))
			{
				AcceptEvent();
				MoveDown();
				return;
			}
		}
		if (can_move_left)
		{
			if (tiles[move_left_index][IDX_DEST].HasPoint(p))
			{
				AcceptEvent();
				MoveLeft();
				return;
			}
		}
		if (can_move_right)
		{
			if (tiles[move_right_index][IDX_DEST].HasPoint(p))
			{
				AcceptEvent();
				MoveRight();
				return;
			}
		}
		if (can_move_up)
		{
			if (tiles[move_up_index][IDX_DEST].HasPoint(p))
			{
				AcceptEvent();
				MoveUp();
				return;
			}
		}
    }

    public override void _PhysicsProcess(float delta)
    {
        if (!readyToRun || !tiles_ready)
            return;
		base._PhysicsProcess(delta);
		CheckComplete();
    }

    public override void _Ready()
    {
		base._Ready();
		tile_font = GD.Load<DynamicFont>("res://tile_font.tres");
		_tree = GetTree();
		_globals = GetNode<Globals>("/root/Globals");

		// Load save if requested
		if (_globals.TilesLoading)
		{
			readyToRun = true;
			Load(_globals.TilesLoadPath);
			_globals.TilesLoading = false;
			return;
		}

		// Precalc these as they are used several times
		columns = (int) _globals.TilesSize.x;
		columns0 = columns - 1;
		rows = (int) _globals.TilesSize.y;
		rows0 = rows - 1;

		// Total number of tiles
		num_tiles = columns * rows;
		num_tiles0 = num_tiles - 1;

		// Initial blank tiles is last tile
		empty = num_tiles0;
		empty_id = empty;

		// Need some random numbers
		RandomNumberGenerator rng = new RandomNumberGenerator();
		rng.Randomize();

		// Determine tile order
		tiles_order = new int[num_tiles];
		for (int i = 0; i < num_tiles; ++i)
			tiles_order[i] = 9999;
		int order;
		for(int i = 0; i < num_tiles; ++i)
		{
			if (i == empty)
			{
				tiles_order[i] = empty_id;
			}
			else
			{
				order = rng.RandiRange(0, num_tiles0 - 1);
				if (i > 0)
				{
					while (Array.IndexOf(tiles_order, order) != -1)
						order = rng.RandiRange(0, num_tiles0 - 1);
				}
				else
				{
					if (order == empty)
					{
						while (order == empty)
							order = rng.RandiRange(0, num_tiles0 - 1);
					}
				}
				tiles_order[i] = order;
			}
		}

		// Determine valid moves
        readyToRun = true;
        CallDeferred("RecalcTiles");
        CallDeferred("CalcMovables");
    }

    public override void _UnhandledInput(InputEvent ev)
    {
		base._UnhandledInput(ev);
        if (!readyToRun || !tiles_ready)
            return;

		// Do nothing when paused
		if (_tree.Paused || !moves_enabled)
			return;

		// If event isn't an action, ignore
		if (!ev.IsActionType())
			return;

		// Take appropriate action
		if (ev.IsActionPressed("refresh"))
		{
			AcceptEvent();
			Update();
			return;
		}
		if (ev.IsActionPressed("quit"))
		{
			AcceptEvent();
			_tree.ChangeScene("res://Main.tscn");
			return;
		}
        if (can_move_left)
		{
    		if (ev.IsActionPressed("move_left"))
			{
				AcceptEvent();
				MoveLeft();
				return;
			}
		}
        if (can_move_right)
		{
    		if (ev.IsActionPressed("move_right"))
			{
				AcceptEvent();
				MoveRight();
				return;
			}
		}
        if (can_move_up)
		{
    		if (ev.IsActionPressed("move_up"))
			{
				AcceptEvent();
				MoveUp();
				return;
			}
		}
        if (can_move_down)
		{
    		if (ev.IsActionPressed("move_down"))
			{
				AcceptEvent();
				MoveDown();
				return;
			}
		}
    }

    private void CalcMovables()
    {
        // Do nothing until initialized
        if (!readyToRun)
            return;

		// Row and column of blank tile
		int row = empty / columns;
		int column = empty % columns;

		// Determine tiles that can move horizontally
		bool left_side = (column == 0);
		bool right_side = (column == columns0);
		bool inside_h = (column > 0 && column < columns0);
		if (left_side)
		{
			can_move_left = true;
			can_move_right = false;
		}
		else if (right_side)
		{
			can_move_left = false;
			can_move_right = true;
		}
        else if (inside_h)
        {
            can_move_left = true;
			can_move_right = true;
		}
		if (can_move_left)
			move_left_index = (row * columns) + column + 1;
		if (can_move_right)
			move_right_index = (row * columns) + column - 1;

		// Determine tiles that can move vertically
		bool top_side = (row == 0);
		bool bottom_side = (row == rows0);
		bool inside_v = (row > 0 && row < rows0);
		if (top_side)
		{
			can_move_up = true;
			can_move_down = false;
		}
		else if (bottom_side)
		{
			can_move_up = false;
			can_move_down = true;
		}
		else if (inside_v)
		{
			can_move_up = true;
			can_move_down = true;
		}
		if (can_move_up)
			move_up_index = ((row + 1) * columns) + column;
		if (can_move_down)
			move_down_index = ((row - 1) * columns) + column;
    }

    private bool CheckComplete()
    {
		for(int index = 0; index < num_tiles; ++index)
		{
			if (tiles_order[index] != index)
				return false;
		}
		moves_enabled = false;
		EmitSignal("won");
		return true;
    }

    public void Load(string path)
    {
		// Ignore input until load is complete
		moves_enabled = false;

		File inp = new File();
		inp.Open(path, File.ModeFlags.Read);

		// Read file format version
		int v = (int) inp.Get16();
		if (v != FILE_VERSION)
		{
			EmitSignal("load_failed", path, WHY.BAD_VERSION);
			inp.Close();
			return;
		}

		// Load TilesSize
		float x = inp.GetFloat();
		float y = inp.GetFloat();

		// Load UseImage
		int useImage = (int) inp.Get8();

		// Load image path
		string imagePath = inp.GetPascalString();

		// Load tiles order
		int numTiles = (int) inp.Get32();
		int[] order = new int[numTiles];
		for (int i = 0; i < numTiles; ++i)
			order[i] = (int) inp.Get32();

		// Read blank tile index
		int e = (int) inp.Get32();
		int eid = (int) inp.Get32();

		// Read moves so far
		int m = (int) inp.Get64();

		// Done loading data
		inp.Close();

		// Internal variables are not correct, so disable
		tiles_ready = false;

		// Set globals
		_globals.TilesSize = new Vector2(x, y);
		_globals.TilesImagePath = imagePath;
		_globals.TilesUseImage = (useImage != 0);
		if (_globals.TilesUseImage && _globals.TilesImagePath != "")
		{
			_globals.TilesImage = new Image();
			_globals.TilesImage.Load(_globals.TilesImagePath);
		}

		// Set local parameters
		columns = (int) _globals.TilesSize.x;
		columns0 = columns - 1;
		rows = (int) _globals.TilesSize.y;
		rows0 = rows - 1;
		num_tiles = columns * rows;
		num_tiles0 = num_tiles - 1;
		tiles_order = order;
		moves = m;
		empty = e;
		empty_id = eid;

		// Reset tiles
		CallDeferred("RecalcTiles");
		CallDeferred("CalcMovables");

		// All ready
		moves_enabled = true;
		CallDeferred("update");

		// Number of moves changed
		CallDeferred("moved", moves);
    }

    public void MoveDown()
    {
		int newEmpty = tiles_order[move_down_index];
		tiles_order[move_down_index] = empty_id;
		tiles_order[empty] = newEmpty;
		empty = move_down_index;
		++moves;
		EmitSignal("moved", moves);
		CalcMovables();
		Update();
    }

    public void MoveLeft()
    {
		int newEmpty = tiles_order[move_left_index];
		tiles_order[move_left_index] = empty_id;
		tiles_order[empty] = newEmpty;
		empty = move_left_index;
		++moves;
		EmitSignal("moved", moves);
		CalcMovables();
		Update();
    }

    public void MoveRight()
    {
		int newEmpty = tiles_order[move_right_index];
		tiles_order[move_right_index] = empty_id;
		tiles_order[empty] = newEmpty;
		empty = move_right_index;
		++moves;
		EmitSignal("moved", moves);
		CalcMovables();
		Update();
    }

    public void MoveUp()
    {
		int newEmpty = tiles_order[move_up_index];
		tiles_order[move_up_index] = empty_id;
		tiles_order[empty] = newEmpty;
		empty = move_up_index;
		++moves;
		EmitSignal("moved", moves);
		CalcMovables();
		Update();
    }

    public void RecalcTiles()
    {
        // Do nothing if we didn't finish initializing yet
        if (!readyToRun)
            return;

        // Stop processing that relies on tiles being set
        tiles_ready = false;

		// This can be called before everything is ready
		if (columns == 0 || rows == 0)
			return;

		// Determine width and height of tiles from our size
		tile_size = new Vector2((RectSize.x - (columns0 * tile_spacing.x)) / columns,
								(RectSize.y - (rows0 * tile_spacing.y)) / rows);
		if (tile_size.x <= 0 || tile_size.y <= 0)
			return;

		// Resize image for display if needed
		if (_globals.TilesUseImage)
		{
			if (_globals.TilesImagePath.Length == 0)
				tiles_image = _globals.TilesImageDefault.Duplicate() as Image;
			else
				tiles_image = _globals.TilesImage.Duplicate() as Image;
			tiles_image.Resize((int) (tile_size.x * columns), (int) (tile_size.y * rows), Image.Interpolation.Lanczos);

			// Convert image to texture for display
			tiles_texture = new ImageTexture();
			tiles_texture.CreateFromImage(tiles_image, 0);
		}
		else
		{
			tiles_image = null;
		}

		// Calculate the bounding boxes for each tile for both display and image
		Vector2 tile_size_int = new Vector2((int) tile_size.x, (int) tile_size.y);
		tiles = new Rect2[num_tiles][];
		Rect2[] tile;
		float left;
		float top;
        int row;
        int col;
		for (row = 0; row < rows; ++row)
		{
			for (col = 0; col < columns; ++col)
			{
				tile = new Rect2[2];
				left = (col * tile_size.x) + (col * tile_spacing.x);
				top = (row * tile_size.y) + (row * tile_spacing.y);
				tile[IDX_DEST] = new Rect2(new Vector2(left, top), tile_size_int);
				left = (int) (col * tile_size.x);
				top =  (int) (row * tile_size.y);
				tile[IDX_SRC] = new Rect2(new Vector2(left, top), tile_size_int);
				tiles[(row * columns) + col ] = tile;
			}
		}
        tiles_ready = true;
    }

    public void Save(string path)
    {
        if (!readyToRun)
            return;

		File sav = new File();
		sav.Open(path, File.ModeFlags.Write);

		// Save file version
		sav.Store16(FILE_VERSION);

		// Save TilesSize
		sav.StoreFloat(_globals.TilesSize.x);
		sav.StoreFloat(_globals.TilesSize.y);

		// Save UseImage state
		if (_globals.TilesUseImage)
			sav.Store8(1);
		else
			sav.Store8(0);

		// Save image path
		sav.StorePascalString(_globals.TilesImagePath);

		// Save tiles order
		sav.Store32((uint) num_tiles);
		foreach (int index in tiles_order)
			sav.Store32((uint) index);

		// Save index of empty
		sav.Store32((uint) empty);
		sav.Store32((uint) empty_id);

		// Save number of moves so far
		sav.Store64((uint) moves);

		// All done
		sav.Close();
    }
}
