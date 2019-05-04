#include "game.h"
#include <QGraphicsRectItem>
#include <QDebug>
#include <iostream>
#include <QEventLoop>
#include <QSet>
#include <QHash>
#include <QFontDatabase>
#include <QFontMetrics>

Game::Game() {
    // set the game window
    setFixedSize(width, length);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // create and set the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, width, length);
    setScene(scene);
    scene->setBackgroundBrush(QColor(58, 23, 17)); // (79, 46, 41)

    // create animation object
    anime = new Animate();
    anime->hide();
    animation = new QPropertyAnimation(anime, "pos");

    // create the title text item
    QFontDatabase::addApplicationFont(":/font/pixel.ttf");
    title = new animateText("test", 60);

    // create the edit mode objects
    editMode = false;

    // construct buttons
    rect = new animateRect(width, length);
    play = new Button("play");
    quit = new Button("quit");
    edit = new Button("edit");
    again = new Button("again");
    back = new Button("back");
    resume = new Button("resume", 140, 50, 25);

    // connect the function with the coresponding buttons
    connect(back, SIGNAL(clicked()), this, SLOT(displayMenu()));
    connect(resume, SIGNAL(clicked()), this, SLOT(gameResume()));
    connect(play, SIGNAL(clicked()), this, SLOT(gameStart()));
    connect(quit, SIGNAL(clicked()), this, SLOT(close()));
    connect(edit, SIGNAL(clicked()), this, SLOT(gameEdit()));
    connect(again, SIGNAL(clicked()), this ,SLOT(gameStart()));

    connect(this, SIGNAL(checkMate()), this, SLOT(gameOver()));

    makeBoard();

    // game pause switch
    menuShow = true;
    pause = false;

    // show the view window
    show();
}

void Game::makeBoard() {
    // add the  tile
    for (int i = 0; i < 8; i ++) {
        for (int j = 0; j < 8; j ++) {
            board[i][j] = "";
            tile[i][j] = new Tile((i + j) % 2);
            tile[i][j]->setPos(10 + 70 * j, 10 + 70 * i);
            tile[i][j]->setNum(QPoint(i, j));
            connect(tile[i][j], SIGNAL(pressed(QPoint)), this, SLOT(mousePressed(QPoint)));
            scene->addItem(tile[i][j]);
        }
    }

    // add the animation object
    scene->addItem(anime);

    // add the main menu object
    scene->addItem(rect);
    scene->addItem(play);
    scene->addItem(quit);
    scene->addItem(edit);
    scene->addItem(again);
    scene->addItem(back);
    scene->addItem(resume);
    scene->addItem(title);
}

void Game::putChess() {
    // set pawns
    for (int i = 0; i < 8; i ++) {
        board[1][i] = "bp";
        board[6][i] = "wp";
    }

    // set kings
    board[0][4] = "bk";
    board[7][4] = "wk";

    // set queens
    board[0][3] = "bq";
    board[7][3] = "wq";

    QString k[3] = {"r", "n", "b"};
    for (int i = 0; i < 3; i ++) {
        board[0][i] = "b" + k[i];
        board[7][i] = "w" + k[i];

        board[0][7 - i] = "b" + k[i];
        board[7][7 - i] = "w" + k[i];
    }

    for (int i = 0; i < 8; i ++) {
        for (int j = 0; j < 8; j ++) {
            tile[i][j]->setKind(board[i][j]);
            if (board[i][j] != "") {
                tile[i][j]->firstMove = true;
            }
        }
    }
}

