#include "animatetext.h"
#include <QFont>
#include <QPointF>

animateText::animateText(QString text, int font_size) {
    setFont(QFont(font_family, font_size));
    setPlainText(text);

    loc = new QPropertyAnimation(this, "pos");
    alpha = new QPropertyAnimation(this, "alpha");

    group = new QParallelAnimationGroup();

    group->addAnimation(loc);
    group->addAnimation(alpha);
}

void animateText::slideIn() {
    qreal x = this->x(), y = this->y();
    // set pos animation
    loc->setStartValue(QPointF(x, y - 5));
    loc->setEndValue(QPointF(x, y));
    loc->setDuration(500);

    // set alpha
    alpha->setStartValue(0);
    alpha->setEndValue(1);
    alpha->setDuration(500);

    group->start();
}

void animateText::slideOut() {
    qreal x = this->x(), y = this->y();
    // set pos animation
    loc->setStartValue(QPointF(x, y));
    loc->setEndValue(QPointF(x, y - 5));
    loc->setDuration(500);

    // set alpha
    alpha->setStartValue(1);
    alpha->setEndValue(0);
    alpha->setDuration(500);

    group->start();
}
