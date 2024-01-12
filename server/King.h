#ifndef SERVER_KING_H
#define SERVER_KING_H

#include "Piece.h"
class King : public Piece{
    using Piece::Piece;

public:

    bool checkMove(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]);

};


#endif //SERVER_KING_H
