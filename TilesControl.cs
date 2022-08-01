#pragma warning disable RCS1213, IDE0051, RCS1110, RCS1146, IDE0044, RCS1169, IDE0052, RCS1085

using Godot;
using System;
using System.Collections.Generic;

public partial class TilesControl : Control
{
	// For save files, version of file format.
	public const int FILE_VERSION = 1;

	// Index within tiles of the empty tile.
	private int _empty;

	// Index value of the empty tile (always the last global tile index,
	// i.e. `_numTiles0` below).
	private int _emptyId;

	// When false, no movement is allowed.
	private bool _movesEnabled;
	public bool Movable {
		get { return _movesEnabled; }
		set {
			if (_movesEnabled != value)
				_movesEnabled = value;
		}
	}

	// Columns in tiles.
	private int _columns = 4;
	private int _columns0 = 3;
	[Export]
	public int Columns {
		get { return _columns; }
		set {
			if (value < 3 || value > 99)
				throw new ArgumentOutOfRangeException("Columns must be between 3 and 99.");
			_columns = value;
			_columns0 = value - 1;
			_numTiles = value * _rows;
			_numTiles0 = _numTiles - 1;
			_tilesReady = false;
		}
	}

	// Rows in tiles.
	private int _rows = 4;
	private int _rows0 = 3;
	[Export]
	public int Rows {
		get { return _rows; }
		set {
			if (value < 3 || value > 99)
				throw new ArgumentOutOfRangeException("Rows must be between 3 and 99.");
			_rows = value;
			_rows0 = value - 1;
			_numTiles = value * _columns;
			_numTiles0 = _numTiles - 1;
			_tilesReady = false;
		}
	}

	// Total number of tiles.
	private int _numTiles = 16;
	private int _numTiles0 = 15;
	public int TilesCount {
		get { return _numTiles; }
	}

	// Spacing between each tile.
	private Vector2 _spacing = new Vector2(5, 5);
	[Export]
	public Vector2 Spacing {
		get { return _spacing; }
		set { _spacing = value; }
	}

	// Font for drawing numerical id's of tiles.
	private Font _numberFont;
	[Export]
	public Font NumberFont {
		get { return _numberFont; }
		set { _numberFont = value; }
	}

	// Outline color for non-image tiles.
	private Color _outlineColor = Colors.Black;
	[Export]
	public Color OutlineColor {
		get { return _outlineColor; }
		set { _outlineColor = value; }
	}

	// Color of font for tile numbers.
	private Color _numberColor = Colors.White;
	[Export]
	public Color NumberColor {
		get { return _numberColor; }
		set { _numberColor = value; }
	}

	// Whn true, add numbers to all tiles (incuding images).
	private bool _showNumbers;
	[Export]
	public bool ShowNumbers {
		get { return _showNumbers; }
		set {
			if (_showNumbers != value)
			{
				_showNumbers = value;
				Update();
			}
		}
	}

	// Whn true, add outline to all tiles (incuding images).
	private bool _showOutlines;
	[Export]
	public bool ShowOutlines {
		get { return _showOutlines; }
		set {
			if (_showOutlines != value)
			{
				_showOutlines = value;
				Update();
			}
		}
	}

	private bool UseImage {
		get {
			return !string.IsNullOrEmpty(_imagePath);
		}
	}

	// Image used for tiles.
	private Image _image;

	// Path to image file.
	private string _imagePath;
	[Export]
	public string ImagePath {
		get { return _imagePath; }
		set {
			if (string.IsNullOrEmpty(value))
			{
				_imagePath = null;
				_image = null;
			}
			else
			{
				_imagePath = value;
				_image = LoadImage(_imagePath);
			}
		}
	}

	// `_tilesImage` converted to texture.  Only valid when `_tilesReady` is true.
	private ImageTexture _tilesTexture;

	// Size of each tile in pixels.  Only valid when `_tilesReady` is true.
	private Vector2 _tileSize;

	// Data for each tile.  Each tile is a two item Rect array.
	private Rect2[][] _tiles;

	// Index within `_tiles`[...][] of Rect2 of tile on `CanvasItem`.
	private const int IDX_DEST = 0;

	// Index within `_tiles`[...][] of Rect2 of tile on `_tilesTexture`.
	private const int IDX_SRC = 1;

