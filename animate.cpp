#include "animate.h"
#include <QDebug>

void Animate::setKind(QString type) {
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

void Animate::setLoc(QPointF pos) {
    setPos(10 + pos.y() * width + width / 2 - src.width() / 2, 10 + pos.x() * length + length / 2 - src.height() / 2);
}

QPointF Animate::loc() {
    return location;
}

void Animate::finish() {
//    hide();
}
