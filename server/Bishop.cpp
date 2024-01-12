#include "Bishop.h"

bool Bishop::checkMove(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]) {
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