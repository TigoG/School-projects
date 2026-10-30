using System;
using System.Collections.Generic;
using System.Text;

namespace Tetris.Content.Pieces
{
    class PieceThree : Tetrisblock
    {
        bool[,] piecethree =
       {
            { false, true , true, false },
            { true, true, false, false },
            { false, false, false, false },
            { false, false, false, false },
        };
    }
}