	// When true, `_tiles`, `_tilesTexture`, and `_tileSize` are valid.
    private bool _tilesReady = false;

	// Actual positions of each tile (i.e. index 0 = tile at upper left).
	// Columns before rows (i.e. index 0 = row 0/col 0, index 1 = row 0/col 1,
	// index 2 = row 0/col 2, index 3 = row 1/col 0, etc. if `Columns` is 3).
	private int[] _tilesOrder;

	// When true, `_moveDownIndex` holds index within `_tiles` that can move down.
	private bool _canMoveDown;
	public bool CanMoveDown {
		get { return _canMoveDown; }
	}

	// When true, `_moveLeftIndex` holds index within `_tiles` than can move left.
	private bool _canMoveLeft;
	public bool CanMoveLeft {
		get { return _canMoveLeft; }
	}

	// When true, `_moveRightIndex` holds index within `_tiles` than can move right.
	private bool _canMoveRight;
	public bool CanMoveRight {
		get { return _canMoveRight; }
	}

	// When true, `_moveUpIndex` holds index within `_tiles` than can move up.
	private bool _canMoveUp;
	public bool CanMoveUp {
		get { return _canMoveUp; }
	}

	// Index within `_tilesOrder` that can move down. Valid only when `_canMoveDown`
	// is true.
	int _moveDownIndex;

	// Index within `_tiles_order` that can moves left. Valid only when `_canMoveLeft`
	// is true.
	int _moveLeftIndex;

	// Index within `_tiles_order` that can move right. Valid only when `_canMoveRight`
	// is true.
	int _moveRightIndex;

	// Index within `_tiles_order` that can move up. Valid only when `_canMoveUp`
	// is true.
	int _moveUpIndex;

	// Number of movements made since initial display.
	int _moves = 0;

	// Signalled when a tile is moved. Throttled to 1/50 second.
	[Signal]
	public delegate void moved(int count);

	// When > 0, a move has occurred.  This hrottles the `moved` signal.
	int _movedSignal = 0;

	// This is signalled when the tiles are all in correct order.
	[Signal]
	public delegate void won();

	// When true internal variables are initialized and ready to use.
    bool _readyToRun = false;

	// Time since last EmitSignal for moves.
	float _lastSignal = 0f;

	// Time since last CheckComplete().
	float _lastWinner = 0f;

	SceneTree _tree;

    public override void _Draw()
    {
        if (!_readyToRun || !_tilesReady)
            return;
        Vector2 extent;
        int index = 0;
        int tileIndex;
        for (int i = 0; i < _numTiles; ++i)
        {
            tileIndex = _tilesOrder[i];
            if (tileIndex != _emptyId)
            {
                if (UseImage)
                {
                    DrawTextureRectRegion(_tilesTexture, _tiles[index][IDX_DEST],
                                            _tiles[_tilesOrder[index]][IDX_SRC]);
                    if (_showNumbers)
                    {
                        string name = (tileIndex + 1).ToString();
                        extent = _numberFont.GetStringSize(name);
                        DrawString(_numberFont, new Vector2(_tiles[index][IDX_DEST].Position.x + 5,
                                _tiles[index][IDX_DEST].Position.y + 10 + (extent.y / 2)),
                                name, HorizontalAlignment.Left, -1, 24, _numberColor);
                    }

					if (_showOutlines)
					{
	                    Rect2[] tile = _tiles[index];
	                    Rect2 area = tile[IDX_DEST];
						Vector2 v = area.Position;
						--v.x;
						++v.y;
						area.Position = v;
						v = area.Size;
						++v.x;
						++v.y;
						area.Size = v;
	                    DrawRect(area, _outlineColor, false);
					}
                }
                else
                {
                    Rect2[] tile = _tiles[index];
                    Rect2 area = tile[IDX_DEST];
                    string name = (tileIndex + 1).ToString();
                    extent = _numberFont.GetStringSize(name);
                    DrawRect(area, _outlineColor, false);
                    DrawString(_numberFont, new Vector2(area.Position.x + ((area.Size.x - extent.x) / 2),
                            area.Position.y + ((area.Size.y - extent.y) / 2) + extent.y),
                            name, HorizontalAlignment.Left, -1, 24, _numberColor);
                }
            }
            ++index;
        }
    }

