using Engine;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System;
using System.Collections.Generic;
using System.IO;

partial class Level : GameObjectList
{
    public const int TileWidth = 72;
    public const int TileHeight = 55;

    Tile[,] tiles;
    List<WaterDrop> waterDrops;

    public Player Player { get; private set; }
    public int LevelIndex { get; private set; }

    SpriteGameObject goal;
    BombTimer timer;

    bool completionDetected;


    GameObjectList fixedItems;

    public Level(int levelIndex, string filename)
    {
        LevelIndex = levelIndex;

        // load the background
        GameObjectList backgrounds = new GameObjectList();
        SpriteGameObject backgroundSky = new SpriteGameObject("Sprites/Backgrounds/spr_sky", TickTick.Depth_Background);
        backgroundSky.LocalPosition = new Vector2(0, 825 - backgroundSky.Height);
        backgrounds.AddChild(backgroundSky);

        AddChild(backgrounds);

        // load the rest of the level
        fixedItems = new GameObjectList();
        // add the timer
        timer = new BombTimer();
        fixedItems.AddChild(timer);


        LoadLevelFromFile(filename);

        // add mountains in the background
        for (int i = 0; i < 4; i++)
        {
            //  SpriteGameObject mountain = new SpriteGameObject(
            //      "Sprites/Backgrounds/spr_mountain_" + (ExtendedGame.Random.Next(2) + 1),
            //      TickTick.Depth_Background + 0.01f * (float)ExtendedGame.Random.NextDouble());

            //  mountain.LocalPosition = new Vector2(mountain.Width * (i-1) * 0.4f, 
            //      BoundingBox.Height - mountain.Height);

            float newDepth = TickTick.Depth_Background + 0.01f * (float)ExtendedGame.Random.NextDouble();
            Mountain mountain = new Mountain(newDepth, i, BoundingBox.Height);

            backgrounds.AddChild(mountain);
        }

        // add clouds
        for (int i = 0; i < 6; i++)
            backgrounds.AddChild(new Cloud(this));
    }

    public Rectangle BoundingBox
    {
        get
        {
            return new Rectangle(0, 0,
                tiles.GetLength(0) * TileWidth,
                tiles.GetLength(1) * TileHeight);
        }
    }

    public BombTimer Timer { get { return timer; } }

    public Vector2 GetCellPosition(int x, int y)
    {
        return new Vector2(x * TileWidth, y * TileHeight);
    }

    public Point GetTileCoordinates(Vector2 position)
    {
        return new Point((int)Math.Floor(position.X / TileWidth), (int)Math.Floor(position.Y / TileHeight));
    }

    public Tile.Type GetTileType(int x, int y)
    {
        // If the x-coordinate is out of range, treat the coordinates as a wall tile.
        // This will prevent the character from walking outside the level.
        if (x < 0 || x >= tiles.GetLength(0))
            return Tile.Type.Wall;

        // If the y-coordinate is out of range, treat the coordinates as an empty tile.
        // This will allow the character to still make a full jump near the top of the level.
        if (y < 0 || y >= tiles.GetLength(1))
            return Tile.Type.Empty;

        return tiles[x, y].TileType;
    }

    public Tile.SurfaceType GetSurfaceType(int x, int y)
    {
        // If the tile with these coordinates doesn't exist, return the normal surface type.
        if (x < 0 || x >= tiles.GetLength(0) || y < 0 || y >= tiles.GetLength(1))
            return Tile.SurfaceType.Normal;

        // Otherwise, return the actual surface type of the tile.
        return tiles[x, y].Surface;
    }

    public override void Update(GameTime gameTime)
    {
        base.Update(gameTime);

        // Update the fixed objects
        fixedItems.Update(gameTime);


        // check if we've finished the level
        if (!completionDetected && AllDropsCollected && Player.HasPixelPreciseCollision(goal))
        {
            completionDetected = true;
            ExtendedGameWithLevels.GetPlayingState().LevelCompleted(LevelIndex);
            Player.Celebrate();

            // stop the timer
            timer.Running = false;
        }

        // check if the timer has passed
        else if (Player.IsAlive && timer.HasPassed)
        {
            Player.Explode();
        }
    }

    /// <summary>
    /// Checks and returns whether the player has collected all water drops in this level.
    /// </summary>
    bool AllDropsCollected
    {
        get
        {
            foreach (WaterDrop drop in waterDrops)
                if (drop.Visible)
                    return false;
            return true;
        }
    }

    public override void Reset()
    {
        base.Reset();
        completionDetected = false;
    }


    public override void DrawFixed(GameTime gameTime, SpriteBatch spriteBatch)
    {
        if (!Visible) return;    
        fixedItems.Draw(gameTime, spriteBatch);
    }

    public override void DrawMoving(GameTime gameTime, SpriteBatch spriteBatch)
    {
        Draw(gameTime, spriteBatch);
    }

    void LoadLevelFromFile(string filename)
    {
        // open the file
        StreamReader reader = new StreamReader(filename);

        // read the description
        string description = reader.ReadLine();

        string timeOfLevel = reader.ReadLine();
        timer.TotalTime = double.Parse(timeOfLevel);
        timer.Reset();

        // read the rows of the grid; keep track of the longest row
        int gridWidth = 0;

        List<string> gridRows = new List<string>();
        string line = reader.ReadLine();
        while (line != null)
        {
            if (line.Length > gridWidth)
                gridWidth = line.Length;

            gridRows.Add(line);
            line = reader.ReadLine();
        }

        // stop reading the file
        reader.Close();

        // create all game objects for the grid
        AddPlayingField(gridRows, gridWidth, gridRows.Count);

        // add game objects to show that general level info
        AddLevelInfoObjects(description);
    }

