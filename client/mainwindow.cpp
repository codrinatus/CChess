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
    QTimer *timer = new QTimer(this);
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &MainWindow::on_exitButton_released);
    connect(timer, &QTimer::timeout, this, &MainWindow::handleTimer);
    timer->start(1000);
    ui->mainWidget->installEventFilter(this);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleTimer(){
    if(time1 == true){
        client->receiveMessage(msg,4);
        qDebug () << msg;
        if(white)
            MoveConverterW(msg);
        else MoveConverterB(msg);
        qDebug () << move.fromX << move.fromY << move.toX << move.toY;
        qDebug()<< chessLabels[move.toX][move.toY]->getRow() << chessLabels[move.toX][move.toY]->getCol();
        chessLabels[move.toX][move.toY]->setPixmap(chessLabels[move.fromX][move.fromY]->pixmap());
        chessLabels[move.fromX][move.fromY]->setPixmap(QPixmap());
        time1=false;
        qDebug () << "timer turn: " << turn;
        turn=true;
        ui->mainWidget->currentWidget()->setDisabled(false);

    }
    if(time2 == true){


    }

}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->mainWidget && event->type() == QEvent::MouseButtonPress)
    {
        if (!turn)
        {
            qDebug() << "Not your turn! Click ignored.";
            return true;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}


void MainWindow::BlackBoard(){
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
            chessboardLayout->addWidget(label, row, col);



        }
    }
    chessboardPage->setLayout(chessboardLayout);

    ui->mainWidget->addWidget(chessboardPage);

}

void MainWindow::WhiteBoard(){
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
            chessLabels[row][col] = label;
            chessboardLayout->addWidget(label, row, col);



        }
    }
    chessboardPage->setLayout(chessboardLayout);

    ui->mainWidget->addWidget(chessboardPage);

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

void MainWindow::MoveConverterB(const char* buf){
    move.fromX = 7 - (buf[0] - '0');
    move.fromY = 7 - (buf[1] - '0');
    move.toX = 7 - (buf[2] - '0');
    move.toY = 7 - (buf[3] - '0');
}

void MainWindow::MoveConverterW(const char* buf){
    move.fromX =  (buf[0] - '0');
    move.fromY =  (buf[1] - '0');
    move.toX =  (buf[2] - '0');
    move.toY =  (buf[3] - '0');
}

void MainWindow::boardGenerator(char color[]){
    if(strcmp(color,"black") == 0){
        WhiteBoard();
        white = false;
    }
    else BlackBoard();
    ui->mainWidget->setCurrentIndex(2);
    qDebug () << "boargen1";
    client->receiveMessage(msg);
    qDebug () << "boargen2: " << msg;
    if(strcmp(msg,"turn") == 0)
        turn = true;
    else if(strcmp(msg,"asteapta") == 0){
        ui->mainWidget->currentWidget()->setDisabled(true);
        time1=true;

        // client->receiveMessage(msg,4);
        // qDebug () << msg;
        // MoveConverter(msg);
        // qDebug () << move.fromX << move.fromY << move.toX << move.toY;
        // qDebug()<< chessLabels[move.toX][move.toY]->getRow() << chessLabels[move.toX][move.toY]->getCol();
        // chessLabels[move.toX][move.toY]->setPixmap(chessLabels[move.fromX][move.fromY]->pixmap());
        // chessLabels[move.fromX][move.fromY]->setPixmap(QPixmap());
        //ui->mainWidget->currentWidget()->update();

    }








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
        client ->receiveMessage(msg,1);
        qDebug() << "if asteaptaunaltjucator msg: " << msg;
    }
    //ui->playMessage->setText(msg);
    //ui->mainWidget->setCurrentIndex(1);
    bzero(msg,100);
    client ->receiveMessage(msg,5);
    qDebug () << "before boargen: " << msg;
    boardGenerator(msg);

    //if(white==false)




    }

void MainWindow::onChessPieceClicked(ChessLabel *clickedLabel)
{
    qDebug () << "chesspc turn1: " << turn;

     if (turn == true){
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
            ui->mainWidget->currentWidget()->setDisabled(true);
            turn=false;
            qDebug () << "chesspc turn2: " << turn;
            clickedLabel->setPixmap(original);
            old-> setPixmap(QPixmap());
            selected = false;
            time1=true;
            //client->receiveMessage(msg);

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
    //turn = false;

     }
     else{ qDebug () << "else ifturn";

     }


}