    public override void _Input(InputEvent ev)
    {
		// Do nothing while paused or uninitialized
        if (!_readyToRun || !_tilesReady || _tree.Paused || !_movesEnabled)
            return;

		// Only handle mouse clicks here
		if (!(ev is InputEventMouseButton))
			return;
		InputEventMouseButton iemb = ev as InputEventMouseButton;

		// Only handle left mouse clicks
		if (iemb.ButtonIndex != MouseButton.Left)
			return;

		// Don't pay attention to double clicks
		if (iemb.DoubleClick)
			return;

		// Only repond to clicks when released
		if (iemb.Pressed)
			return;

		// Adjust position of click to local coordinates
		InputEventMouseButton evmb = MakeInputLocal(iemb) as InputEventMouseButton;

		// Take appropriate action if needed
		if (_canMoveDown)
		{
			if (_tiles[_moveDownIndex][IDX_DEST].HasPoint(evmb.Position))
			{
				AcceptEvent();
				MoveDown();
				return;
			}
		}
		if (_canMoveLeft)
		{
			if (_tiles[_moveLeftIndex][IDX_DEST].HasPoint(evmb.Position))
			{
				AcceptEvent();
				MoveLeft();
				return;
			}
		}
		if (_canMoveRight)
		{
			if (_tiles[_moveRightIndex][IDX_DEST].HasPoint(evmb.Position))
			{
				AcceptEvent();
				MoveRight();
				return;
			}
		}
		if (_canMoveUp)
		{
			if (_tiles[_moveUpIndex][IDX_DEST].HasPoint(evmb.Position))
			{
				AcceptEvent();
				MoveUp();
				return;
			}
		}
    }

    public override void _PhysicsProcess(float delta)
    {
        if (!_readyToRun)
            return;

		// Update tiles if needed.
		if (!_tilesReady)
			RecalcTiles();

		// Emit moved signal if needed @ 1/50th second.
		if (_movedSignal > 0)
		{
			if (_lastSignal + delta > 0.02f)
			{
				int m = _moves;
				_movedSignal = 0;
				EmitSignal("moved", m);
				_lastSignal = 0f;
			}
			else
			{
				_lastSignal += delta;
			}
		}

		// Limit winning checks to 1/20 second.
		if (_lastWinner + delta > 0.05f)
		{
			CheckComplete();
			_lastWinner = 0f;
		}
		else
		{
			_lastWinner += delta;
		}
    }

    public override void _Ready()
    {
		base._Ready();
		_tree = GetTree();

		_readyToRun = false;
		_movesEnabled = false;
    }

    public override void _UnhandledInput(InputEvent ev)
    {
        if (!_readyToRun || !_tilesReady || _tree.Paused || !_movesEnabled
				|| !ev.IsActionType())
		{
			return;
		}

		// Take appropriate action
        if (_canMoveLeft)
		{
    		if (ev.IsActionPressed("move_left"))
			{
				AcceptEvent();
				MoveLeft();
				return;
			}
		}
        if (_canMoveRight)
		{
    		if (ev.IsActionPressed("move_right"))
			{
				AcceptEvent();
				MoveRight();
				return;
			}
		}
        if (_canMoveUp)
		{
    		if (ev.IsActionPressed("move_up"))
			{
				AcceptEvent();
				MoveUp();
				return;
			}
		}
        if (_canMoveDown)
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
        if (!_readyToRun)
            return;

		// Row and column of blank tile
		int row = _empty / _columns;
		int column = _empty % _columns;

		// Determine tiles that can move horizontally
		bool left_side = (column == 0);
		bool right_side = (column == _columns0);
		bool inside_h = (column > 0 && column < _columns0);
		if (left_side)
		{
			_canMoveLeft = true;
			_canMoveRight = false;
		}
		else if (right_side)
		{
			_canMoveLeft = false;
			_canMoveRight = true;
		}
        else if (inside_h)
        {
            _canMoveLeft = true;
			_canMoveRight = true;
		}
		if (_canMoveLeft)
			_moveLeftIndex = (row * _columns) + column + 1;
		if (_canMoveRight)
			_moveRightIndex = (row * _columns) + column - 1;

		// Determine tiles that can move vertically
		bool top_side = (row == 0);
		bool bottom_side = (row == _rows0);
		bool inside_v = (row > 0 && row < _rows0);
		if (top_side)
		{
			_canMoveUp = true;
			_canMoveDown = false;
		}
		else if (bottom_side)
		{
			_canMoveUp = false;
			_canMoveDown = true;
		}
		else if (inside_v)
		{
			_canMoveUp = true;
			_canMoveDown = true;
		}
		if (_canMoveUp)
			_moveUpIndex = ((row + 1) * _columns) + column;
		if (_canMoveDown)
			_moveDownIndex = ((row - 1) * _columns) + column;
    }

