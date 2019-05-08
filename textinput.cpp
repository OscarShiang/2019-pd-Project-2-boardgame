#include "textinput.h"
#include <QFont>
#include <QFontMetrics>
#include <QDebug>

#define L -1 // left
#define R 1 // right

TextInput::TextInput(int width, int height, int size): font_size(size) {
    // set the height and width for the whole group
    boundingRect().setRect(0, 0, width, height);

    // create and add the textItem into the group
    text = new AnimateText("", font_size);
    addToGroup(text);

    // create and add the index into the group
    cursor = new Cursor(size);
    cursor->setY(cursor->y() + 7);
    update(font_size);
    addToGroup(cursor);
    cursor->hide();

    // set the limit of input
    textCount = 0;
    limit = 9999999;
}

void TextInput::keyPressEvent(QKeyEvent *event) {
    QString tmp = text->toPlainText();
    int pos = cursor->getPos();

    // to limit the users to enter between 0 to 9 and dot(.)
    if (textCount < limit && event->key() != Qt::Key_Backspace) {
        if (event->key() == Qt::Key_0) {
            text->setPlainText(tmp.left(pos) + "0" + tmp.right(tmp.length() - pos));
            cursor->increase();
            textCount ++;
        }
        else if (event->key() == Qt::Key_1) {
            text->setPlainText(tmp.left(pos) + "1" + tmp.right(tmp.length() - pos));
            cursor->increase();
            textCount ++;
        }
        else if (event->key() == Qt::Key_2) {
            text->setPlainText(tmp.left(pos) + "2" + tmp.right(tmp.length() - pos));
            cursor->increase();
            textCount ++;
        }
        else if (event->key() == Qt::Key_3) {
            text->setPlainText(tmp.left(pos) + "3" + tmp.right(tmp.length() - pos));
            cursor->increase();
            textCount ++;
        }
        else if (event->key() == Qt::Key_4) {
            text->setPlainText(tmp.left(pos) + "4" + tmp.right(tmp.length() - pos));
            cursor->increase();
            textCount ++;
        }
        else if (event->key() == Qt::Key_5) {
            text->setPlainText(tmp.left(pos) + "5" + tmp.right(tmp.length() - pos));
            cursor->increase();
            textCount ++;
        }
        else if (event->key() == Qt::Key_6) {
            text->setPlainText(tmp.left(pos) + "6" + tmp.right(tmp.length() - pos));
            cursor->increase();
            textCount ++;
        }
        else if (event->key() == Qt::Key_7) {
            text->setPlainText(tmp.left(pos) + "7" + tmp.right(tmp.length() - pos));
            cursor->increase();
            textCount ++;
        }
        else if (event->key() == Qt::Key_8) {
            text->setPlainText(tmp.left(pos) + "8" + tmp.right(tmp.length() - pos));
            cursor->increase();
            textCount ++;
        }
        else if (event->key() == Qt::Key_9) {
            text->setPlainText(tmp.left(pos) + "9" + tmp.right(tmp.length() - pos));
            cursor->increase();
            textCount ++;
        }
        else if (event->key() == Qt::Key_Period) {
            text->setPlainText(tmp.left(pos) + "." + tmp.right(tmp.length() - pos));
            cursor->increase();
            textCount ++;
        }
        cursor->reset();
        update(cursor->getPos());
    }
    // back space delete
    if (event->key() == Qt::Key_Backspace && pos > 0) {
        text->setPlainText(tmp.left(pos - 1) + (tmp.right(tmp.length() - pos)));
        cursor->move(text->toPlainText(), L);
        textCount --;
    }
    if (event->key() == Qt::Key_Left) {
        cursor->move(text->toPlainText(), L);
    }
    else if (event->key() == Qt::Key_Right) {
        cursor->move(text->toPlainText(), R);
    }
    else if (event->key() == Qt::Key_End) {
        cursor->setPosition(tmp.length());
    }
    else if (event->key() == Qt::Key_Home) {
        cursor->setPosition(0);
    }
    cursor->reset();
    update(cursor->getPos());
}

void TextInput::update(int pos) {
    QFontMetrics font(QFont(font_family, font_size));
    cursor->setX(font.width(text->toPlainText().left(pos)) + 4);
}

void TextInput::releaseFocus() {
    cursor->pause();
}

void TextInput::getFocus() {
    cursor->start();
}

void TextInput::setMaxText(int maxtext) {
    limit = maxtext;
}

QString TextInput::getContent() {
    QString content = text->toPlainText();
    text->setPlainText("");
    cursor->setPosition(0);
    update(cursor->getPos());
    textCount = 0;
    return content;
}
