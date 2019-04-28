#include "game.h"
#include <QGraphicsRectItem>
#include <QDebug>
#include <iostream>
#include <QEventLoop>

Game::Game() {
    // set the game window
    setFixedSize(width, length);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // create and set the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, width, length);
    setScene(scene);

    // create animation object
    anime = new Animate();
    anime->hide();
    animation = new QPropertyAnimation(anime, "pos");

    // create the board
    makeBoard();
    putChess();

    focusChess = UNFOCUS;

    // for game play
    turn = "w";

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
        }
    }
}

void Game::mousePressed(QPoint pos) {
    qDebug() << pos << board[pos.x()][pos.y()];

    // set focus
    if (focusChess == UNFOCUS) {
        // test if the tile has no chess
        if (board[pos.x()][pos.y()] == "")
            return;
        // chack if the right chess is being chosen
        if (turn != board[pos.x()][pos.y()][0])
            return ;
        // check if the chess cannot move
        canMove(pos);
        if (possibleList.empty())
            return;

        focusChess = pos;
        tile[pos.x()][pos.y()]->focus(true);

//        qDebug() << "START can move list";
//        for (int i = 0; i < possibleList.size(); i ++) {
//            qDebug() << possibleList.at(i);
//        }
//        qDebug() << "END";

        setRemind(true);

        qDebug() << "set focus";
    }
    else if (focusChess == pos || !isValid(pos)) {
        tile[focusChess.x()][focusChess.y()]->focus(false);
        setRemind(false);
        focusChess = UNFOCUS;
        possibleList.clear();
        qDebug() << "release focus";
    }
    else if (isValid(pos)) {
        qDebug() << "move chess";
        tile[focusChess.x()][focusChess.y()]->focus(false);
        moveChess(focusChess, pos);
        possibleList.clear();
        focusChess = UNFOCUS;

        // change the right
        if (turn == "b")
            turn = "w";
        else if (turn == "w")
            turn = "b";

        qDebug() << "release focus";
    }
}

void Game::moveChess(QPoint init, QPoint final) {
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

    // promotion set the pawn to queen
    if (board[final.x()][final.y()][1] == "p" && final.x() == (board[final.x()][final.y()][0] == "b" ? 7 : 0)) {
        qDebug() << "promotion";
        board[final.x()][final.y()] = board[final.x()][final.y()][0] + "q";
    }

    tile[final.x()][final.y()]->setKind(board[final.x()][final.y()]);

    for (int i = 0; i < 8; i ++)
        qDebug("%2s %2s %2s %2s %2s %2s %2s %2s", board[i][0].toStdString().c_str(), board[i][1].toStdString().c_str(), board[i][2].toStdString().c_str(), board[i][3].toStdString().c_str(), board[i][4].toStdString().c_str(), board[i][5].toStdString().c_str(), board[i][6].toStdString().c_str(), board[i][7].toStdString().c_str());
}

