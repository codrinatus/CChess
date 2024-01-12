#ifndef SERVER_PIECE_H
#define SERVER_PIECE_H


#include <iostream>
#include <cmath>
#include <typeinfo>
#include <cstring>

using namespace std;

class Piece {
public:
    virtual ~Piece();

protected:
    bool color;
public:


    Piece(bool color) : color(color) {}

    bool getColor() { return color; }

    void setColor(bool color) { this->color = color; }

    virtual bool checkMove(int xfrom, int yfrom, int xto, int yto, Piece* board[8][8]) = 0;
};


#endif //SERVER_PIECE_H
