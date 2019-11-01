#pragma once

#include "Command.hpp"

class QuitCommand : public Command {
    public:
        QuitCommand();
        Message serialize();
        static std::unique_ptr<QuitCommand> deserialize(Message message);
};