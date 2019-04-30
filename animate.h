#ifndef ANIMATE_H
#define ANIMATE_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPropertyAnimation>
#include <QGraphicsSceneMouseEvent>

class Animate: public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    Animate(QString type = "");

    void setKind(QString type);
    QPointF getPicSize();

    QPixmap src;
    const int length = 70;
    const QPoint lengthF = QPoint(70, 70);
    qreal width();
    qreal height();

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void clicked();

public slots:
    void finish();
};

#endif // ANIMATE_H
