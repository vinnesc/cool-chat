#pragma once

#include "Types.hpp"

enum Commands {
    name,
    quit,
    whisper,
    join,
    create
};

class Command {
    protected:
        Commands type;
    public:
        virtual Message serialize() = 0;
        virtual void deserialize() = 0;
};