#include "chat_core.hpp"

void ChatCore::add_UI(QObject *ui_core)
{
    this->ui_core=ui_core;
    QObject::connect(ui_core, SIGNAL(loginSignal(QString, QString, QString)),
                     this, SLOT(tryconnect(QString, QString, QString)));
    QObject::connect(ui_core, SIGNAL(disconnectSignal()),
                     this, SLOT(disconnect()));
    QObject::connect(ui_core, SIGNAL(sendMessSignal(QString, QString)),
                     this, SLOT(send_mess(QString, QString)));
}

void ChatCore::tryconnect(const QString& nickname,
                                      const QString& ip, const QString& port)
{

    auto thread = new QThread(this);

    if (server != nullptr)
    {
        server->destroy_connection();
        delete server;
    }
    server = new Server(nickname.toStdString(), ip.toStdString(), port.toInt());
//    server->moveToThread(thread);
    connect(thread, SIGNAL(started()), server, SLOT(establish_connection()));

    connect(server, SIGNAL(connected()), this, SLOT(connection_established()));

    connect(server, SIGNAL(readyRead()), this, SLOT(message_received()));

    connect(server, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(connection_failed(QAbstractSocket::SocketError)));

    connect(server, SIGNAL(stop_thread()), thread, SLOT(quit()));
    thread->start();
}


void ChatCore::connection_established() {
    worker = std::unique_ptr<Worker>(
                new Worker(this, &this->msg_queue, &this->mutex));

//    connect(worker.get(), SIGNAL(authorization_failed()),
//            SLOT(authorization_failed()));
    connect(worker.get(), SIGNAL(online_users(QString)),
            SIGNAL(onlineUsers(QString)));
    connect(worker.get(), SIGNAL(msg_from_user(QString, QString)),
            SIGNAL(newMess(QString, QString)));
    connect(worker.get(), SIGNAL(user_connected(QString)),
            SIGNAL(userConnected(QString)));
    connect(worker.get(), SIGNAL(user_disconnected(QString)),
            SIGNAL(userDisconnected(QString)));


    worker->start();
    emit connected("Connected successfully");
}


void ChatCore::connection_failed(QAbstractSocket::SocketError e)
{
    QString err_msg = server->get_socket()->errorString();

    server = nullptr;
    if (worker != nullptr)
        worker->abort();
    emit failed(err_msg);
}

void ChatCore::message_received()
{
    mutex.lock();
    auto msg = server->read();
//    qDebug() << "ChatCore: " << msg;
    msg_queue.push(msg);
    mutex.unlock();
}


void ChatCore::send_mess(QString user, QString mess)
{
    if (server == nullptr)
        return;

    auto msg = "{\"method\": \"send_to_user\", \"user\": \"" +
                   user + "\", " +
                   "\"msg\": \""  + mess + "\"}";
    qDebug() << (msg);
    server->send(msg);
}

void ChatCore::disconnect()
{
    if (server == nullptr)
        return;

    server->destroy_connection();
    server = nullptr;
    if (worker != nullptr)
        worker->abort();

    emit disconnected("Disconnected from server");
}


ChatCore::~ChatCore()
{
    if (worker != nullptr)
        worker->abort();

    delete server;
}
