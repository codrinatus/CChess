#include "King.h"

bool King::checkMove(int xfrom, int yfrom, int xto, int yto, Piece *board[8][8]) {
    cout<<"king check\n";
    if (board[xto][yto] != NULL) {
        cout<<"pozitie ocupata\n";
        cout << board[xto][yto]->getColor() << " " << board[xfrom][yfrom]->getColor() << "\n";
        if (board[xto][yto]->getColor() == board[xfrom][yfrom]->getColor())
            return false;
    }
    if (abs(yfrom - yto) <= 1 && abs(xfrom - xto) <= 1)
        return true;
    // fara verificare de sah
    // facem verificare dupa mutare si daca e sah dam revert si zicem invalid

    return false;
}