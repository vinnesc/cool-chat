#pragma once

#include "Command.hpp"

class NameCommand : public Command {
    private:
        std::string name;
    public:
        NameCommand(std::string name);
        std::string getName();
        Message serialize();
        static std::unique_ptr<NameCommand> deserialize(Message message);
};