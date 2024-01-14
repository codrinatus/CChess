#include "Pawn.h"

bool Pawn::checkMove(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]) {

    cout << "Pawn check\n";
    if (color) {
        cout << "Culoare alba\n";
        cout << "Xfrom: " << xfrom << " Xto: " << xto << "\n";
        if (xfrom - xto == 1) {
            cout << "Pion alb a mutat pe linie\n";
            if (yfrom == yto) {
                cout << "Pionul se va muta in fata\n";
                if (board[xto][yto] == NULL) {
                    cout << "Pionul a avut spatiu sa se mute\n";
                    return true;
                }
            } else if ((abs(yfrom - yto) == 1) && board[xto][yto] != NULL)
                if (board[xto][yto]->getColor() != board[xfrom][yfrom]->getColor()) {
                    return true;
                }
        }
        return false;
    } else {
        cout << "negru\n";
        if (xto - xfrom == 1) {
            cout << "Pion negru a mutat pe linie\n";
            if (yfrom == yto) {
                if (board[xto][yto] == NULL)
                    return true;
            } else if ((abs(yfrom - yto) == 1) && board[xto][yto] != NULL)
                if (board[xto][yto]->getColor() != board[xfrom][yfrom]->getColor()) {
                    return true;
                }
        }
        return false;
    }
}