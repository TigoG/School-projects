using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System;
using System.Collections.Generic;
using System.Text;
class BlockPiece
{
    public Texture2D BlockTexture;
    public Vector2 location;
    public Color BlockColor;

    public void Initialize()
    {
        BlockTexture = TetrisGame.ContentManager.Load<Texture2D>("block");
        location = new Vector2(0, 0);
    }
    public void Update()
    {
        
    }
    public void Draw()
    {
        SpriteBatch.Draw(BlockTexture, location, Color.Blue);
    }
}