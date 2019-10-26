#include "commands/WhisperCommand.hpp"

WhisperCommand::WhisperCommand(Message message) {
    auto parsed_message = json::parse(message);
    auto command = enumToString(Commands::WHISPER);

    if (parsed_message["type"] == "command" && parsed_message["command"] == command) {
        this->command = Commands::WHISPER;
        this->receiver = parsed_message["from"];
        this->sender = parsed_message["to"];
        this->message = parsed_message["message"];
    } else {
        throw "WHISPER command not valid";
    }
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