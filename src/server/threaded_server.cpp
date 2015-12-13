#include "threaded_server.hpp"

using std::strcmp;

ThreadedServer::ThreadedServer(int port) :
        BaseServer::BaseServer(port)
{
    //TODO
}

ThreadedServer::~ThreadedServer()
{
    // TODO
}

void ThreadedServer::stop_server()
{
    // TODO
    BaseServer::stop_server();
}

void ThreadedServer::serve_forever()
{
    /*!
     * @brief Main server loop.
     *
     * In endless loop the #master_socket tries to accept new connections. If accept is OK,
     * creates new user and handles him in separate deamon threads.
     */

    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    while(true)
    {
        sockfd_t client_sock = accept(master_socket, (sockaddr *) &client_addr, &len);
        if (client_sock == -1)
        {
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

void ThreadedServer::handle_request(User::user_ptr user)
{
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
    if (!authorize(user))
    {
        user->close_connection();
        ///< if #user doesn't passed authorization than disconnect him
        return;
    }

    user->set_nickname(user->get_message());

    std::cout << user->get_nickname() << " " <<
            user->get_address() << " connected" << std::endl;

    send_to_all_users(
            api::user_connected(user->get_nickname())
                     );
    ///< Notifying online users about new incomer

    mutex.lock();
        users.insert(user);
    mutex.unlock();

    user->send_message(online_users());
    ///< Send to incomer all current online users

    /// @todo All things below were made for testing and should be reimplemented!
    while (user->read_message() > 0)
    {
        std::cout << user->get_message() << std::endl;

        Document json;
        json.Parse(user->get_message().c_str());

        if (json.IsObject() && json.HasMember("method"))
        {
            std::string method = json["method"].GetString();
            ///< Next checking method and invoking appropriate handler

            if (method == api::get_online_users)
            {
                user->send_message(online_users());
                continue;
            }

            if (method == api::create_room && json.HasMember("roommate"))
            {
                auto roommate = search_by_nickname(json["roommate"].GetString());

                if (roommate != nullptr)
                {
                    user->set_roommate(roommate);
                    user->send_message(api::room_created("succsess", roommate->get_nickname()));
                } else {
                    user->send_message(api::room_created("failed", "null"));
                }
                continue;
            }

            if (method == api::send_to_roommate)
            {
                if (user->has_roomate() && json.HasMember("msg"))
                {
                    user->send_to_roommate(json["msg"].GetString());
                }
                continue;
                // @TODO handle errors
            }

            if (method == api::send_to_user)
            {
                if (json.HasMember("user") && json.HasMember("msg"))
                {
                    auto receiver = json["user"].GetString();
                    auto msg = json["msg"].GetString();
                    User::user_ptr receiver_ptr = nullptr;
                    if ((receiver_ptr = search_by_nickname(receiver)) != nullptr)
                    {
                        receiver_ptr->send_message(
                                api::msg_from_user(user->get_nickname(), msg)
                                                  );
                    } else {
                        user->send_message("Ne dostavleno))0");
                    }
                }
                continue;
                //if (search_by_nickname())
            }

            //
            //if (strcmp(method, API::METHODS::DESTROY_ROOM) == 0)
            //{
            //    if (user->has_roomate())
            //    {
            //        user->detach_roommate();
            //    }
            //    continue;
            //}
        }
        user->send_message(
                api::api_error(user->get_message()));
    }

    mutex.lock();
        users.erase(user);
    mutex.unlock();

    send_to_all_users(
            api::user_disconnected(user->get_nickname())
                     );
    user->close_connection();
}

bool ThreadedServer::authorize(User::user_ptr user) const
{
    /*!
     * @brief Authorizes new user.
     * @param[in] user - connected user.
     *
     * Requests user nickname and check its uniqueness.
     * If user disconnects on this stage function returns false.
     * If user provides unique nickname function returns true.
     */

    user->read_message();
    auto nickname = user->get_message();
    if (!search_by_nickname(nickname))
    {
        user->send_message(api::authorized("succsess"));
        return true;
    } else {
        user->send_message(api::authorized("failed"));
        return false;
    }
}

User::user_ptr ThreadedServer::search_by_nickname(std::string nickname) const
{
    /*!
     * @brief Searches user by nickname.
     * @param[in] nickname - target user.
     * @param[out] User::user_ptr - found user (nullptr = not found).
     */

    for (const auto& user : users)
    {
        if (user->get_nickname() == nickname)
        {
            return user;
        }
    }
    return nullptr;
}

std::string ThreadedServer::online_users() const
{
    /*!
     * @brief Creates JSONified string that contains online users.
     */

    using namespace rapidjson;
    using std::string;

    Document response;
    response.Parse("{ \"online_users\": [] }");

    for (const auto& user : users)
    {
        SetValueByPointer(response, "/online_users/-", user->get_nickname().c_str());
    }

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    response.Accept(writer);
    std::string output = buffer.GetString();

    return output;
}

void ThreadedServer::send_to_all_users(std::string msg)
{
    for (const auto& user : users)
    {
        user->send_message(msg);
    }
}
