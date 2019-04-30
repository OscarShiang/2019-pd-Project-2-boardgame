#include "button.h"
#include <QPen>
#include <QBrush>
#include <QFont>

Button::Button(QString label_name, int width, int height, int size) {
    setAcceptHoverEvents(true);

    rect = new QGraphicsRectItem();
    rect->setRect(0, 0, width, height);
    font_size = size;

    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(height / 10);
    rect->setPen(pen);
    rect->setBrush(Qt::red);

    text = new QGraphicsTextItem();
    text->setFont(QFont(font_family, font_size));
    text->setPlainText(label_name);
    text->setDefaultTextColor(Qt::white);
    addToGroup(rect);
    addToGroup(text);

    text->setPos(rect->rect().width() / 2 - text->boundingRect().width() / 2, rect->rect().height() / 2 - text->boundingRect().height() / 2);

}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    rect->setBrush(Qt::darkRed);
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    rect->setBrush(Qt::red);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked();
}

qreal Button::width() {
    return rect->rect().width();
}

qreal Button::height() {
    return rect->rect().height();
}
