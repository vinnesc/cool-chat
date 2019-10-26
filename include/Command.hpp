#pragma once

#include "Types.hpp"

#include "json.hpp"

using nlohmann::json;

class QuitCommand;
class NameCommand;
class WhisperCommand;

enum class Commands {
    NAME = 0,
    QUIT,
    WHISPER,
    JOIN,
    CREATE,
    UNKNOWN
};

class Command {
    protected:
        Commands command;
    public:
        Commands getCommandType();
        virtual Message serialize();
        static Command* deserialize(Message message);
};

const std::string enumToString(Commands command);