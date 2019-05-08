#ifndef CURSOR_H
#define CURSOR_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QTimer>

class Cursor: public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    Cursor(int font_size = 15);

public slots:
    void shine();
    void reset();
    void increase();
    void move(QString str, int direction);
    void setPosition(int input);
    int getPos();
    void pause();
    void start();

private:
    bool isShine;
    int pos;
    QTimer *timer;
};

#endif // CURSOR_H
