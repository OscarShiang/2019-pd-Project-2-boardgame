#include "animatepixmap.h"
#include <QDebug>

AnimatePixmap::AnimatePixmap(QString type) {
    setKind(type);
}

void AnimatePixmap::setKind(QString type) {
    if (type == "") {
        hide();
        return;
    }

    QString piece;
    if (type[1] == "p")
        piece = "pawn";
    else if (type[1] == "r")
        piece = "rock";
    else if (type[1] == "n")
        piece = "knight";
    else if (type[1] == "b")
        piece = "bishop";
    else if (type[1] == "k")
        piece = "king";
    else if (type[1] == "q")
        piece = "queen";

    if (type[0] == "b")
        src.load(":/pic/black/" + piece +".svg");
    else if (type[0] == "w")
        src.load(":/pic/white/" + piece +".svg");

    src = src.scaledToHeight(45, Qt::FastTransformation);
    setPixmap(src);
    show();
}

void AnimatePixmap::finish() {
    hide();
}

QPointF AnimatePixmap::getPicSize() {
    return QPointF(src.width(), src.height());
}

void AnimatePixmap::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked();
}

qreal AnimatePixmap::width() {
    return src.width();
}

qreal AnimatePixmap::height() {
    return src.height();
}
