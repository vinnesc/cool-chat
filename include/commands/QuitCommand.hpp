#pragma once

#include "Command.hpp"

#define QUIT "quit"

class QuitCommand : public Command {
    public:
        Message serialize();
        void desearialize(Message payload);
};