#include "chessroom.h"
#include "ui_chessroom.h"

ChessRoom::ChessRoom(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChessRoom)
{
    ui->setupUi(this);
}

ChessRoom::~ChessRoom()
{
    delete ui;
}
