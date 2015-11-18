#include "remote_server.hpp"

#include <QDebug>
#include <QMessageBox>

Server::Server(QString _nickname, QString _ip, int _port)
    : nickname(_nickname), ip(_ip), port(_port) { }

Server::~Server() {
//    delete client_socket;
}

void Server::establish_connection() {
    client_socket = std::move(
                std::unique_ptr<QTcpSocket>(new QTcpSocket));

//    client_socket->connectToHost(ip, port);
    if (client_socket->waitForConnected(1))
        qDebug("Connected!");

    connect(client_socket.get(), SIGNAL(connected()), SIGNAL(socket_connected()));

    connect(client_socket.get(), SIGNAL(readyRead()), SLOT(socket_ready_read()));

    connect(client_socket.get(), SIGNAL(error(QAbstractSocket::SocketError)),
            SIGNAL(socket_fails(QAbstractSocket::SocketError)));


}

void Server::disconnect() {
    if (client_socket != nullptr) {
//        close(client_socket);
        client_socket = nullptr;
    }
}

QString Server::get_address() {
    return ip + ":" + QString::number(port);
}

const std::unique_ptr<QTcpSocket>& Server::get_client_socket() {
    return client_socket;
}

void Server::socket_ready_read()
{

}

