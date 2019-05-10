#include "radio.h"
#include <QByteArray>
#include <QDataStream>
#include <QBrush>
#include <QPen>
#include <QList>
#include <QNetworkInterface>
#include "game.h"

Radio::Radio() {
    setHandlesChildEvents(false);

    // handle transaction process
    server = new QTcpServer();
    socket = new QTcpSocket();
    checkConnect = new QTimer();
    checkConnect->start(100);

    // handle interface
    ok = new Button("connect", 100, 40, 15);
    addToGroup(ok);
    connect(ok, SIGNAL(clicked()), this, SLOT(startConnect()));
    ok->setPos(0, 180);

    create = new Button("create", 140, 50, 25);
    addToGroup(create);
    create->setPos(0, 0);
    connect(create, SIGNAL(clicked()), this, SLOT(setServer()));

    join = new Button("join");
    addToGroup(join);
    join->setPos(0, 70);
    connect(join, SIGNAL(clicked()), this, SLOT(setClient()));

    back = new Button("back", 100, 40, 20);
    back->hide();
    addToGroup(back);
    connect(back, SIGNAL(clicked()), this, SIGNAL(goBack()));

    // set the title
    ip = new AnimateText("ip address", 20);
    port = new AnimateText("port", 20);
    addToGroup(ip);
    addToGroup(port);
    ip->setPos(0, 0);
    port->setPos(0, 80);

    // create the textinput object
    ipInput = new TextInput(320, 50, 25);
    portInput = new TextInput(120, 50, 25);

    connect(ipInput, SIGNAL(clicked()), this, SLOT(showIpInput()));
    connect(portInput, SIGNAL(clicked()), this, SLOT(showPortInput()));

    ipInput->setPos(0, 33);
    portInput->setPos(0, 113);

    ipInput->setMaxText(15);
    portInput->setMaxText(5);

    addToGroup(ipInput);
    addToGroup(portInput);

    ipInput->releaseFocus();
    portInput->releaseFocus();

    point = ipInput;

    ok->hide();
    ipInput->hide();
    portInput->hide();

    ip->hide();
    port->hide();
}

void Radio::startConnect() {
    qDebug() << "connected";
    emit connected();

    if (role == "host") {
        delete socket;
        socket = server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    }
    else if (role == "client") {   
        QString host = ipInput->getContent();
        int port = portInput->getContent().toUInt();

        socket->connectToHost(host, port);

        checkConnect->start();
        connect(checkConnect, SIGNAL(timeout()), this, SLOT(connectSuccess()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));

        ipInput->hide();
        portInput->hide();
        ok->hide();
    }

    ip->hide();
    port->hide();
    back->hide();
}

void Radio::sendMessage(QString content) {
    QByteArray byte;
    QDataStream stream(&byte, QIODevice::WriteOnly);

    stream << content;
    socket->write(byte);
}

void Radio::connectSuccess() {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        disconnect(checkConnect);
    }
}

void Radio::readMessage() {
    QDataStream in(socket);

    in.startTransaction();
    if (!in.commitTransaction()) {
        return;
    }

    in >> data;
    qDebug() << data;
    emit received(turn);
}

void Radio::showIpInput() {
    portInput->releaseFocus();
    ipInput->getFocus();

    point = ipInput;
}

void Radio::showPortInput() {
    ipInput->releaseFocus();
    portInput->getFocus();

    point = portInput;
}

void Radio::keyPressEvent(QKeyEvent *event) {
    // passing the event into the object
    point->keyPressEvent(event);
}

void Radio::setServer() {
    role = "host";
    turn = "w";
    emit select(role);

    // set the server
    server->listen();
    connect(server, SIGNAL(newConnection()), this, SLOT(startConnect()));
    QList <QHostAddress> ipList = QNetworkInterface::allAddresses();

    QString tmp;
    foreach(QHostAddress i, ipList) {
        if (i.toIPv4Address())
            tmp += i.toString() + "\n";
    }

    ip->setPlainText("ip:\n" + tmp);
    port->setPlainText("port:\n" + QString::number(server->serverPort()));

    ip->setFontSize(25);
    port->setFontSize(25);
    ip->setPos(0, 0);
    port->setPos(0, 100);

    // set the appearance
    ok->hide();
    ipInput->hide();
    portInput->hide();
//    ipRect->hide();
//    portRect->hide();
    create->hide();
    join->hide();
    ip->show();
    port->show();

    back->show();
    back->setPos(0, 180);
}

void Radio::setClient() {
    role = "client";
    turn = "b";
    emit select(role);

    ip->setFontSize(20);
    port->setFontSize(20);
    ip->setPos(0, 0);
    port->setPos(0, 80);

    // set the appearance
    ok->show();
    ipInput->show();
    portInput->show();
//    ipRect->show();
//    portRect->show();
    ip->show();
    port->show();
    create->hide();
    join->hide();

    back->show();
    back->setPos(220, 180);
    ok->setPos(100, 180);
}

QString Radio::getRole() {
    return role;
}

QString Radio::getData() {
    return data;
}
