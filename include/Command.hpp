#pragma once

#include "Types.hpp"

#include "json.hpp"

using nlohmann::json;

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
        virtual Message serialize();
        virtual Command deserialize(Message message);
};

const std::string enumToString(Commands command);