#ifndef TILE_H
#define TILE_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QObject>
#include <QPixmap>
#include "animatepixmap.h"

class Tile: public QObject, public QGraphicsItemGroup {
    Q_OBJECT
public:
    // constructor
    Tile(int color_code);

    void setNum(QPoint ipt);
    void setKind(QString type = "");
    void setRemind(bool ipt);
    void focus(bool ipt);
    QPointF getPicPos();

    // mouse event
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void editEndEnvent();

    bool firstMove;

signals:
    void pressed(QPoint pos);

private:
    QPoint pos;
    QColor color;
    QGraphicsRectItem *rect;
    QGraphicsRectItem *outline;
    QPropertyAnimation *animate;
    AnimatePixmap *pic;
    static const int width = 70;
    static const int length = 70;

};

#endif // TILE_H
