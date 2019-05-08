#include "cursor.h"
#include <QBrush>
#define switch_ms 470

Cursor::Cursor(int font_size) {
    // set size
    setRect(0, 0, int(font_size / 20), font_size);
    isShine = false;
    pos = 0;

    // set the color inside
    setBrush(Qt::black);

    // create and bind the timer with the index
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(shine()));
    timer->start(switch_ms);
}

void Cursor::shine() {
    setVisible(isShine);
    isShine = !isShine;
}

void Cursor::reset() {
    isShine = true;
}

void Cursor::increase() {
    pos ++;
}

void Cursor::move(QString str, int direction) {
    if (direction == 1 && pos < str.length())
        pos ++;
    else if (direction == -1 && pos > 0)
        pos --;
}

void Cursor::setPosition(int input) {
    pos = input;
}

int Cursor::getPos() {
    return pos;
}

void Cursor::pause() {
    timer->stop();
    hide();
}

void Cursor::start() {
    timer->start();
    show();
}
