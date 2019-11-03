#pragma once

#include "Types.hpp"

#include "json.hpp"

using nlohmann::json;

class QuitCommand;
class NameCommand;
class WhisperCommand;
class ListUsersCommand;

enum class Commands {
    NAME = 0,
    QUIT,
    WHISPER,
    JOIN,
    CREATE,
    LIST_USERS,
    UNKNOWN
};

class Command {
    private:
        Commands command;
    public:
        Command(Commands command);
        //virtual ~Command();
        Commands getCommandType();
        virtual Message serialize();
        static std::unique_ptr<Command> deserialize(Message message);
};

const std::string enumToString(Commands command);