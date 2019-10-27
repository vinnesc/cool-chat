#include "commands/QuitCommand.hpp"

QuitCommand::QuitCommand() : Command(Commands::QUIT)
{
}

Message QuitCommand::serialize() {
    json j;

    j["type"] = "command";
    j["command"] = Commands::QUIT;

    return j.dump();
}

QuitCommand* QuitCommand::deserialize(Message message) {
    auto parsed_message = json::parse(message);
    auto command = Commands::QUIT;

    if (parsed_message["type"] == "command" && parsed_message["command"] == command) {
        return new QuitCommand();
    } else {
        throw "QUIT command not valid";
    }
}