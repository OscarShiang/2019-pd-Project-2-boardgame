#include "animate.h"
#include <QDebug>

Animate::Animate(QString type) {
    setKind(type);
}

void Animate::setKind(QString type) {
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

void Animate::finish() {
    hide();
}

QPointF Animate::getPicSize() {
    return QPointF(src.width(), src.height());
}

void Animate::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked();
}

qreal Animate::width() {
    return src.width();
}

qreal Animate::height() {
    return src.height();
}
