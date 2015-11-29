#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>

#include <iostream>
#include <strings.h>
#include <sstream>
#include <thread>
#include <mutex>

#include "../rapidjson/document.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/writer.h"
#include "../server/server_api.hpp"

using namespace rapidjson;

std::mutex m;
bool TALK_WITH_SERVER = true;

void send_data(int sockfd) {
    while(true) {
        std::string str;
        getline(std::cin, str);

        if (str == "!toserver") {
            TALK_WITH_SERVER = true;

            std::stringstream ss;
            ss << "{\"method\": \"" << API::METHODS::DESTROY_ROOM << "\"}";
            str = ss.str();

            send(sockfd, str.c_str(), str.size(), 0);
            continue;
        }

        if (str[0] == ':') {
            std::stringstream ss;
            ss << "{\"method\": \"" << API::METHODS::CREATE_ROOM << "\", " << "\"roommate\": \"" <<
                str.substr(1) << "\"}";
            str = ss.str();
            send(sockfd, str.c_str(), str.size(), 0);
            TALK_WITH_SERVER = false;
            continue;
        }

        if (TALK_WITH_SERVER) {
            send(sockfd, str.c_str(), str.size(), 0);
        } else {
            std::stringstream ss;
            ss << "{\"method\": \"" << API::METHODS::SEND_TO_ROOMMATE << "\", \"msg\": \"" << str << "\"}";
            str = ss.str();
            send(sockfd, str.c_str(), str.size(), 0);
        }
    }
    close(sockfd);
}

void get_data(int sockfd) {
    Document d;
    char buff[4096] = {0};
    ssize_t res = 0;

    while ((res = recv(sockfd, buff, sizeof(buff) - 1, 0)) > 0) {
        buff[res] = '\0';
        std::cout << buff << std::endl;
        d.Parse(buff);
        if (d.IsObject() && d.HasMember("error")) {
            TALK_WITH_SERVER = true;
        }
        bzero(buff, sizeof(buff));
    }
}

int main(int argc, char **argv) {

    const int port = 8080;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 1) {
        const char *error = "socket error\n";
        write(STDIN_FILENO, error, sizeof(error));
    }
    sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_aton("127.0.0.1", &(server.sin_addr));

    if (connect(sockfd, (sockaddr *) (&server), sizeof(server)) != 0) {
        std::cout << "error connect" << std::endl;
        exit(1);
    }

    std::thread t1(send_data, sockfd);
    std::thread t2(get_data, sockfd);

    t1.join();
    t2.join();
    return 0;
};
