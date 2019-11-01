#pragma once

#include "Command.hpp"

class WhisperCommand : public Command {
    private:
        std::string sender;
        std::string receiver;
        Message message;
    public:
        WhisperCommand(std::string sender, std::string receiver, Message message);
        Message serialize();
        static WhisperCommand* deserialize(Message message);
        std::string getSender();
        std::string getReceiver();
        Message getMessage();
};