#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>

class Button: public QObject, public QGraphicsItemGroup {
    Q_OBJECT
public:
    Button(QString label_name, int width = 140, int height = 50, int size = 30);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    qreal width();
    qreal height();

signals:
    void clicked();

private:
    QGraphicsTextItem *text;
    QGraphicsRectItem *rect;
    int font_size = 30;
    const QString font_family = "Joystix";

};

#endif // BUTTON_H