    private bool CheckComplete()
    {
#if TEST_WIN
		if (_moves < 10)	// For testing, this forces win after 10 moves
		{
#endif
			for(int index = 0; index < _numTiles; ++index)
			{
				if (_tilesOrder[index] != index)
					return false;
			}
#if TEST_WIN
		}
#endif
		_movesEnabled = false;
		EmitSignal("won");
		return true;
    }

    public void LoadGame(string path)
    {
		_movesEnabled = false;

		File inp = new File();
		inp.Open(path, File.ModeFlags.Read);

		// Read file format version.
		int v = (int) inp.Get16();
		if (v != FILE_VERSION)
		{
			inp.Close();
			throw new FormatException($"Uknown Save File Version: {v}");
		}

		// Load columns and rows.
		int co = inp.Get8();
		int ro = inp.Get8();

		// Load UseImage.
		byte tmp = inp.Get8();
		bool useImage = tmp != 0;

		// Load image path.
		string imagePath = inp.GetPascalString();

		// Load tiles order.
		int numTiles = (int) inp.Get16();
		int[] order = new int[numTiles];
		for (int i = 0; i < numTiles; ++i)
			order[i] = (int) inp.Get16();

		// Read blank tile index.
		int e = (int) inp.Get16();
		int eid = (int) inp.Get16();

		// Read moves so far
		int m = (int) inp.Get32();

		tmp = inp.Get8();
		bool showNumbers = tmp != 0;

		// Done loading data
		inp.Close();

		if (useImage)
		{
			_imagePath = imagePath;
			_image = LoadImage(_imagePath);
		}
		else
		{
			_imagePath = null;
			_image = null;
		}
		Columns = co;
		Rows = ro;
		_tilesOrder = order;
		_moves = m;
		_empty = e;
		_emptyId = eid;
		_showNumbers = showNumbers;
		++_movedSignal;

		// Reset tiles
		CallDeferred("RecalcTiles");
		CallDeferred("CalcMovables");

		// All ready
		_readyToRun = true;
		_movesEnabled = true;
		_tilesReady = false;
		CallDeferred("update");
    }

    private Image LoadImage(string path)
    {
        if (path.Substr(0, 4) == "res:")
        {
            return ResourceLoader.Load<Image>(path, "Image", ResourceLoader.CacheMode.Ignore);
        }
        else
        {
            Image i = new Image();
            i.Load(path);
            return i;
        }
    }

    private void MoveDown()
    {
		int newEmpty = _tilesOrder[_moveDownIndex];
		_tilesOrder[_moveDownIndex] = _emptyId;
		_tilesOrder[_empty] = newEmpty;
		_empty = _moveDownIndex;
		++_moves;
		++_movedSignal;
		CalcMovables();
		Update();
    }

    private void MoveLeft()
    {
		int newEmpty = _tilesOrder[_moveLeftIndex];
		_tilesOrder[_moveLeftIndex] = _emptyId;
		_tilesOrder[_empty] = newEmpty;
		_empty = _moveLeftIndex;
		++_moves;
		++_movedSignal;
		CalcMovables();
		Update();
    }

    private void MoveRight()
    {
		int newEmpty = _tilesOrder[_moveRightIndex];
		_tilesOrder[_moveRightIndex] = _emptyId;
		_tilesOrder[_empty] = newEmpty;
		_empty = _moveRightIndex;
		++_moves;
		++_movedSignal;
		CalcMovables();
		Update();
    }

    private void MoveUp()
    {
		int newEmpty = _tilesOrder[_moveUpIndex];
		_tilesOrder[_moveUpIndex] = _emptyId;
		_tilesOrder[_empty] = newEmpty;
		_empty = _moveUpIndex;
		++_moves;
		++_movedSignal;
		CalcMovables();
		Update();
    }