    void AddLevelInfoObjects(string description)
    {
        // - background box
        SpriteGameObject frame = new SpriteGameObject("Sprites/UI/spr_frame_hint", TickTick.Depth_UIBackground);
        frame.SetOriginToCenter();
        frame.LocalPosition = new Vector2(720, 50);
        fixedItems.AddChild(frame);

        // - text
        TextGameObject hintText = new TextGameObject("Fonts/HintFont", TickTick.Depth_UIForeground, Color.Black, TextGameObject.Alignment.Left);
        hintText.Text = description;
        hintText.LocalPosition = new Vector2(510, 40);
        fixedItems.AddChild(hintText);
    }

    void AddPlayingField(List<string> gridRows, int gridWidth, int gridHeight)
    {
        // create a parent object for everything
        GameObjectList playingField = new GameObjectList();

        // initialize the list of water drops
        waterDrops = new List<WaterDrop>();

        // prepare the grid arrays
        tiles = new Tile[gridWidth, gridHeight];

        // load the tiles
        for (int y = 0; y < gridHeight; y++)
        {
            string row = gridRows[y];
            for (int x = 0; x < gridWidth; x++)
            {
                // the row could be too short; if so, pretend there is an empty tile
                char symbol = '.';
                if (x < row.Length)
                    symbol = row[x];

                // load the tile
                AddTile(x, y, symbol);
            }
        }
    }

    void AddTile(int x, int y, char symbol)
    {
        // load the static part of the tile
        Tile tile = CharToStaticTile(symbol);
        tile.LocalPosition = GetCellPosition(x, y);
        AddChild(tile);

        // store a reference to that tile in the grid
        tiles[x, y] = tile;

        // load the dynamic part of the tile
        if (symbol == '1')
            LoadCharacter(x, y);
        else if (symbol == 'X')
            LoadGoal(x, y);
        else if (symbol == 'W')
            LoadWaterDrop(x, y);
        else if (symbol == 'R')
            LoadRocketEnemy(x, y);
        else if (symbol == 'T')
            LoadTurtleEnemy(x, y);
        else if (symbol == 'S')
            LoadSparkyEnemy(x, y);
        else if (symbol == 'A' || symbol == 'B' || symbol == 'C')
            LoadFlameEnemy(x, y, symbol);
    }

    Tile CharToStaticTile(char symbol)
    {
        switch (symbol)
        {
            case '-':
                return new Tile(Tile.Type.Platform, Tile.SurfaceType.Normal);
            case '#':
                return new Tile(Tile.Type.Wall, Tile.SurfaceType.Normal);
            case 'h':
                return new Tile(Tile.Type.Platform, Tile.SurfaceType.Hot);
            case 'H':
                return new Tile(Tile.Type.Wall, Tile.SurfaceType.Hot);
            case 'i':
                return new Tile(Tile.Type.Platform, Tile.SurfaceType.Ice);
            case 'I':
                return new Tile(Tile.Type.Wall, Tile.SurfaceType.Ice);
            default:
                return new Tile(Tile.Type.Empty, Tile.SurfaceType.Normal);
        }
    }

    void LoadCharacter(int x, int y)
    {
        // create the bomb character
        Player = new Player(this, GetCellBottomCenter(x, y));
        AddChild(Player);
    }

    void LoadGoal(int x, int y)
    {
        // create the exit object
        goal = new SpriteGameObject("Sprites/LevelObjects/spr_goal", TickTick.Depth_LevelObjects);
        // make sure it's standing exactly on the tile below
        goal.LocalPosition = GetCellPosition(x, y + 1);
        goal.Origin = new Vector2(0, goal.Height);
        AddChild(goal);
    }

    void LoadWaterDrop(int x, int y)
    {
        // create the water drop object;  place it around the center of the tile
        Vector2 pos = GetCellPosition(x, y) + new Vector2(TileWidth / 2, TileHeight / 3);
        WaterDrop w = new WaterDrop(this, pos);
        // add it to the game world
        AddChild(w);
        // store an extra reference to it
        waterDrops.Add(w);
    }

    void LoadRocketEnemy(int x, int y)
    {
        Rocket r = new Rocket(this, GetCellPosition(x, y), x != 0);
        AddChild(r);
    }

    void LoadTurtleEnemy(int x, int y)
    {
        Turtle t = new Turtle(this);
        t.LocalPosition = GetCellBottomCenter(x, y);
        AddChild(t);
    }

    void LoadSparkyEnemy(int x, int y)
    {
        Sparky s = new Sparky(this, GetCellBottomCenter(x, y));
        AddChild(s);
    }

    void LoadFlameEnemy(int x, int y, char symbol)
    {
        Vector2 pos = GetCellBottomCenter(x, y);

        PatrollingEnemy enemy;
        if (symbol == 'A')
            enemy = new PatrollingEnemy(this, pos);
        else if (symbol == 'B')
            enemy = new PlayerFollowingEnemy(this, pos);
        else
            enemy = new UnpredictableEnemy(this, pos);

        AddChild(enemy);
    }

    Vector2 GetCellBottomCenter(int x, int y)
    {
        return GetCellPosition(x, y + 1) + new Vector2(TileWidth / 2, 0);
    }

}

