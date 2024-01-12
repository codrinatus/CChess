#include "Queen.h"

bool Queen::checkMove(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]) {
    if (board[xto][yto] != NULL)
        if (board[xto][yto]->getColor() == board[xfrom][yfrom]->getColor())
            return false;
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
            if (board[xfrom + k * i][yfrom + k * j] != NULL)
                return false;
        return true;
    }
    return false;
}