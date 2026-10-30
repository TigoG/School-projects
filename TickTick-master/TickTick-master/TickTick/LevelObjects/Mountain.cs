using Engine;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System;
using System.Collections.Generic;
using System.Text;


    class Mountain : SpriteGameObject
    {
        Level level;

        double depth;
    int boundingboxheight;
        int pos;

    public Mountain(float newdDepth, int newpos, int newboundingboxheight) : base("Sprites/Backgrounds/spr_mountain_" + (ExtendedGame.Random.Next(2) + 1),

                              newdDepth)
    {
        pos = newpos;
        depth = newdDepth;
        boundingboxheight = newboundingboxheight;
    }

        private Vector2 CalculatePosition()
        {
            double adjustedDepth = depth * 10000;

            float fraction =(float) adjustedDepth / 100f;

            return new Vector2(Width * (pos - 1) * 0.4f -(Camera.Position.X  * fraction), boundingboxheight - Height);
        }


      

    public override void Draw(GameTime gameTime, SpriteBatch spriteBatch)
    {
        LocalPosition = CalculatePosition();
        base.Draw(gameTime, spriteBatch);
      
    }



}
