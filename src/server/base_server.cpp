#include "base_server.hpp"

BaseServer::BaseServer(int port) {
    /*!
     * @brief Instantiates server with passed port and invokes service functions
     * @param[in] port - Server port
     */

    this->port = port;

    server_init();
    server_bind();
    server_activate();
}

void BaseServer::server_init() {
    /*!
     * @brief Creates server socket.
     * If #allow_reuse_address is true makes socket reusable.
     */

    if ((server_socket = socket(address_family, socket_type, 0)) == -1) {
        // TODO
        std::cerr << "socket creating error\n";
        exit(1);
    }

    if (allow_reuse_address) {
        int yes = 1;
        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            // TODO
            std::cerr << "set SO_REUSEADDR error\n";
            exit(1);
        }
    }
}

void BaseServer::server_bind() {
    /*!
     * @brief Binds #server_socket with server #port and local address.
     */

    bzero(&socket_address, sizeof(socket_address));
    socket_address.sin_family = address_family;
    socket_address.sin_port = htons(port);
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (sockaddr *) &socket_address, sizeof(socket_address)) < 0) {
        // TODO
        std::cerr << "socket bind error\n";
        exit(1);
    }
}

void BaseServer::server_activate() {
    /*!
     * @brief Makes #server_socket listening.
     */

    if (listen(server_socket, request_queue_size) < 0) {
        // TODO
        std::cerr << "listen error\n";
        exit(1);
    }
}

std::string BaseServer::get_ip() const {
    /*!
     * @brief Returns server IP
     */

    char buffer[INET_ADDRSTRLEN];
    inet_ntop(address_family, &(socket_address.sin_addr),
              buffer, INET_ADDRSTRLEN);
    return std::string(buffer);
}

BaseServer::~BaseServer() {
    stop_server();
}

int BaseServer::get_port() const {
    /*!
     * @brief Returns server #port
     */

    return port;
}

void BaseServer::stop_server() {
    /*!
     * @brief In #BaseServer closes #server_socket.
     */

    if (server_socket != -1) {
        close(server_socket);
        server_socket = -1;
    }
}

std::string BaseServer::get_address() const {
    /*!
     * @brief Returns server address in the following form: ip:port
     */

    return std::string(get_ip() + ":" + std::to_string(get_port()));
}