    public void RecalcTiles()
    {
        if (!_readyToRun)
            return;

		if (_columns == 0)
			throw new ArgumentOutOfRangeException("Columns has to be between 3 and 99");
		if (_rows == 0)
			throw new ArgumentOutOfRangeException("Rows has to be between 3 and 99");

        // Stop processing that relies on tiles being set.
        _tilesReady = false;

		// Reload and resize if size changed
		if (UseImage)
		{
			if (_image == null || Size.x != _image.GetWidth() || Size.y != _image.GetHeight())
			{
				_image = LoadImage(_imagePath);
				_image.Resize((int) Size.x, (int) Size.y);
			}
			_tilesTexture = ImageTexture.CreateFromImage(_image);
		}

		// Determine width and height of tiles from our size.
		_tileSize = new Vector2((Size.x - (_columns0 * _spacing.x)) / _columns,
								(Size.y - (_rows0 * _spacing.y)) / _rows);
		if (_tileSize.x <= 0 || _tileSize.y <= 0)
		{
            // If this happens, its a fluke at startup and this will get called again
            // which will be succesful.  In Godot 4+, this doesn't happen.
            return;
		}

		// Calculate the bounding boxes for each tile for both display and image
		Vector2 tileSize = new Vector2((int) _tileSize.x, (int) _tileSize.y);
		_tiles = new Rect2[_numTiles][];
		Rect2[] tile;
		for (int row = 0; row < _rows; ++row)
		{
			for (int col = 0; col < _columns; ++col)
			{
				tile = new Rect2[2];
				tile[IDX_DEST] = new Rect2(new Vector2((col * _tileSize.x) + (col * _spacing.x),
						(row * _tileSize.y) + (row * _spacing.y)), tileSize);
				tile[IDX_SRC] = new Rect2(new Vector2(col * _tileSize.x, row * _tileSize.y),
						tileSize);
				_tiles[(row * _columns) + col ] = tile;
			}
		}
        _tilesReady = true;
    }

    private void ResetTiles()
    {
		// Initial blank tile is last tile.
		_empty = _numTiles0;
		_emptyId = _empty;

		// Need some random numbers.
		RandomNumberGenerator rng = new RandomNumberGenerator();
		rng.Randomize();

		// Determine tile order.
		_tilesOrder = new int[_numTiles];
		for (int i = 0; i < _numTiles; ++i)
			_tilesOrder[i] = 9999;
		int order;
		for(int i = 0; i < _numTiles; ++i)
		{
			if (i == _empty)
			{
				_tilesOrder[i] = _emptyId;
			}
			else
			{
				order = rng.RandiRange(0, _numTiles0 - 1);
				if (i > 0)
				{
					while (Array.IndexOf(_tilesOrder, order) != -1)
						order = rng.RandiRange(0, _numTiles0 - 1);
				}
				else
				{
					if (order == _empty)
					{
						while (order == _empty)
							order = rng.RandiRange(0, _numTiles0 - 1);
					}
				}
				_tilesOrder[i] = order;
			}
		}

		// Initialized, but queue movement and tiles calc.
        _readyToRun = true;
		_movesEnabled = true;
        CallDeferred("RecalcTiles");
        CallDeferred("CalcMovables");
    }

    public void SaveGame(string path)
    {
        if (!_readyToRun)
            return;

		File sav = new File();
		sav.Open(path, File.ModeFlags.Write);

		// Save file version
		sav.Store16(FILE_VERSION);

		// Save TilesSize
		sav.Store8((byte) _columns);
		sav.Store8((byte) _rows);

		// Save UseImage state
		if (UseImage)
			sav.Store8(1);
		else
			sav.Store8(0);

		// Save image path
		sav.StorePascalString(_imagePath);

		// Save tiles order
		sav.Store16((ushort) _numTiles);
		foreach (int index in _tilesOrder)
			sav.Store16((ushort) index);

		// Save index of empty.
		sav.Store16((ushort) _empty);
		sav.Store16((ushort) _emptyId);

		// Save number of moves so far.
		sav.Store32((uint) _moves);

		// Save show numbers.
		if (_showNumbers)
			sav.Store8(1);
		else
			sav.Store8(0);

		// All done
		sav.Close();
    }

	// Called to begin game.
	public void Start()
	{
		ResetTiles();
	}
}
