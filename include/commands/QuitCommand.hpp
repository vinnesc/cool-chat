#pragma once

#include "Command.hpp"

class QuitCommand : public Command {
    public:
        QuitCommand();
        Message serialize();
        static QuitCommand deserialize(Message message);
};