#ifndef SERVER_ROOK_H
#define SERVER_ROOK_H

#include "Piece.h"

class Rook : public Piece{
    using Piece::Piece;

public:
    bool checkMove(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]);

};


#endif //SERVER_ROOK_H
