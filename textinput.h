#ifndef TEXTINPUT_H
#define TEXTINPUT_H

#include <QObject>
#include <QGraphicsItemGroup>
#include <QKeyEvent>
#include <QString>
#include "cursor.h"
#include "animatetext.h"

class TextInput: public QObject, public QGraphicsItemGroup {
    Q_OBJECT
public:
    TextInput(int width, int height, int size);
    void keyPressEvent(QKeyEvent *event);
    void update(int pos);
    void releaseFocus();
    void getFocus();
    void setMaxText(int maxtext);
    QString getContent();

private:
    Cursor *cursor;
    AnimateText *text;
    const QString font_family = "Joystix";
    int font_size;
    int textCount;
    int limit;
};

#endif // TEXTINPUT_H
