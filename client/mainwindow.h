#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chesslabel.h"
#include "qstackedwidget.h"
#include <QMainWindow>

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

    void on_MainWindow_destroyed();
    void onChessPieceClicked(ChessLabel *clickedLabel);
    void boardGenerator(char color[]);
private:
    Ui::MainWindow *ui;
    ClientHandler *client;
    QStackedWidget *stackedWidget;
    char msg[500];
    char msg2[500];
    bool selected = false;
    QPixmap original;
    ChessLabel *old;
    bool white = true;
    ChessLabel *chessLabels[8][8];
    bool turn;



};

#endif // MAINWINDOW_H
