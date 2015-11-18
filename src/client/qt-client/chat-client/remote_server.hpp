#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <memory>

#include <QDebug>
#include <QMessageBox>
#include <QTcpSocket>

class Server : public QObject {
    Q_OBJECT
public:
    Server() = delete;
    Server(QString, QString, int);
    ~Server();
    Server(const Server&) = delete;

    void establish_connection();
    void disconnect();
    QString get_address();
    const std::unique_ptr<QTcpSocket>& get_client_socket();

signals:
    void socket_fails(QAbstractSocket::SocketError);
    void socket_connected();

private slots:
    void socket_ready_read();


private:

    QString nickname;
    QString ip;
    int port;
    std::unique_ptr<QTcpSocket> client_socket = nullptr;
};

#endif // SERVER_HPP
