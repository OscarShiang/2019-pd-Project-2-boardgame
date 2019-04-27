#ifndef ANIMATE_H
#define ANIMATE_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPropertyAnimation>

class Animate: public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    Q_PROPERTY(QPointF loc READ loc WRITE setLoc)
public:
    void setKind(QString type);
    void setLoc(QPointF pos);
    QPointF loc();

public slots:
    void finish();

private:
    QPixmap src;
    QPointF location;
    const int width = 70;
    const int length = 70;
};

#endif // ANIMATE_H
