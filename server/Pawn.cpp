#include "Pawn.h"

bool Pawn::checkMove(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]) {

    if (color) {
        if (!moved)
            if (xfrom - xto == 2)
                if (yfrom == yto && board[xto][yto] == NULL) {
                    moved = true;
                    return true;
                }
        if (xfrom - xto == 1) {
            if (yfrom == yto) {
                if (board[xto][yto] == NULL) {
                    moved = true;
                    return true;
                }
            } else if ((abs(yfrom - yto) == 1) && board[xto][yto] != NULL)
                if (board[xto][yto]->getColor() != board[xfrom][yfrom]->getColor()) {
                    moved = true;
                    return true;
                }
        }
        return false;
    } else {
        if (!moved)
            if (xto - xfrom == 2)
                if (yfrom == yto && board[xto][yto] == NULL) {
                    moved = true;
                    return true;
                }
        if (xto - xfrom == 1) {
            if (yfrom == yto) {
                if (board[xto][yto] == NULL) {
                    moved = true;
                    return true;
                }
            } else if ((abs(yfrom - yto) == 1) && board[xto][yto] != NULL)
                if (board[xto][yto]->getColor() != board[xfrom][yfrom]->getColor()) {
                    moved = true;
                    return true;
                }
        }
        return false;
    }
}