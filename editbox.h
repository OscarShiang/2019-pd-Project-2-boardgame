#ifndef EDITBOX_H
#define EDITBOX_H

#include <QObject>
#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include "animate.h"
#include "button.h"

class EditBox: public QObject, public QGraphicsItemGroup {
    Q_OBJECT
public:
    EditBox();
    Button *play, *del, *change;

    void buttonPos(QString pos);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

public slots:
    void changeColor();
    void addPawn();
    void addRock();
    void addKnight();
    void addBishop();
    void addKing();
    void addQueen();

    void chessDel(QString type);
    void checkForReady();

signals:
    void remove();
    void addChess(QString type);
    void readyToStart();

private:
    QGraphicsPixmapItem *box;
    Animate *pawn, *rock, *bishop, *knight, *king, *queen;
    QString color;

    QPoint num_pawn, num_rock, num_bishop, num_knight, num_king, num_queen;
};

#endif // EDITBOX_H
