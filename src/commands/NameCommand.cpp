#include "commands/NameCommand.hpp"

NameCommand::NameCommand(Message message) {
    auto parsed_message = json::parse(message);
    auto command = enumToString(Commands::NAME);

    if (parsed_message["type"] == "command" && parsed_message["command"] == command) {
        this->command = Commands::NAME;
        this->name = parsed_message["name"];
    } else {
        throw "NAME command not valid";
    }
}

Message NameCommand::serialize() {
    json j;

    j["type"] = "command";
    j["command"] = this->command;
    j["name"] = this->name;

    return j.dump();
}

std::string NameCommand::getName() {
    return this->name;
}