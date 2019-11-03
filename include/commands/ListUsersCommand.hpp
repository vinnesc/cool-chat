#pragma once

#include "Command.hpp"

class ListUsersCommand : public Command {
    private:
        std::vector<std::string> users_names;
        bool response;
    public:
        ListUsersCommand();
        ListUsersCommand(const std::vector<std::string>& names);
        Message serialize();
        static std::unique_ptr<ListUsersCommand> deserialize(const Message message);
        std::vector<std::string> getUsers();
        void addUser(const std::string name);
};