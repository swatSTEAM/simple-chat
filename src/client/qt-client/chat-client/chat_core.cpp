#include "chat_core.hpp"

ChatCore::ChatCore()
{

//    connect(ui->actionConnect, SIGNAL(triggered()),
//            SLOT(action_connect_triggered()));

//    connect(ui->actionDisconnect, SIGNAL(triggered()),
//            SLOT(action_disconnect_triggered()));
}

void ChatCore::tryconnect(const QString& nickname,
                                      const QString& ip, const QString& port)
{
    qDebug() << nickname << ip << port;
    remote_server = new Server(nickname.toStdString(), ip.toStdString(), port.toInt());

    connect(remote_server, SIGNAL(connected()), SLOT(connection_established()));
    connect(remote_server, SIGNAL(error()), SLOT(connection_failed()));

    auto thread = new QThread(this);
    remote_server->moveToThread(thread);

    connect(thread, SIGNAL(started()), remote_server, SLOT(establish_connection()));
    connect(remote_server, SIGNAL(stop_thread()), thread, SLOT(quit()));

    thread->start();
}

void ChatCore::connection_failed() {
    delete remote_server;
    remote_server = nullptr;
    emit failed("Connection failed");
}

void ChatCore::disconnect() {
    if (remote_server == nullptr) {
        return;
    }
    remote_server->disconnect();
    remote_server = nullptr;
    emit disconnected("Disconnected from server");
}

void ChatCore::connection_established() {
    emit connected("Connected successfully");
}


ChatCore::~ChatCore() {
    delete remote_server;
}
