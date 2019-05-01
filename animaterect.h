#ifndef ANIMATERECT_H
#define ANIMATERECT_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QPropertyAnimation>

class animateRect: public QObject, public QGraphicsRectItem {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal alpha READ opacity WRITE setOpacity)

public:
    // constructors
    animateRect(int width, int height);

    // animation functions
    void fadeIn();
    void fadeOut();

private:
    QPropertyAnimation *anime;
};

#endif // ANIMATERECT_H
