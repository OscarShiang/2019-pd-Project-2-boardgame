#ifndef ANIMATE_H
#define ANIMATE_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPropertyAnimation>
#include <QGraphicsSceneMouseEvent>

class AnimatePixmap: public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    AnimatePixmap(QString type = "");

    void setKind(QString type);
    QPointF getPicSize();

    const int length = 70;
    const QPoint lengthF = QPoint(70, 70);
    QPixmap src;
    qreal width();
    qreal height();

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

signals:
    void clicked();

public slots:
    void finish();
};

#endif // ANIMATE_H
