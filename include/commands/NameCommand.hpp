#pragma once

#include "Command.hpp"

class NameCommand : public Command {
    private:
        std::string name;
    public:
        NameCommand(std::string name);
        std::string getName();
        Message serialize();
        static NameCommand* deserialize(Message message);
};