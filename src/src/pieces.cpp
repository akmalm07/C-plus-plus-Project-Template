#include "headers.h"
#include "pieces.h"


bool Piece::piece_exists(int index) const
{
    return boardSetup[index] != EMPTY;
}

// Impleent a stateless, function based chess design!
// Instead of having to see if the pawn moved, we cna simply check if the pieces is in its starting position
// This will make it easier to implement an AI later on