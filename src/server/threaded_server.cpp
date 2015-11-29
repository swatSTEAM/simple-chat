#include "threaded_server.hpp"

using std::strcmp;

ThreadedServer::ThreadedServer(int port) :
        BaseServer::BaseServer(port)
{
    //TODO
}

ThreadedServer::~ThreadedServer() {
    // TODO
}

void ThreadedServer::stop_server() {
    // TODO
    BaseServer::stop_server();
}

void ThreadedServer::serve_forever() {
    /*!
     * @brief Main server loop.
     *
     * In endless loop the #master_socket tries to accept new connections. If accept is OK,
     * creates new user and handles him in separate deamon threads.
     */

    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    while(true) {
        sockfd_t client_sock = accept(master_socket, (sockaddr *) &client_addr, &len);
        if (client_sock == -1) {
            // TODO
            std::cout << strerror(errno) << std::endl;
            exit(1);
        }
        std::cout << "new connection" << std::endl;
        User::user_ptr new_user = std::make_shared<User>(client_sock, client_addr);
        std::thread thread(&ThreadedServer::handle_request, this, new_user);
        thread.detach();
    }
}

void ThreadedServer::handle_request(User::user_ptr user) {
    /*!
     * @brief Handles income connections.
     * @param[in] user - connected user.
     *
     * Initially server tries to authorize the user. If authorization is OK,
     * user will be added in the set. Next, while connection is alive getting
     * data from user. When user disconnects, pulls it out from the set and
     * closes connection.
     */

    using namespace rapidjson;
    if (!authorize(user)) {
        user->close_connection();
        return;
    }

    user->set_nickname(user->get_message());

    std::cout << user->get_nickname() << " " <<
            user->get_address() << " connected" << std::endl;

    mutex.lock();
        users.insert(user);
    mutex.unlock();

    /// @todo All things below were made for testing and should be reimplemented!
    while (user->read_message() > 0) {
        std::cout << user->get_message() << std::endl;

        Document json;
        json.Parse(user->get_message().c_str());

        if (json.IsObject() && json.HasMember("method")) {
            auto method = json["method"].GetString();

            if (strcmp(method, API::METHODS::GET_ONLINE_USERS) == 0) {
                user->send_message(online_users());
                continue;
            }

            if (strcmp(method, API::METHODS::CREATE_ROOM) == 0 &&
                    json.HasMember("roommate"))
            {
                auto roommate = search_by_nickname(
                        json["roommate"].GetString());

                if (roommate != nullptr) {
                    user->set_roommate(roommate);
                }
                continue;
            }

            if (strcmp(method, API::METHODS::DESTROY_ROOM) == 0) {
                if (user->has_roomate()) {
                    user->detach_roommate();
                }
                continue;
            }


            if (strcmp(method, API::METHODS::SEND_TO_ROOMMATE) == 0) {
                if (user->has_roomate() && json.HasMember("msg")) {
                    user->send_to_roommate(json["msg"].GetString());
                }
            }
        }
    }

    mutex.lock();
        users.erase(user);
    mutex.unlock();

    user->close_connection();
}

bool ThreadedServer::authorize(User::user_ptr user) const {
    /*!
     * @brief Authorizes new user.
     * @param[in] user - connected user.
     *
     * Requests user nickname while it isn't unique.
     * If user disconnects on this stage function returns false.
     * If user provides unique nickname function returns true.
     */

    user->send_message("\nDream up nickname:");

    while (user->read_message() > 0 &&
            search_by_nickname(user->get_message()) != nullptr)
    {
        user->send_message("\n" + user->get_message() +
                           " allready exists.\nTry again:");
    }

    return user->get_message() != "";
}

User::user_ptr ThreadedServer::search_by_nickname(std::string nickname) const {
    /*!
     * @brief Searches user by nickname.
     * @param[in] nickname - target user.
     * @param[out] User::user_ptr - found user (nullptr if not found).
     */

    for (const auto& user : users) {
        if (user->get_nickname() == nickname) {
            return user;
        }
    }
    return nullptr;
}

std::string ThreadedServer::online_users() const {
    /*!
     * @brief Creates JSONified string that contains online users.
     */

    using namespace rapidjson;
    using std::string;

    Document response;
    response.Parse("{\"online_users\": []}");

    for (const auto& user : users) {
        SetValueByPointer(response, "/online_users/-", user->get_nickname().c_str());
    }

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    response.Accept(writer);
    std::string output = buffer.GetString();
    return output;
}

