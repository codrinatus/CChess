#ifndef CHESSROOM_H
#define CHESSROOM_H

#include <QWidget>

namespace Ui {
class ChessRoom;
}

class ChessRoom : public QWidget
{
    Q_OBJECT

public:
    explicit ChessRoom(QWidget *parent = nullptr);
    ~ChessRoom();

private:
    Ui::ChessRoom *ui;
};

#endif // CHESSROOM_H
