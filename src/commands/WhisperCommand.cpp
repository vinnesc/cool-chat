#include "commands/WhisperCommand.hpp"

WhisperCommand::WhisperCommand(std::string sender, std::string receiver, Message message)
: Command(Commands::WHISPER), sender(sender), receiver(receiver), message(message)
{
}

Message WhisperCommand::serialize() {
    json j;

    j["type"] = "command";
    j["command"] = Commands::WHISPER;
    j["from"] = this->sender;
    j["to"] = this->receiver;
    j["message"] = this->message;

    return j.dump();
}

WhisperCommand* WhisperCommand::deserialize(Message message) {
    auto parsed_message = json::parse(message);
    auto command = Commands::WHISPER;

    if (parsed_message["type"] == "command" && parsed_message["command"] == command) {
        return new WhisperCommand(parsed_message["from"], parsed_message["to"], parsed_message["message"]);
    } else {
        throw "WHISPER command not valid";
    }
}
