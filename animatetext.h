#ifndef ANIMATETEXT_H
#define ANIMATETEXT_H

#include <QGraphicsTextItem>
#include <QObject>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

class AnimateText: public QGraphicsTextItem {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(qreal alpha READ opacity WRITE setOpacity)

public:
    AnimateText(QString text, int font_size);
    void slideIn();
    void slideOut();
    void setFontSize(int size_ipt);

private:
    const QString font_family = "Joystix";
    QPropertyAnimation *loc, *alpha;
    QParallelAnimationGroup *group;
};

#endif // ANIMATETEXT_H
