#ifndef SERVER_KNIGHT_H
#define SERVER_KNIGHT_H

#include "Piece.h"

class Knight : public Piece{
    using Piece::Piece;

public:

    bool checkMove(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]);

};


#endif //SERVER_KNIGHT_H
