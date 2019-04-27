#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QObject>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QList>
#include <QPropertyAnimation>
#include "tile.h"
#include "animate.h"

class Game: public QGraphicsView {
    Q_OBJECT
public:
    // constructor
    Game();

    QGraphicsScene *scene;
    Tile *tile[8][8];

    // game method
    void makeBoard();
    void putChess();
    void moveChess(QPoint init, QPoint final);
    void setRemind(bool ipt);
    void canMove(QPoint target);
    void testWalkOff(int x, int y, QString direction);
    bool isValid(QPoint pos);

public slots:
    void mousePressed(QPoint pos);
    void test();

private:
    const int width = 580;
    const int length = 580;

    // for chess manage
    QPoint focusChess;
    QString board[8][8];
    QList <QPoint> possibleList;

    const QPoint UNFOCUS = QPoint(-1, -1);

    // for animation
    Animate *anime;
    QPropertyAnimation *animation;
};

#endif // GAME_H
