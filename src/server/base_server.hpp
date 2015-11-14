#ifndef BASE_SERVER_HPP
#define BASE_SERVER_HPP

#include <iostream>
#include <string>

#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

typedef int sockfd_t;

/*!
	@brief Base abstract TCP server class.
*/

class BaseServer {

public:
    explicit BaseServer(int port);
    virtual ~BaseServer();

    BaseServer() = delete;
    BaseServer(const BaseServer&) = delete;
    BaseServer& operator=(const BaseServer&) = delete;

    virtual std::string get_address() const;
    virtual std::string get_ip() const;
    virtual int get_port() const;
    virtual void stop_server();
    virtual void serve_forever() = 0;

protected:
    int port;
    sockfd_t server_socket;
    int socket_type = SOCK_STREAM;
    int request_queue_size = 5;
    bool allow_reuse_address = true;
    sa_family_t address_family = AF_INET;
    sockaddr_in socket_address;


    static constexpr size_t BUFFER_SIZE = 4096;
    ///< Length of bytes that #server_socket accepts at a time.

    virtual void server_init();
    virtual void server_bind();
    virtual void server_activate();
};

#endif //BASE_SERVER_HPP
