#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chesslabel.h"
#include "qstackedwidget.h"
#include <QMainWindow>
#include <QTimer>
#include <QProcess>

struct Pos{
    int fromX,fromY,toX,toY;
};

class ClientHandler; // Forward declaration

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, ClientHandler *clientHandler = nullptr);

    ~MainWindow();

private slots:


    void on_exitButton_released();

    void on_playButton_released();
    void handleTimer();
    void onChessPieceClicked(ChessLabel *clickedLabel);
    void boardGenerator(char color[]);
    void MoveConverterB(const char* msg);
    void MoveConverterW(const char* msg);
    void BlackBoard();
    void WhiteBoard();
    bool eventFilter(QObject *obj, QEvent *event);


    void on_return2button_released();

    void on_return1button_released();

private:
    Ui::MainWindow *ui;
    ClientHandler *client;
    QStackedWidget *stackedWidget;
    char msg[500];
    //char msg2[500];
    bool selected = false;
    QPixmap original;
    QPixmap pix;
    ChessLabel *old;
    bool white = true;
    ChessLabel *chessLabels[8][8];
    bool turn;
    Pos move;
    bool time1=false;
    bool time2=false;
    bool time3=false;
    bool time4=false;




};

#endif // MAINWINDOW_H
