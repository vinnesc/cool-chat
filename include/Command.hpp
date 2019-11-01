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