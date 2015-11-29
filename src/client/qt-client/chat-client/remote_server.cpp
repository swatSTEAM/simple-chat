#include "remote_server.hpp"

Server::Server(std::string _nickname, std::string _ip, int _port)
    : nickname(_nickname), ip(_ip), port(_port) { }

Server::~Server() {

}

void Server::establish_connection() {
    disconnect();

    master_socket = std::move(
                std::unique_ptr<QTcpSocket>(new QTcpSocket(nullptr))
                );

    connect(master_socket.get(), SIGNAL(connected()),
            SIGNAL(connected()), Qt::DirectConnection);

    connect(master_socket.get(), SIGNAL(readyRead()),
            SIGNAL(readyRead()), Qt::DirectConnection);

    // Qt signal doesn't work properly. Sometimes it simply doesn't emit error.
    // If you know how to tackle with it please share your ideas here:
    // http://stackoverflow.com/questions/33977234/qtcpsocket-does-not-emit-error-signal
    // Due to this trouble I check socket state and emit signal manually if error occurs.

    //    connect(master_socket.get(), SIGNAL(error(QAbstractSocket::SocketError)),
    //            SIGNAL(error(QAbstractSocket::SocketError)), Qt::DirectConnection);

    master_socket->connectToHost(ip.c_str(), port);
    master_socket->waitForConnected(timeout*1000);

    if (master_socket->state() == QAbstractSocket::UnconnectedState) {
        disconnect();
        emit error();
    }

    emit stop_thread();
}

void Server::disconnect() {
    if (master_socket) {
        master_socket->disconnectFromHost();
    }
}

QString Server::get_address() {
    return QString::fromStdString(ip) + ":" + QString::number(port);
}
