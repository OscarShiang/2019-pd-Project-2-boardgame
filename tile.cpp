#include "tile.h"
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QDebug>

Tile::Tile(int color_code) {
    // turn the hover function to br active
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

    QPen pen;
    pen.setWidth(3);
    pen.setColor((color_code)? Qt::white : QColor(132, 88, 75));
    outline->setPen(pen);

    outline->hide();

    pic = new QGraphicsPixmapItem();

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
    rect->setBrush(QColor(255, 151, 31, 255));
}

void Tile::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    rect->setBrush(color);
}

void Tile::setKind(QString ipt) {
    kind = ipt[1];
    side = ipt[0];
    if (ipt == "") {
        pic->hide();
        return;
    }

    QPixmap src;
    QString piece;
    if (kind == "p")
        piece = "pawn";
    else if (kind == "r")
        piece = "rock";
    else if (kind == "n")
        piece = "knight";
    else if (kind == "b")
        piece = "bishop";
    else if (kind == "k")
        piece = "king";
    else if (kind == "q")
        piece = "queen";

    if (side == "b")
        src.load(":/pic/black/" + piece +".svg");
    else if (side == "w")
        src.load(":/pic/white/" + piece +".svg");

    pic->show();

    src = src.scaledToHeight(45, Qt::FastTransformation);
    pic->setPixmap(src);
    pic->setPos(width / 2 - qreal(src.width() / 2), length / 2 - qreal(src.height() / 2));
}

QPoint Tile::getPos() {
    return pos;
}

void Tile::setRemind(bool ipt) {
    if (ipt)
        outline->show();
    else {
        outline->hide();
    }
}
