#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QObject>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QMutex>
#include "remote_server.hpp"
#include "worker.h"

class ChatCore : public QObject {
    Q_OBJECT

public:
    ChatCore() {}
    void add_UI(QObject *ui_core);
    ~ChatCore();

signals:
    void connected(const QString mess);
    void disconnected(const QString mess);
    void failed(const QString mess);
    void newMess(QString user, QString mess);

    void onlineUsers(QString messList);
//    void room_created_succsessfully(QString& mess);
    void userConnected(QString mess);
    void userDisconnected(QString mess);


public slots:
    void tryconnect(const QString&, const QString&, const QString&);
    void disconnect();
    void connection_established();
    void connection_failed(QAbstractSocket::SocketError);
    void send_mess(QString user, QString mess);
    void message_received();

private:
    Server *server = nullptr;
    QObject *ui_core;
    std::queue<QString> msg_queue;
    QMutex mutex;
    std::unique_ptr<Worker> worker;
};

#endif // MAIN_WINDOW_HPP
