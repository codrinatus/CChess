#ifndef SERVER_QUEEN_H
#define SERVER_QUEEN_H

#include "Piece.h"

class Queen : public Piece {
    using Piece::Piece;

private:
    bool diagonally(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]);

    bool rowcol(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]);


public:

    bool checkMove(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]);
};


#endif //SERVER_QUEEN_H
