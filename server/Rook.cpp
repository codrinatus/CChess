#include "Rook.h"

bool Rook::checkMove(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]) {
    if (board[xto][yto] != NULL)
        if (board[xto][yto]->getColor() == board[xfrom][yfrom]->getColor())
            return false;
    if(xfrom != xto && yfrom != yto){
        return false;
    }
    if (xfrom == xto) {
        if (yfrom < yto)
            for (int y = yfrom + 1; y < yto; y++) {
                if (board[xfrom][y] != NULL)
                    return false;
            }
        else
            for (int y = yfrom - 1; y > yto; y--) {
                if (board[xfrom][y] != NULL)
                    return false;
            }

    } else if (yfrom == yto) {
        if (xfrom < xto)
            for (int x = xfrom + 1; x < xto; x++) {
                if (board[x][yfrom] != NULL)
                    return false;
            }
        else
            for (int x = xfrom - 1; x > xto; x--) {
                if (board[x][yfrom] != NULL)
                    return false;
            }

    }
    return true;
}