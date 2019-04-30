#include "tile.h"
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QDebug>

Tile::Tile(int color_code) {
    firstMove = true;

    // turn the hover function to be active
    setAcceptHoverEvents(true);

    // set the rect size and the style
    rect = new QGraphicsRectItem();
    rect->setRect(0, 0, width, length);
    rect->setPen(QPen(Qt::transparent));

    // set the color by the given color_code
    if (color_code) // dark tile
        color.setRgb(132, 88, 75); // color.setRgb(151, 207, 144);
    else // light tile
        color.setRgb(239, 190, 134); // color.setRgb(252, 224, 164);
    rect->setBrush(color);

    // set the remind outline
    outline = new QGraphicsRectItem();
    outline->setRect(qreal(width / 2) - qreal(55 / 2), qreal(length / 2) - qreal(55 / 2), 55, 55);
    outline->setBrush(Qt::transparent);
    // draw the outline
    QPen pen;
    pen.setWidth(3);
    pen.setColor((color_code)? Qt::white : QColor(132, 88, 75));
    outline->setPen(pen);
    outline->hide();

    // create the pic object
    pic = new Animate();
    animate = new QPropertyAnimation(pic, "pos");

    // add the element into group
    addToGroup(rect);
    addToGroup(outline);
    addToGroup(pic);
}

void Tile::setNum(QPoint ipt) {
    pos = ipt;
}

void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // for test
    emit pressed(pos);
}

void Tile::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    rect->setBrush(QColor(255, 151, 31));
}

void Tile::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    rect->setBrush(color);
}

void Tile::setKind(QString type) {
    if (type == "")
        firstMove = false;
    pic->setKind(type);
    pic->setPos(width / 2 - qreal(pic->src.width() / 2), length / 2 - qreal(pic->src.height() / 2));
}

void Tile::setRemind(bool ipt) {
    if (ipt)
        outline->show();
    else {
        outline->hide();
    }
}

void Tile::focus(bool ipt) {
    animate->setDuration(150);
    animate->setStartValue(pic->pos());
    if (ipt) {
        animate->setEndValue(QPointF(width / 2 - qreal(pic->src.width() / 2), length / 2 - qreal(pic->src.height() / 2) - 5));
    }
    else {
        animate->setEndValue(QPointF(width / 2 - qreal(pic->src.width() / 2), length / 2 - qreal(pic->src.height() / 2) ));
    }
    animate->start();
}

QPointF Tile::getPicPos() {
    return pic->pos();
}

void Tile::editEndEnvent() {
    setAcceptHoverEvents(true);
    rect->setBrush(color);
}
