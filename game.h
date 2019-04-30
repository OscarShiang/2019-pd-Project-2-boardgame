#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QObject>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QList>
#include <QSet>
#include <QPropertyAnimation>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include "tile.h"
#include "animate.h"
#include "button.h"
#include "editbox.h"

class Game: public QGraphicsView {
    Q_OBJECT
public:
    // constructor
    Game();

    QGraphicsScene *scene;
    Tile *tile[8][8];

    // method for general playing
    void makeBoard();
    void putChess();
    void cleanBoard();
    void moveChess(QPoint init, QPoint final);
    void setRemind(bool ipt);
    void canMove(QPoint target, QSet <QPoint> &possibleList);
    void testWalkOff(int x, int y, QString direction, QSet <QPoint> &possibleList);
    bool isValid(QPoint pos);
    void gameJudge();

    // for game pausing
    void keyPressEvent(QKeyEvent *event);

public slots:
    void mousePressed(QPoint pos);
    void displayMenu();
    void gameStart();
    void gameEdit();
    void gameOver();

    // method for editing board
    void editRemove();
    void editChess(QString type);

signals:
    void checkMate();
    void chessRemove(QString type);

private:
    const int width = 580;
    const int length = 580;

    QGraphicsTextItem *title;

    // for chess manage
    QString board[8][8];
    QPoint focusChess;
    QSet <QPoint> possibleList;
    QList <QPoint> castlingList; // index 0 for queen side; index 1 for king side

    const QPoint NONEXIST = QPoint(-1, -1);

    // for game judgement
    QString turn;
    QString whocheck;
    QString checkmate;
    bool check;

    // for edit mode
    EditBox *box;
    bool editMode;

    // for title
    QGraphicsRectItem *rect;
    Button *play;
    Button *edit;
    Button *quit;
    Button *again;
    Button *back;
    Button *resume;

    // for game pause
    bool pause;

    // for animation
    Animate *anime;
    QPropertyAnimation *animation;
};

#endif // GAME_H
