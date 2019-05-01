#include "animaterect.h"
#include <QPen>
#include <QBrush>
#include <QEventLoop>
#include <QDebug>

animateRect::animateRect(int width, int height) {
    // set the appearance of rect
    setRect(0, 0, width, height);
    setPen(QPen(Qt::transparent));
    setBrush(Qt::white);
    setOpacity(0.6);

    // create the animation object
    anime = new QPropertyAnimation(this, "alpha");
}

void animateRect::fadeIn() {
    show();
    anime->setDuration(250);
    anime->setStartValue(0);
    anime->setKeyValueAt(0.3, 0.3);
    anime->setEndValue(0.6);
    anime->start();
}

void animateRect::fadeOut() {
    anime->setDuration(250);
    anime->setStartValue(0.6);
    anime->setKeyValueAt(0.3, 0.3);
    anime->setEndValue(0);
    anime->start();
}
