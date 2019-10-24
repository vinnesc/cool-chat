#include "QuitCommand.hpp"

Message QuitCommand::serialize() {
    return QUIT;
}

void QuitCommand::desearialize(Message payload) {
    if (payload == QUIT) {
        this->type = quit;
    }
}