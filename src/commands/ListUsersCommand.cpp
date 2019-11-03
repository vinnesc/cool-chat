#include "commands/ListUsersCommand.hpp"
#include <iostream>

ListUsersCommand::ListUsersCommand(const std::vector<std::string>& names)
: Command(Commands::LIST_USERS), users_names(names), response(true)
{
}

ListUsersCommand::ListUsersCommand()
: Command(Commands::LIST_USERS), response(false)
{
}

Message ListUsersCommand::serialize() {
    json j;

    j["type"] = "command";
    j["command"] = Commands::LIST_USERS;
    j["response"] = true;
    j["users"] = this->users_names;
    return j.dump();
}

std::unique_ptr<ListUsersCommand> ListUsersCommand::deserialize(Message message) {
    auto parsed_message = json::parse(message);

    if (parsed_message["type"] == "command" && parsed_message["command"] == Commands::LIST_USERS) {
        if (parsed_message["response"]) {
            return std::make_unique<ListUsersCommand>(parsed_message["users"]);
        } else {
            return std::make_unique<ListUsersCommand>();
        }
    } else {
        throw "LIST_USERS command not valid";
    }
}