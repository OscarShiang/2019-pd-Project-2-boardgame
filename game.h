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
#include <cstdlib>
#include "tile.h"
#include "animatepixmap.h"
#include "button.h"
#include "editbox.h"
#include "animaterect.h"
#include "animatetext.h"
#include "radio.h"

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
    void gameSelect();
    void gameStart();
    void gameEdit();
    void gameOver();
    void gameOnline();
    void logIn(QString ipt);
    void analyseData(QString ipt);

    // method for editing board
    void editRemove();
    void editChess(QString type);

    // method for pause screen
    void gameResume();

signals:
    void checkMate();
    void chessRemove(QString type);

private:
    const int width = 580;
    const int length = 580;

    AnimateText *title;

    // for chess manage
    QString board[8][8];
    QPoint focusChess;
    QSet <QPoint> possibleList;
    QList <QPoint> castlingList; // index 0 for queen side; index 1 for king side

    const QPoint NONEXIST = QPoint(-1, -1);

    int mode, prevMode; // 1 for menu; 2 for mode select; 3 for edit; -1 for pause

    // for game judgement
    QString turn;
    QString whocheck;
    QString checkmate;
    bool check;

    // for edit mode
    EditBox *box;

    // for title
    AnimateRect *rect;
    Button *play;
    Button *edit;
    Button *quit;
    Button *again;
    Button *back;
    Button *resume;
    Button *online;
    Button *classic;

    // for internet connection
    Radio *radio;
    bool playOnline;

    // for animation
    AnimatePixmap *anime;
    QPropertyAnimation *animation;
};

namespace Mode {
    enum {Menu, Select, Play, Edit, Pause};
}

#endif // GAME_H
