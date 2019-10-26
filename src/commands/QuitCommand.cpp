#include "commands/QuitCommand.hpp"

QuitCommand::QuitCommand(Message message) {
    auto parsed_message = json::parse(message);
    auto command = enumToString(Commands::QUIT);

    if (parsed_message["type"] == "command" && parsed_message["command"] == command) {
        this->command = Commands::QUIT;
    } else {
        throw "QUIT command not valid";
    }
}

Message QuitCommand::serialize() {
    json j;

    j["type"] = "command";
    j["command"] = this->command;

    return j.dump();
}