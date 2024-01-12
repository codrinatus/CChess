#include "ClientHandler.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <unistd.h>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QGraphicsOpacityEffect>
#include "chesslabel.h"


MainWindow::MainWindow(QWidget *parent, ClientHandler *clientHandler)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    client(clientHandler)
{
    ui->setupUi(this);
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);
    stackedWidget->addWidget(ui->mainWidget);
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &MainWindow::on_exitButton_released);




    QWidget *chessboardPage = new QWidget(this);
    QGridLayout *chessboardLayout = new QGridLayout(chessboardPage);
    chessboardPage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    chessboardPage->setMinimumSize(600,600);
    chessboardPage->setMaximumSize(600,600);

    int scale = qMin(chessboardPage->width() / 8, chessboardPage->height() / 8) - 10;
    QPixmap wpawn(":/pieces/pieces/white-pawn.png");
    QPixmap swPawn = wpawn.scaled(scale,scale);
    QPixmap wrook(":/pieces/pieces/white-rook.png");
    QPixmap swrook = wrook.scaled(scale,scale);
    QPixmap wkn(":/pieces/pieces/white-knight.png");
    QPixmap swkn = wkn.scaled(scale,scale);
    QPixmap wbis(":/pieces/pieces/white-bishop.png");
    QPixmap swbis = wbis.scaled(scale,scale);
    QPixmap wk(":/pieces/pieces/white-king.png");
    QPixmap swk = wk.scaled(scale,scale);
    QPixmap wq(":/pieces/pieces/white-queen.png");
    QPixmap swq = wq.scaled(scale,scale);

    QPixmap bpawn(":/pieces/pieces/black-pawn.png");
    QPixmap sbpawn = bpawn.scaled(scale,scale);
    QPixmap brook(":/pieces/pieces/black-rook.png");
    QPixmap sbrook = brook.scaled(scale,scale);
    QPixmap bkn(":/pieces/pieces/black-knight.png");
    QPixmap sbkn = bkn.scaled(scale,scale);
    QPixmap bbis(":/pieces/pieces/black-bishop.png");
    QPixmap sbbis = bbis.scaled(scale,scale);
    QPixmap bk(":/pieces/pieces/black-king.png");
    QPixmap sbk = bk.scaled(scale,scale);
    QPixmap bq(":/pieces/pieces/black-queen.png");
    QPixmap sbq = bq.scaled(scale,scale);



    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessLabel *label = new ChessLabel(this,row,col);
            label->setAlignment(Qt::AlignCenter);
            if(row == 1)
                label->setPixmap(swPawn);
            else if(row == 0){
                if(col == 0 || col == 7)
                    label->setPixmap(swrook);
                else if(col == 1 || col == 6)
                    label->setPixmap(swkn);
                else if(col == 2 || col == 5)
                    label->setPixmap(swbis);
                else if(col == 3)
                    label->setPixmap(swk);
                else if(col == 4)
                    label->setPixmap(swq);
            }
            if(row == 6)
                label->setPixmap(sbpawn);
            else if(row == 7)
            {
                if(col == 0 || col == 7)
                    label->setPixmap(sbrook);
                else if(col == 1 || col == 6)
                    label->setPixmap(sbkn);
                else if(col == 2 || col == 5)
                    label->setPixmap(sbbis);
                else if(col == 3)
                    label->setPixmap(sbk);
                else if(col == 4)
                    label->setPixmap(sbq);
            }


            if ((row + col) % 2 == 0) {
                label->setStyleSheet("background-color: white;");
            } else {
                label->setStyleSheet("background-color: rgb(145, 65, 172);");
            }

            connect(label, &ChessLabel::pieceClicked, this, &MainWindow::onChessPieceClicked);
            chessboardLayout->addWidget(label, row, col);



        }
    }
    chessboardPage->setLayout(chessboardLayout);

    ui->mainWidget->addWidget(chessboardPage);


    QWidget *chessboardPageW = new QWidget(this);
    QGridLayout *chessboardLayoutW = new QGridLayout(chessboardPageW);
    chessboardPageW->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    chessboardPageW->setMinimumSize(600,600);
    chessboardPageW->setMaximumSize(600,600);

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            ChessLabel *label = new ChessLabel(this,row,col);
            label->setAlignment(Qt::AlignCenter);
            if(row == 1)
                label->setPixmap(sbpawn);
            else if(row == 0){
                if(col == 0 || col == 7)
                    label->setPixmap(sbrook);
                else if(col == 1 || col == 6)
                    label->setPixmap(sbkn);
                else if(col == 2 || col == 5)
                    label->setPixmap(sbbis);
                else if(col == 4)
                    label->setPixmap(sbk);
                else if(col == 3)
                    label->setPixmap(sbq);
            }
            if(row == 6)
                label->setPixmap(swPawn);
            else if(row == 7)
            {
                if(col == 0 || col == 7)
                    label->setPixmap(swrook);
                else if(col == 1 || col == 6)
                    label->setPixmap(swkn);
                else if(col == 2 || col == 5)
                    label->setPixmap(swbis);
                else if(col == 4)
                    label->setPixmap(swk);
                else if(col == 3)
                    label->setPixmap(swq);
            }


            if ((row + col) % 2 == 0) {
                label->setStyleSheet("background-color: white;");
            } else {
                label->setStyleSheet("background-color: rgb(145, 65, 172);");
            }

            connect(label, &ChessLabel::pieceClicked, this, &MainWindow::onChessPieceClicked);
            chessLabels[row][col] = label;
            chessboardLayoutW->addWidget(label, row, col);



        }
    }
    chessboardPageW->setLayout(chessboardLayoutW);

    ui->mainWidget->addWidget(chessboardPageW);







}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_exitButton_released()
{
    qDebug() << "exit button clicked";
    client ->sendMessage("exit");
    qDebug() << "sendMessage(exit) called";
    close();
    exit(0);
    //ui->mainWidget->setCurrentIndex(2);

}