void Game::mousePressed(QPoint pos) {
    if (pause || menuShow)
        return;
    if (!editMode) {
        qDebug() << pos << board[pos.x()][pos.y()];

        // set focus
        if (focusChess == NONEXIST) {
            // test if the tile has no chess
            if (board[pos.x()][pos.y()] == "")
                return;
            // chack if the right chess is being chosen
            if (turn != board[pos.x()][pos.y()][0])
                return ;
            // check if the chess cannot move
            canMove(pos, possibleList);
            if (possibleList.empty())
                return;

            focusChess = pos;
            tile[pos.x()][pos.y()]->focus(true);

            //        qDebug() << "START can move list";
            //        for (QSet <QPoint> ::iterator it = possibleList.begin(); it != possibleList.end(); it ++) {
            //            qDebug() << *it;
            //        }
            //        qDebug() << "END";

            setRemind(true);

            qDebug() << "set focus";
        }
        else if (focusChess == pos || !isValid(pos)) {
            tile[focusChess.x()][focusChess.y()]->focus(false);
            setRemind(false);
            focusChess = NONEXIST;
            possibleList.clear();
            qDebug() << "release focus";
        }
        else if (isValid(pos)) {
            qDebug() << "move chess";
            tile[focusChess.x()][focusChess.y()]->focus(false);
            moveChess(focusChess, pos);
            possibleList.clear();
            focusChess = NONEXIST;

            // change the right
            if (turn == "b")
                turn = "w";
            else if (turn == "w")
                turn = "b";

            qDebug() << "release focus";
        }
    }
    else {
        qDebug() << "here";
        if (focusChess == NONEXIST) {
            focusChess = pos;
            rect->fadeIn();
            box->show();

            box->setZValue(1);
            tile[pos.x()][pos.y()]->setZValue(2);
            tile[pos.x()][pos.y()]->setAcceptHoverEvents(false);

            if (pos.x() < 4) {
                box->buttonPos("below");
                box->setPos(10 + 15 * pos.y(), 15 + (pos.x() + 1) * 70);
            }
            else {
                box->buttonPos("above");
                box->setPos(10 + 15 * pos.y(), (pos.x() - 1.5) * 70);
            }
            qDebug() << "focus";
        }
        else {
            tile[focusChess.x()][focusChess.y()]->setZValue(0);
            tile[focusChess.x()][focusChess.y()]->editEndEnvent();
            focusChess = NONEXIST;
            box->hide();
            rect->fadeOut();
            qDebug() << "unfocus";
        }
    }
}

