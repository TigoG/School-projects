using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Engine
{
    public static class Camera
    {

        static public Vector2 Position = new Vector2(0, 0);


        /// <summary>
        /// The width and height of the game world, in game units.
        /// </summary>
        public static Point WorldSize { get; set; }

        /// <summary>
        /// The width and height of the window, in pixels.
        /// We assume that camera size equals window size.
        /// </summary>
        public static Point WindowSize { get; set; }

        public static Point ScreenSize { get; set; }

     

        public static Tuple<Matrix, Matrix> GetTranslationMatrix(GraphicsDevice graphicsDevice) 
        {

            graphicsDevice.Viewport = CalculateViewport(ScreenSize);
        
            // calculate how the graphics should be scaled, so that the game world fits inside the window
            Matrix scaleMatrix = Matrix.CreateScale((float)graphicsDevice.Viewport.Width / WorldSize.X, (float)graphicsDevice.Viewport.Height / WorldSize.Y, 1);
            Matrix translationMatrix = Matrix.CreateTranslation(-(int)Position.X, -(int)Position.Y, 0);
            return new Tuple<Matrix, Matrix>(scaleMatrix, translationMatrix);        
        }


        public static void MoveCamera(Vector2 positionPlayer)
        {
            if (positionPlayer.X > WindowSize.X / 3 * 4)
            {
               
               Position.X = positionPlayer.X - (WindowSize.X / 3 * 4);
               
                // Check if we reached the end of the world. 
               if (Position.X > (WorldSize.X - WindowSize.X) - WindowSize.X / 4 + 40)
               {
                    Position.X = (WorldSize.X - WindowSize.X) - WindowSize.X / 4 + 40;
               }
            }
            else
            {
                Position.X = 0;
            }

            if (positionPlayer.Y > WindowSize.Y / 3 * 4)
            {

                Position.Y = positionPlayer.Y - (WindowSize.Y / 3 * 4);

                // Check if we reached the end of the world. 
                if (Position.Y > (WorldSize.Y - WindowSize.Y) - WindowSize.Y / 4 - 15)
                {
                    Position.Y = (WorldSize.Y - WindowSize.Y) - WindowSize.Y / 4 - 15;
                }
            }
            else
            {
                Position.Y = 0;
            }

        }

        /// <summary>
        /// Calculates and returns the viewport to use, so that the game world fits on the screen while preserving its aspect ratio.
        /// </summary>
        /// <param name="windowSize">The size of the screen on which the world should be drawn.</param>
        /// <returns>A Viewport object that will show the game world as large as possible while preserving its aspect ratio.</returns>
        static Viewport CalculateViewport(Point windowSize)
        {
            // create a Viewport object
            Viewport viewport = new Viewport();

            // calculate the two aspect ratios
            float gameAspectRatio = (float)WorldSize.X / WorldSize.Y;
            float windowAspectRatio = (float)windowSize.X / windowSize.Y;

            // if the window is relatively wide, use the full window height
            if (windowAspectRatio > gameAspectRatio)
            {
                viewport.Width = (int)(windowSize.Y * gameAspectRatio);
                viewport.Height = windowSize.Y;
            }
            // if the window is relatively high, use the full window width
            else
            {
                viewport.Width = windowSize.X;
                viewport.Height = (int)(windowSize.X / gameAspectRatio);
            }

            // calculate and store the top-left corner of the viewport
            viewport.X = (windowSize.X - viewport.Width) / 2;
            viewport.Y = (windowSize.Y - viewport.Height) / 2;

            return viewport;
        }

       

    }
}