void Game::canMove(QPoint target) {
    int x = target.x(), y = target.y();

    if (board[x][y][1] == "p") {
        // rule of pawn
        for (int i = 1; i <= 2; i ++) {
            // move
            if (board[x - (board[x][y][0] == "b"? -i : i)][y] == "") {
                possibleList.push_back(QPoint(x - (board[x][y][0] == "b"? -i : i), y));
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
                    possibleList.push_back(QPoint(x - (board[x][y][0] == "b" ? -1 : 1), y + i));
                }
            }
        }
    }
    else if (board[x][y][1] == "r") {
        // rule of rock
        testWalkOff(x, y, "u");
        testWalkOff(x, y, "d");
        testWalkOff(x, y, "l");
        testWalkOff(x, y, "r");
    }
    else if (board[x][y][1] == "k") {
        // rule of king
        for (int i = -1; i <= 1; i ++) {
            for (int j = -1; j <= 1; j ++) {
                if (x + i >= 0 && x + i < 8 && y + j >= 0 && y + j < 8) {
                    if (board[x][y][0] != board[x + i][y + j][0])
                        possibleList.push_back(QPoint(x + i, y + j));
                }
            }
        }
    }
    else if (board[x][y][1] == "b") {
        // rule of bishop
        testWalkOff(x, y, "lu");
        testWalkOff(x, y, "ru");
        testWalkOff(x, y, "ld");
        testWalkOff(x, y, "rd");
    }

    else if (board[x][y][1] == "q") {
        // rule of queen
        testWalkOff(x, y, "lu");
        testWalkOff(x, y, "ru");
        testWalkOff(x, y, "ld");
        testWalkOff(x, y, "rd");
        testWalkOff(x, y, "u");
        testWalkOff(x, y, "d");
        testWalkOff(x, y, "l");
        testWalkOff(x, y, "r");
    }

    else if (board[x][y][1] == "n") {
        // rule of knight
        for (int i = -1; i <= 1; i += 2) {
            for (int j = -1; j <= 1; j += 2) {
                if (x + 2 * i >= 0 && x + 2 * i < 8 && y + j >= 0 && y + j < 8) {
                    if (board[x + 2 * i][y + j][0] == board[x][y][0])
                        continue;
                    if (board[x + 2 * i][y + j] == "" || board[x + 2 * i][y + j][0] != board[x][y][0]) {
                        possibleList.push_back(QPoint(x + 2 * i, y + j));
                    }
                }
                if (x + i >= 0 && x + i < 8 && y + 2 * j >= 0 && y + 2 * j < 8) {
                    if (board[x + i][y + 2 * j][0] == board[x][y][0])
                        continue;
                    if (board[x + i][y + 2 * j] == "" || board[x + i][y + 2 * j][0] != board[x][y][0]) {
                        possibleList.push_back(QPoint(x + i, y + 2 * j));
                    }
                }
            }
        }
    }
}

void Game::setRemind(bool ipt) {
    for (int i = 0; i < possibleList.size(); i ++) {
        int x = possibleList[i].x(), y = possibleList[i].y();
        tile[x][y]->setRemind(ipt);
    }
}

bool Game::isValid(QPoint pos) {
    for (int i = 0; i < possibleList.size(); i ++) {
        if (pos == possibleList.at(i))
            return true;
    }
    return false;
}

void Game::testWalkOff(int x, int y, QString direction) {
    if (direction == "u") {
        for (int i = x - 1; i >= 0; i --) { // forward
            if (board[i][y] == "") {
                possibleList.push_back(QPoint(i, y));
            }
            // can kill
            else if (board[i][y][0] != board[x][y][0]) {
                possibleList.push_back(QPoint(i, y));
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
                possibleList.push_back(QPoint(i, y));
            }
            // can kill
            else if (board[i][y][0] != board[x][y][0]) {
                possibleList.push_back(QPoint(i, y));
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
                possibleList.push_back(QPoint(x, i));
            }
            // can kill
            else if (board[x][i][0] != board[x][y][0]) {
                possibleList.push_back(QPoint(x, i));
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
                possibleList.push_back(QPoint(x, i));
            }
            // can kill
            else if (board[x][i][0] != board[x][y][0]) {
                possibleList.push_back(QPoint(x, i));
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
                possibleList.push_back(QPoint(x - i, y - i));
            }
            else if (board[x - i][y - i][0] != board[x][y][0]) {
                possibleList.push_back(QPoint(x - i, y - i));
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
                possibleList.push_back(QPoint(x - i, y + i));
            }
            else if (board[x - i][y + i][0] != board[x][y][0]) {
                possibleList.push_back(QPoint(x - i, y + i));
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
                possibleList.push_back(QPoint(x + i, y - i));
            }
            else if (board[x + i][y - i][0] != board[x][y][0]) {
                possibleList.push_back(QPoint(x + i, y - i));
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
                possibleList.push_back(QPoint(x + i, y + i));
            }
            else if (board[x + i][y + i][0] != board[x][y][0]) {
                possibleList.push_back(QPoint(x + i, y + i));
                break;
            }
            else if (board[x + i][y + i][0] == board[x][y][0]) {
                break;
            }
            i ++;
        }
    }
}

void Game::test() {
    qDebug() << "animation end";
}
