#include "Knight.h"

bool Knight::checkMove(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]) {
    if (((abs(yfrom - yto) == 1) && (abs(xfrom - xto) == 2)) ||
        ((abs(yfrom - yto) == 2) && (abs(xfrom - xto) == 1))) {
        if (board[xto][yto] == NULL)
            return true;
        else if (board[xto][yto]->getColor() != board[xfrom][yfrom]->getColor())
            return true;
    }
    return false;
}