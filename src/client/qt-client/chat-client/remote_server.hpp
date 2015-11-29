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
    Server(std::string, std::string, int);
    ~Server();
    Server(const Server&) = delete;

    void disconnect();
    QString get_address();

private slots:
    void establish_connection();

signals:
    void connected();
    void readyRead();
    void error();
    void stop_thread();
private:
    std::string nickname;
    std::string ip;
    int port;
    const int timeout = 5;
    std::unique_ptr<QTcpSocket> master_socket = nullptr;
};

#endif // SERVER_HPP
