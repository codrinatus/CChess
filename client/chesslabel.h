#ifndef CHESSLABEL_H
#define CHESSLABEL_H

#include <QLabel>
#include <QMouseEvent>

class ChessLabel : public QLabel
{
    Q_OBJECT

public:
    ChessLabel(QWidget *parent = nullptr, int row = -1, int col=-1);

    int getRow() const;

    int getCol() const;
    char* getMove() const;
    char* getInvMove() const;

signals:
    void pieceClicked(ChessLabel *label);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    int row;
    int col;
};


#endif // CHESSLABEL_H