void MainWindow::boardGenerator(char color[]){
    if(strcmp(color,"black") == 0){
        ui->mainWidget->setCurrentIndex(2);
        white = false;
    }
    else ui->mainWidget->setCurrentIndex(3);
    // client->receiveMessage(msg);
    // if(strcmp(msg,"turn") == 0)
    //     turn = true;
    // else if(strcmp(msg,"asteapta") == 0)
    //     client->receiveMessage(msg);






}


void MainWindow::on_playButton_released()
{
    qDebug() << "play button clicked";
    client ->sendMessage("play");
    qDebug() << "sendMessage(play) called";
    bzero(msg,100);
    client ->receiveMessage(msg);
    qDebug() << msg;

    if(strcmp(msg,"Asteapta un alt jucator") == 0){
        client ->receiveMessage(msg);
    }
    ui->playMessage->setText(msg);
    ui->mainWidget->setCurrentIndex(1);
    bzero(msg,100);
    client ->receiveMessage(msg);
    boardGenerator(msg);

    //if(white==false)




}

void MainWindow::onChessPieceClicked(ChessLabel *clickedLabel)
    {

    // if (turn == true)
    //     {
        if(selected==false){
            original = clickedLabel->pixmap();
            selected = true;
            old = clickedLabel;
            qDebug() << "Rand: "<< clickedLabel ->getRow() << " Coloana: " <<clickedLabel ->getCol();
            qDebug() << clickedLabel->getMove();
        }
        else{
            char move[10];
            if(white == true)
            {
            strcpy(move,old->getMove());
            strcat(move,clickedLabel->getMove());
            }
            else
            {
            strcpy(move,old->getInvMove());
            strcat(move,clickedLabel->getInvMove());
            }
            qDebug() << move;
            client->sendMessage("miscare");
            client->sendMessage(move);
            bzero(msg,100);
            client->receiveMessage(msg);
            qDebug() << msg;
            if(strcmp(msg,"valid")==0)
            {
            clickedLabel->setPixmap(original);
            old-> setPixmap(QPixmap());
            selected = false;
            }
            else
            {
                selected = false;
                qDebug() << "Miscare invalida";
            }
        }
    //     client->receiveMessage(msg);
    //     if(strcmp(msg,"turn") == 0)
    //         turn = true;
    //     else if(strcmp(msg,"asteapta") == 0)
    //         client->receiveMessage(msg);

    // }

  }