void Game::moveChess(QPoint init, QPoint final) {
    // if the king is being killed
    if (board[final.x()][final.y()][1] == "k") {
        checkmate = board[init.x()][init.y()][0];
        qDebug() << "the" << checkmate << "get checkmate";
    }

    // implement move or kill on actual board variables
    board[final.x()][final.y()] = board[init.x()][init.y()];
    board[init.x()][init.y()] = "";

    // animation start
    setRemind(false);
    anime->setKind(board[final.x()][final.y()]);
    tile[init.x()][init.y()]->setKind(board[init.x()][init.y()]);

    animation->setStartValue(QPointF(10 + init.y() * 70 + 70 / 2, 10 + init.x() * 70 + 70 / 2 - 5) - anime->getPicSize() / 2);
    animation->setDuration(300);
    animation->setEndValue(QPointF(10 + final.y() * 70 + 70 / 2, 10 + final.x() * 70 + 70 / 2) - anime->getPicSize() / 2);

    QEventLoop loop; // to stop the execute
    connect(animation, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(animation, SIGNAL(finished()), anime, SLOT(finish()));

    animation->start();
    loop.exec(); // run the loop

    // promotion: set the pawn to queen
    if (board[final.x()][final.y()][1] == "p" && final.x() == (board[final.x()][final.y()][0] == "b" ? 7 : 0)) {
        qDebug() << "promotion";
        board[final.x()][final.y()] = board[final.x()][final.y()][0] + "q";
    }

    // castling: king castle transposition
    if (board[final.x()][final.y()][1] == "k") {
        // kingside castling
        if (castlingList[1] != NONEXIST) {
            qDebug() << "castling";
            board[final.x()][final.y() - 1] = board[final.x()][final.y() + 1];
            board[final.x()][final.y() + 1] = "";
            tile[final.x()][final.y() - 1]->setKind(board[final.x()][final.y() - 1]);
            tile[final.x()][final.y() + 1]->setKind(board[final.x()][final.y() + 1]);
        }
        // queenside castling
        else if (castlingList[0] != NONEXIST) {
            qDebug() << "castling";
            board[final.x()][final.y() + 1] = board[final.x()][final.y() - 2];
            board[final.x()][final.y() - 2] = "";
            tile[final.x()][final.y() + 1]->setKind(board[final.x()][final.y() + 1]);
            tile[final.x()][final.y() - 2]->setKind(board[final.x()][final.y() - 2]);
        }
    }

    tile[final.x()][final.y()]->setKind(board[final.x()][final.y()]);

    for (int i = 0; i < 8; i ++)
        qDebug("%2s %2s %2s %2s %2s %2s %2s %2s", board[i][0].toStdString().c_str(), board[i][1].toStdString().c_str(), board[i][2].toStdString().c_str(), board[i][3].toStdString().c_str(), board[i][4].toStdString().c_str(), board[i][5].toStdString().c_str(), board[i][6].toStdString().c_str(), board[i][7].toStdString().c_str());

    if (checkmate != "") {
        emit checkMate();
    }
    gameJudge();
}

void Game::canMove(QPoint target, QSet <QPoint> &possibleList) {
    int x = target.x(), y = target.y();

    if (board[x][y][1] == "p") {
        // rule of pawn
        for (int i = 1; i <= 2; i ++) {
            // move
            if (board[x - (board[x][y][0] == "b"? -i : i)][y] == "") {
                possibleList += QPoint(x - (board[x][y][0] == "b"? -i : i), y);
            }
            else {
                break;
            }
            if (x != 1 && x != 6)
                break;
        }
        // kill
        for (int i = -1; i <= 1; i += 2) {
            if (x - 1 >= 0 && x - 1 < 8 && y + i >= 0 && y + i < 8) {
                // if is enemy
                if (board[x - (board[x][y][0] == "b" ? -1 : 1)][y + i][0] != board[x][y][0] && board[x - (board[x][y][0] == "b" ? -1 : 1)][y + i] != "") {
                    possibleList += QPoint(x - (board[x][y][0] == "b" ? -1 : 1), y + i);
                }
            }
        }
    }
    else if (board[x][y][1] == "r") {
        // rule of rock
        testWalkOff(x, y, "u", possibleList);
        testWalkOff(x, y, "d", possibleList);
        testWalkOff(x, y, "l", possibleList);
        testWalkOff(x, y, "r", possibleList);
    }
    else if (board[x][y][1] == "k") {
        // rule of king
        for (int i = -1; i <= 1; i ++) {
            for (int j = -1; j <= 1; j ++) {
                if (x + i >= 0 && x + i < 8 && y + j >= 0 && y + j < 8) {
                    if (board[x][y][0] != board[x + i][y + j][0])
                        possibleList += QPoint(x + i, y + j);
                }
            }
        }

        // check if can castling
        castlingList = {NONEXIST, NONEXIST};
        if (target == QPoint((board[x][y][0] == "b" ? 0 : 7), 4)) {
            // king side castling
            if (board[(board[x][y][0] == "b" ? 0 : 7)][6] == "" && tile[x][y]->firstMove && tile[(board[x][y][0] == "b" ? 0 : 7)][7]->firstMove) {
                if (board[x][y][0] == board[(board[x][y][0] == "b" ? 0 : 7)][7][0] && board[(board[x][y][0] == "b" ? 0 : 7)][7][1] == "r") {
                    possibleList += QPoint((board[x][y][0] == "b" ? 0 : 7), 6);
                    castlingList[1] = QPoint((board[x][y][0] == "b" ? 0 : 7), 6);
                }
            }
            // queen side castling
            if (board[(board[x][y][0] == "b" ? 0 : 7)][2] == "" && board[(board[x][y][0] == "b" ? 0 : 7)][1] == "" && tile[x][y]->firstMove && tile[(board[x][y][0] == "b" ? 0 : 7)][0]->firstMove) {
                if (board[x][y][0] == board[(board[x][y][0] == "b" ? 0 : 7)][0][0] && board[(board[x][y][0] == "b" ? 0 : 7)][0][1] == "r") {
                    possibleList += QPoint((board[x][y][0] == "b" ? 0 : 7), 2);
                    castlingList[0] = QPoint((board[x][y][0] == "b" ? 0 : 7), 2);
                }
            }
        }
    }
    else if (board[x][y][1] == "b") {
        // rule of bishop
        testWalkOff(x, y, "lu", possibleList);
        testWalkOff(x, y, "ru", possibleList);
        testWalkOff(x, y, "ld", possibleList);
        testWalkOff(x, y, "rd", possibleList);
    }

    else if (board[x][y][1] == "q") {
        // rule of queen
        testWalkOff(x, y, "lu", possibleList);
        testWalkOff(x, y, "ru", possibleList);
        testWalkOff(x, y, "ld", possibleList);
        testWalkOff(x, y, "rd", possibleList);
        testWalkOff(x, y, "u", possibleList);
        testWalkOff(x, y, "d", possibleList);
        testWalkOff(x, y, "l", possibleList);
        testWalkOff(x, y, "r", possibleList);
    }

    else if (board[x][y][1] == "n") {
        // rule of knight
        for (int i = -1; i <= 1; i += 2) {
            for (int j = -1; j <= 1; j += 2) {
                if (x + 2 * i >= 0 && x + 2 * i < 8 && y + j >= 0 && y + j < 8) {
                    if (board[x + 2 * i][y + j][0] == board[x][y][0])
                        continue;
                    if (board[x + 2 * i][y + j] == "" || board[x + 2 * i][y + j][0] != board[x][y][0]) {
                        possibleList += QPoint(x + 2 * i, y + j);
                    }
                }
                if (x + i >= 0 && x + i < 8 && y + 2 * j >= 0 && y + 2 * j < 8) {
                    if (board[x + i][y + 2 * j][0] == board[x][y][0])
                        continue;
                    if (board[x + i][y + 2 * j] == "" || board[x + i][y + 2 * j][0] != board[x][y][0]) {
                        possibleList += QPoint(x + i, y + 2 * j);
                    }
                }
            }
        }
    }
}

void Game::setRemind(bool ipt) {
    QSet <QPoint> ::iterator it;
    for (it = possibleList.begin(); it != possibleList.end(); it ++) {
        int x = it->x(), y = it->y();
        tile[x][y]->setRemind(ipt);
    }
}

bool Game::isValid(QPoint pos) {
    return possibleList.contains(pos);
}

void Game::testWalkOff(int x, int y, QString direction, QSet <QPoint> &possibleList) {
    if (direction == "u") {
        for (int i = x - 1; i >= 0; i --) { // forward
            if (board[i][y] == "") {
                possibleList += QPoint(i, y);
            }
            // can kill
            else if (board[i][y][0] != board[x][y][0]) {
                possibleList += QPoint(i, y);
                break;
            }
            else if (board[i][y][0] == board[x][y][0]) {
                break;
            }
        }
    }
    else if (direction == "d") {
        for (int i = x + 1; i < 8; i ++) { // backward
            if (board[i][y] == "") {
                possibleList += QPoint(i, y);
            }
            // can kill
            else if (board[i][y][0] != board[x][y][0]) {
                possibleList += QPoint(i, y);
                break;
            }
            else if (board[i][y][0] == board[x][y][0]) {
                break;
            }
        }
    }
    else if (direction == "l") {
        for (int i = y - 1; i >= 0; i --) { // leftward
            if (board[x][i] == "") {
                possibleList += QPoint(x, i);
            }
            // can kill
            else if (board[x][i][0] != board[x][y][0]) {
                possibleList += QPoint(x, i);
                break;
            }
            else if (board[x][i][0] == board[x][y][0]) {
                break;
            }
        }
    }
    else if (direction == "r") {
        for (int i = y + 1; i < 8; i ++) { // rightward
            if (board[x][i] == "") {
                possibleList += QPoint(x, i);
            }
            // can kill
            else if (board[x][i][0] != board[x][y][0]) {
                possibleList += QPoint(x, i);
                break;
            }
            else if (board[x][i][0] == board[x][y][0]) {
                break;
            }
        }
    }
    else if (direction == "lu") {
        int i = 1; // left-up
        while (x - i >= 0 && y - i >= 0) {
            if (board[x - i][y - i] == "") {
                possibleList += QPoint(x - i, y - i);
            }
            else if (board[x - i][y - i][0] != board[x][y][0]) {
                possibleList += QPoint(x - i, y - i);
                break;
            }
            else if (board[x - i][y - i][0] == board[x][y][0]) {
                break;
            }
            i ++;
        }
    }
    else if (direction == "ru") {
        int i = 1; // right-up
        while (x - i >= 0 && y + i < 8) {
            if (board[x - i][y + i] == "") {
                possibleList += QPoint(x - i, y + i);
            }
            else if (board[x - i][y + i][0] != board[x][y][0]) {
                possibleList += QPoint(x - i, y + i);
                break;
            }
            else if (board[x - i][y + i][0] == board[x][y][0]) {
                break;
            }
            i ++;
        }
    }
    else if (direction == "ld") {
        int i = 1; // left-down
        while (x + i < 8 && y - i >= 0) {
            if (board[x + i][y - i] == "") {
                possibleList += QPoint(x + i, y - i);
            }
            else if (board[x + i][y - i][0] != board[x][y][0]) {
                possibleList += QPoint(x + i, y - i);
                break;
            }
            else if (board[x + i][y - i][0] == board[x][y][0]) {
                break;
            }
            i ++;
        }
    }
    else if (direction == "rd") {
        int i = 1; // right-down
        while (x + i < 8 && y + i < 8) {
            if (board[x + i][y + i] == "") {
                possibleList += QPoint(x + i, y + i);
            }
            else if (board[x + i][y + i][0] != board[x][y][0]) {
                possibleList += QPoint(x + i, y + i);
                break;
            }
            else if (board[x + i][y + i][0] == board[x][y][0]) {
                break;
            }
            i ++;
        }
    }
}

// to implement QSet on QPoint
inline uint qHash (const QPoint &key) {
    return qHash(QPair <int, int> (key.x(), key.y()));
}

void Game::gameJudge() {
    // judge for check
    QPoint whiteking, blackking;
    QSet <QPoint> blackField, whiteField, judge, temp;
    for (int i = 0; i < 8; i ++) {
        for (int j = 0; j < 8; j ++) {
            if (board[i][j] == "")
                continue;
            else {
                canMove(QPoint(i, j), temp);
                if (board[i][j][0] == "b") {
                    if (board[i][j][1] == "k")
                        blackking = QPoint(i, j);
                    blackField += temp;
                }
                else if (board[i][j][0] == "w") {
                    if (board[i][j][1] == "k")
                        whiteking = QPoint(i, j);
                    whiteField += temp;
                }
            }
        }
    }
    // judge for black side
    if (whiteField.contains(blackking)) {
        // white check black
        check = true;
        whocheck = "w";
        qDebug() << "white check";
    }

    canMove(blackking, temp);
    judge = temp - whiteField;
    if (judge.empty()) {
        // stalement
    }

    // judge for white side
    if (blackField.contains(whiteking)) {
        // black check white
        check = true;
        whocheck = "b";
        qDebug() << "black check";
    }

    canMove(whiteking, temp);
    judge = temp - blackField;
    if (judge.empty()) {
        // stalement
    }

}

void Game::gameOver() {
    rect->setZValue(0);

    rect->fadeIn();

    title->setPlainText(tr(checkmate == "b" ? "black" : "white") + tr("\nwin\nthe\ngame"));
    title->setY(scene->height() / 2 - title->boundingRect().height() / 2 - 20);
    title->slideIn();

    back->setPos(380, scene->height() / 2 - quit->height() / 2 + 70);
    back->show();

    again->setPos(380, scene->height() / 2 - quit->height() / 2);
    again->show();
}

void Game::gameStart() {
    menuShow = false;

    // get rid of the menu object
    rect->fadeOut();
    play->hide();
    quit->hide();
    edit->hide();
    again->hide();
    back->hide();

    // create the board
    if (!editMode) {
        title->slideOut();
        cleanBoard();
        putChess(); // normal play
    }
    else {
        rect->setZValue(1);
        tile[focusChess.x()][focusChess.y()]->setZValue(0);
        tile[focusChess.x()][focusChess.y()]->editEndEnvent();

        delete box;
    }

    editMode = false;

    focusChess = NONEXIST;
    castlingList = {NONEXIST, NONEXIST};

    // for game play
    turn = "w";
    check = false;
    checkmate = "";
}

void Game::displayMenu() {
    menuShow = true;

    if (editMode) {
        delete box;
    }

    pause = false;
    editMode = false;

    rect->setZValue(0);

    back->hide();
    resume->hide();
    again->hide();

    cleanBoard();
    putChess();

    title->setPlainText("chess\ngame");
    title->setPos(30, scene->height() / 2 - title->boundingRect().height() / 2 - 20);
    title->slideIn();

//    rect->show();

    play->setPos(380, scene->height() / 2 - play->height() / 2 - 70);
    play->show();

    quit->setPos(380, scene->height() / 2 - quit->height() / 2 + 70);
    quit->show();

    edit->setPos(380, scene->height() / 2 - edit->height() / 2);
    edit->show();
}

void Game::cleanBoard() {
    for (int i = 0; i < 8; i ++) {
        for (int j = 0; j < 8; j ++) {
            board[i][j] = "";
            tile[i][j]->setKind();
        }
    }
}

void Game::gameEdit() {
    menuShow = false;

    box = new EditBox();
    scene->addItem(box);

    cleanBoard();
    rect->fadeOut();
    title->slideOut();
    again->hide();
    back->hide();
    quit->hide();
    edit->hide();
    play->hide();
    box->hide();
    resume->hide();

    focusChess = NONEXIST;

    connect(box, SIGNAL(readyToStart()), this, SLOT(gameStart()));
    connect(box, SIGNAL(remove()), this, SLOT(editRemove()));
    connect(this, SIGNAL(chessRemove(QString)), box, SLOT(chessDel(QString)));
    connect(box, SIGNAL(addChess(QString)), this, SLOT(editChess(QString)));

    editMode = true;
}

void Game::editRemove() {
    emit chessRemove(board[focusChess.x()][focusChess.y()]);

    board[focusChess.x()][focusChess.y()] = "";
    tile[focusChess.x()][focusChess.y()]->setKind("");

    // for edit end
    tile[focusChess.x()][focusChess.y()]->editEndEnvent();
    tile[focusChess.x()][focusChess.y()]->setZValue(0);
    focusChess = NONEXIST;
    box->hide();
    rect->fadeOut();
}

void Game::editChess(QString type) {
    board[focusChess.x()][focusChess.y()] = type;
    tile[focusChess.x()][focusChess.y()]->setKind(type);

    // for edit end
    tile[focusChess.x()][focusChess.y()]->editEndEnvent();
    tile[focusChess.x()][focusChess.y()]->setZValue(0);
    focusChess = NONEXIST;
    box->hide();
    rect->fadeOut();
}

void Game::keyPressEvent(QKeyEvent *event) {
    if (menuShow)
        return;
    if (event->key() == Qt::Key_Escape) {
        rect->setZValue(0);

        if (!pause) {
            title->setPlainText("game\npause");
            title->setY(scene->height() / 2 - title->boundingRect().height() / 2 - 20);
            title->slideIn();

            back->show();

            resume->setPos(380, scene->height() / 2 - quit->height() / 2 - 35);
            resume->show();

            back->setPos(380, scene->height() / 2 - quit->height() / 2 + 35);
            back->show();

            rect->fadeIn();

            pause = true;
        }
        else {
            title->slideOut();
            resume->hide();
            back->hide();
            rect->fadeOut();

            pause = false;
        }
    }
}

void Game::gameResume() {
    rect->setZValue(0);
    title->slideOut();
    resume->hide();
    back->hide();
    rect->fadeOut();

    pause = false;
}
