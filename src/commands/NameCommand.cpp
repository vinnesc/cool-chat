#include "commands/NameCommand.hpp"

NameCommand::NameCommand(std::string name) {
    this->command = Commands::NAME;
    this->name = name;
}

Message NameCommand::serialize() {
    json j;

    j["type"] = "command";
    j["command"] = this->command;
    j["name"] = this->name;

    return j.dump();
}

NameCommand NameCommand::deserialize(Message message) {
    auto parsed_message = json::parse(message);
    auto command = enumToString(Commands::NAME);

    if (parsed_message["type"] == "command" && parsed_message["command"] == command) {
        return NameCommand(parsed_message["name"]);
    } else {
        throw "NAME command not valid";
    }
}

std::string NameCommand::getName() {
    return this->name;
}