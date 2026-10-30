using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;


class Tetrisblock
{
    Vector2 location;
    BlockPiece blockpiece;
    bool[,] Nextpiece = new bool[4, 4];


    public void Initialize()
    {
               
    }

    public Tetrisblock()
    {
        blockpiece = TetrisGame.ContentManager.Load<Texture2D>("block");
        
    }

    
    public void Update()
    {
       
    }
    public void Draw(GameTime gameTime, SpriteBatch spriteBatch)
    {
     //  for(int i = 0; i < 4; i++)
     //   {
     //       for(int j = 0; j<4; j++)
     //       {
     //           if (Nextpiece[i, j] == true)
     //           {
     //               spriteBatch.Draw(blockpiece, new Vector2(30 * j, 30 * i), Color.Blue);
     //           }
     //       }
     //   } 

        if(Nextpiece = PieceOne)
    }
}

