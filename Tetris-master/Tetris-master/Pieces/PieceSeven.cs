using System;
using System.Collections.Generic;
using System.Text;

namespace Tetris.Content.Pieces
{
    class PieceSeven : Tetrisblock
    {
        bool[,] pieceseven =
       {
            { true, true, true, false },
            { false, true, false, false },
            { false, false, false, false },
            { false, false, false, false },
        };
    }
}
