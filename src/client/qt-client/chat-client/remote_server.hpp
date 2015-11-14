#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>

#include <iostream>
#include <string>
#include <memory>

#include <QDebug>
#include <QMessageBox>

class Server {
public:
    static constexpr int SUCCESS = 0;
    static constexpr int SOCKET_ERROR = 1;
    static constexpr int CONNECTION_ERROR = 2;
    static constexpr int ADDRESS_ERROR = 3;

    typedef std::unique_ptr<Server> server_ptr;
    Server() = delete;
    Server(std::string, std::string, int);
    int connect();
    void disconnect();
    std::string get_address();

private:
    std::string nickname;
    std::string ip;
    int port;
    int client_socket = 0;
};

#endif // CLIENT_HPP
