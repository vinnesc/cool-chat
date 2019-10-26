#pragma once

#include "Command.hpp"

class QuitCommand : public Command {
    public:
        QuitCommand(Message message);
        Message serialize();
};