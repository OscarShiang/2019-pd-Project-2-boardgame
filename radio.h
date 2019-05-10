#ifndef RADIO_H
#define RADIO_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QGraphicsItemGroup>
#include <QTimer>
#include <QObject>
#include <QKeyEvent>
#include "button.h"
#include "animatetext.h"
#include "textinput.h"
#include "animaterect.h"

class Radio: public QObject, public QGraphicsItemGroup {
    Q_OBJECT
public:
    Radio();
    ~Radio();

    void sendMessage(QString content);
    void keyPressEvent(QKeyEvent *event);
    QString getRole();
    QString getData();

signals:
    void goBack();
    void select(QString out);
    void connected();
    void received(QString out);

public slots:
    void startConnect();
    void showIpInput();
    void showPortInput();
    void connectSuccess();
    void readMessage();
    void setServer();
    void setClient();

private:
    QString role;
    QString data;
    QString turn;
    QTcpServer *server;
    QTcpSocket *socket;
    QTcpSocket *receiver;
    QTimer *checkConnect;

    Button *ok, *create, *join, *back;
    TextInput *ipInput, *point;
    TextInput *portInput;
    AnimateRect *ipRect;
    AnimateRect *portRect;
    AnimateText *ip, *port;
};

#endif // RADIO_H
