#include "Queen.h"

bool Queen::rowcol(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]) {
    if (board[xto][yto] != NULL)
        if (board[xto][yto]->getColor() == board[xfrom][yfrom]->getColor())
            return false;
    if (xfrom != xto && yfrom != yto) {
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

bool Queen::diagonally(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]) {
    if ((xfrom + yfrom == xto + yto) || (abs(xfrom - yfrom) == abs(xto - yto))) {
        int i, j;
        if (xfrom < xto)
            i = 1;
        else
            i = -1;
        if (yfrom < yto)
            j = 1;
        else
            j = -1;

        for (int k = 1; k <= abs(xto - xfrom); k++)
            if (board[xfrom + k * i][yfrom + k * j] != NULL) {
                if (board[xfrom + k * i][yfrom + k * j]->getColor() == board[xfrom][yfrom]->getColor())
                    return false;
                else {
                    if (xfrom + k * i == xto)
                        return true;
                    else return false;
                }
            }

        return true;
    }
    return false;
}

bool Queen::checkMove(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]) {
    return rowcol(xfrom, yfrom, xto, yto, board) || diagonally(xfrom, yfrom, xto, yto, board);

}

