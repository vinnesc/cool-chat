#pragma once

#include "Command.hpp"

class NameCommand : public Command {
    private:
        std::string name;
    public:
        NameCommand(Message message);
        std::string getName();
        Message serialize();
};