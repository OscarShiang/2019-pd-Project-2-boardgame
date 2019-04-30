#include "editbox.h"
#include <QPixmap>
#include <QDebug>

EditBox::EditBox() {
    // to make the object in the group pressable
    setHandlesChildEvents(false);

    // set the initial color for the chess
    color = "b";

    // create the buttons
    box = new QGraphicsPixmapItem(QPixmap(":/pic/box.svg").scaledToHeight(105, Qt::FastTransformation));
    pawn = new Animate("bp");
    rock = new Animate("br");
    bishop = new Animate("bb");
    knight = new Animate("bn");
    king = new Animate("bk");
    queen = new Animate("bq");

    addToGroup(box);

    // initial the num of the chesses
    num_pawn = QPoint(8, 8);
    num_rock = QPoint(2, 2);
    num_bishop = QPoint(2, 2);
    num_knight = QPoint(2, 2);
    num_king = QPoint(1, 1);
    num_queen = QPoint(1, 1);

    // add the buttons into the group
    addToGroup(pawn);
    addToGroup(rock);
    addToGroup(knight);
    addToGroup(bishop);
    addToGroup(king);
    addToGroup(queen);

    // set the pos of the buttons
    pawn->setPos(30, 30);
    rock->setPos(92, 30);
    knight->setPos(164, 30);
    bishop->setPos(236, 30);
    king->setPos(308, 30);
    queen->setPos(375, 30);

    // connect the buttons with functions
    connect(pawn, SIGNAL(clicked()), this, SLOT(addPawn()));
    connect(rock, SIGNAL(clicked()), this, SLOT(addRock()));
    connect(knight, SIGNAL(clicked()), this, SLOT(addKnight()));
    connect(bishop, SIGNAL(clicked()), this, SLOT(addBishop()));
    connect(king, SIGNAL(clicked()), this, SLOT(addKing()));
    connect(queen, SIGNAL(clicked()), this, SLOT(addQueen()));


    change = new Button("color", 60, 30, 15);
    connect(change, SIGNAL(clicked()), this, SLOT(changeColor()));
    addToGroup(change);
    change->setPos(240, 110);

    play = new Button("play", 60, 30, 15);
    connect(play, SIGNAL(clicked()), this, SLOT(checkForReady()));
    addToGroup(play);
    play->setPos(380, 110);

    del = new Button("del", 60, 30, 15);
    connect(del, SIGNAL(clicked()), this, SIGNAL(remove()));
    addToGroup(del);
    del->setPos(310, 110);
}

void EditBox::changeColor() {
    if (color == "b")
        color = "w";
    else if (color == "w")
        color = "b";

    pawn->setKind(color + "p");
    rock->setKind(color + "r");
    bishop->setKind(color + "b");
    knight->setKind(color + "n");
    king->setKind(color + "k");
    queen->setKind(color + "q");
}

void EditBox::buttonPos(QString pos) {
    if (pos == "below") {
        play->setY(110);
        del->setY(110);
        change->setY(110);
    }
    else if (pos == "above") {
        play->setY(-35);
        del->setY(-35);
        change->setY(-35);
    }
}

void EditBox::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    return;
}

void EditBox::addPawn() {
    if (color == "b") {
        if (num_pawn.x() > 0) {
            num_pawn -= QPoint(1, 0);
            emit addChess(color + "p");
        }
    }
    else {
        if (num_pawn.y() > 0) {
            num_pawn -= QPoint(0, 1);
            emit addChess(color + "p");
        }
    }
}

void EditBox::addRock() {
    if (color == "b") {
        if (num_rock.x() > 0) {
            num_rock -= QPoint(1, 0);
            emit addChess(color + "r");
        }
    }
    else {
        if (num_rock.y() > 0) {
            num_rock -= QPoint(0, 1);
            emit addChess(color + "r");
        }
    }
}

void EditBox::addKnight() {
    if (color == "b") {
        if (num_knight.x() > 0) {
            num_knight -= QPoint(1, 0);
            emit addChess(color + "n");
        }
    }
    else {
        if (num_knight.y() > 0) {
            num_knight -= QPoint(0, 1);
            emit addChess(color + "n");
        }
    }
}

void EditBox::addBishop() {
    if (color == "b") {
        if (num_bishop.x() > 0) {
            num_bishop -= QPoint(1, 0);
            emit addChess(color + "b");
        }
    }
    else {
        if (num_bishop.y() > 0) {
            num_bishop -= QPoint(0, 1);
            emit addChess(color + "b");
        }
    }
}

void EditBox::addKing() {
    qDebug() << num_king;
    if (color == "b") {
        if (num_king.x() > 0) {
            num_king -= QPoint(1, 0);
            emit addChess(color + "k");
        }
    }
    else {
        if (num_king.y() > 0) {
            num_king -= QPoint(0, 1);
            emit addChess(color + "k");
        }
    }
}

void EditBox::addQueen() {
    if (color == "b") {
        if (num_queen.x() > 0) {
            num_queen -= QPoint(1, 0);
            emit addChess(color + "q");
        }
    }
    else {
        if (num_queen.y() > 0) {
            num_queen -= QPoint(0, 1);
            emit addChess(color + "q");
        }
    }
}

void EditBox::chessDel(QString type) {
    qDebug() << "the chess being deleted" << type;
    if (type[0] == "b") {
        if (type[1] == "p")
            num_pawn += QPoint(1, 0);
        else if (type[1] == "r")
            num_rock += QPoint(1, 0);
        else if (type[1] == "n")
            num_knight += QPoint(1, 0);
        else if (type[1] == "b")
            num_bishop += QPoint(1, 0);
        else if (type[1] == "k")
            num_king += QPoint(1, 0);
        else if (type[1] == "q")
            num_queen += QPoint(1, 0);
    }
    else {
        if (type[1] == "p")
            num_pawn += QPoint(0, 1);
        else if (type[1] == "r")
            num_rock += QPoint(0, 1);
        else if (type[1] == "n")
            num_knight += QPoint(0, 1);
        else if (type[1] == "b")
            num_bishop += QPoint(0, 1);
        else if (type[1] == "k")
            num_king += QPoint(0, 1);
        else if (type[1] == "q")
            num_queen += QPoint(0, 1);
    }
}

void EditBox::checkForReady() {
    QPoint sum = num_king + num_pawn + num_rock + num_queen + num_bishop + num_knight;

    if (num_king != QPoint(0, 0)) {
        qDebug() << "can play";
        emit readyToStart();
    }
    else if (sum.x() != 16 && sum.y() != 16) {
        emit readyToStart();
    }
}
