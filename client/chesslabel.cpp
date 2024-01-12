#include "chesslabel.h"

ChessLabel::ChessLabel(QWidget *parent, int row, int col)
    : QLabel(parent),row(row),col(col)
{
}

void ChessLabel::mousePressEvent(QMouseEvent *event)
{
    // Emit the signal when the label is clicked
    emit pieceClicked(this);
}

int ChessLabel::getCol() const
{
    return col;
}

int ChessLabel::getRow() const
{
    return row;
}

char* ChessLabel::getMove() const
{
    char* result = new char[5];
    sprintf(result,"%d%d",row,col);
    return result;
}

char* ChessLabel::getInvMove() const
{
    char* result = new char[5];
    sprintf(result,"%d%d",7-row,7-col);
    return result;
}

