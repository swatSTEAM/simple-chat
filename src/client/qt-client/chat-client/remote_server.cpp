#include "remote_server.hpp"


Server::Server(std::string _nickname, std::string _ip, int _port) {
    nickname = _nickname;
    ip = _ip;
    port = _port;
}

int Server::connect() {
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 1) {
        client_socket = 0;
        return SOCKET_ERROR;
    }

    sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (inet_aton(ip.c_str(), &(server.sin_addr)) < 1) {
        client_socket = 0;
        return ADDRESS_ERROR;
    }

    if (::connect(client_socket, (sockaddr *) (&server), sizeof(server)) != 0) {
        close(client_socket);
        client_socket = 0;
        return CONNECTION_ERROR;
    }

    return SUCCESS;
}

void Server::disconnect() {
    if (client_socket != 0) {
        close(client_socket);
        client_socket = 0;
    }
}

std::string Server::get_address() {
    return ip + ":" + std::to_string(port);
}
