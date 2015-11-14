#ifndef SERVER_API_HPP
#define SERVER_API_HPP

namespace API {
    namespace METHODS {
        static const char GET_ONLINE_USERS[] = "get_online_users";
        static const char CREATE_ROOM[] = "create_room";
        static const char DESTROY_ROOM[] = "destroy_room";
        static const char SEND_TO_ROOMMATE[] = "send_to_roommate";
    }
};

#endif //SERVER_API_HPP
