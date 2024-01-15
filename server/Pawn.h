#ifndef SERVER_PAWN_H
#define SERVER_PAWN_H

#include "Piece.h"


class Pawn : public Piece {
    using Piece::Piece;
private:
    bool moved = false; //pentru miscare 2 casute la inceput, trebuie adaugat in check
public:

    bool checkMove(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]);
    };


#endif //SERVER_PAWN_H
