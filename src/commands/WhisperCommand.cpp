#include "commands/WhisperCommand.hpp"

WhisperCommand::WhisperCommand(std::string sender, std::string receiver, Message message) {
    this->command = Commands::WHISPER;
    this->sender = sender;
    this->receiver = receiver;
    this->message = message;
}

Message WhisperCommand::serialize() {
    json j;

    j["type"] = "command";
    j["command"] = this->command;
    j["from"] = this->sender;
    j["to"] = this->receiver;
    j["message"] = this->message;

    return j.dump();
}

WhisperCommand WhisperCommand::deserialize(Message message) {
    auto parsed_message = json::parse(message);
    auto command = enumToString(Commands::WHISPER);

    if (parsed_message["type"] == "command" && parsed_message["command"] == command) {
        return WhisperCommand(parsed_message["from"], parsed_message["to"], parsed_message["message"]);
    } else {
        throw "WHISPER command not valid";
    }
}
