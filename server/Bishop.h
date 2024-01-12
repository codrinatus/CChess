#ifndef SERVER_BISHOP_H
#define SERVER_BISHOP_H

#include "Piece.h"


class Bishop: public Piece{
    using Piece::Piece;
public:

    bool checkMove(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]);
};


#endif //SERVER_BISHOP_H
