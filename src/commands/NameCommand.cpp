#include "commands/NameCommand.hpp"

NameCommand::NameCommand(std::string name)
: Command(Commands::NAME), name(name)
{
}

Message NameCommand::serialize() {
    json j;

    j["type"] = "command";
    j["command"] = Commands::NAME;
    j["name"] = this->name;

    return j.dump();
}

std::unique_ptr<NameCommand> NameCommand::deserialize(Message message) {
    auto parsed_message = json::parse(message);

    if (parsed_message["type"] == "command" && parsed_message["command"] == Commands::NAME) {
        return std::make_unique<NameCommand>(parsed_message["name"]);
    } else {
        throw "NAME command not valid";
    }
}

std::string NameCommand::getName() {
    return this->name;
}