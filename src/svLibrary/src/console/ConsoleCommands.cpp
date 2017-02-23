#include <iostream>

#include <sv/Common.h>
#include <sv/console/ConsoleCommands.h>

namespace sv {
bool BindCommand::execute(Console *const console, int argc, char *argv[]) {
    bool result = false;

    if (argc != 3) {
        console->appendToOutputBuffer(
            "Usage: bind \"KEY_NAME\" \"COMMAND_STRING\"\n");
    } else {
        char *keyName   = sv::stripSurroundingQuotes(argv[1]);
        char *cmdString = sv::stripSurroundingQuotes(argv[2]);
        inputSystem->bind(keyName, cmdString);

        result = true;
    }

    return result;
}

bool UnbindAllCommand::execute(Console *const console, int argc, char *argv[]) {
    bool result = false;

    if (argc != 1) {
        console->appendToOutputBuffer("Usage: unbindall\n");
    } else {
        inputSystem->clearAllBindings();
        result = true;
    }

    return result;
}

bool SetCommand::execute(Console *const console, int argc, char *argv[]) {
    bool result = false;

    if (argc != 3) {
        console->appendToOutputBuffer(
            "Usage: set \"KEY_NAME\" \"VALUE_AS_STRING\"");
    } else {
        char *cvarName  = sv::stripSurroundingQuotes(argv[1]);
        char *cvarValue = sv::stripSurroundingQuotes(argv[2]);
        cvars->setStringValue(cvarName, cvarValue);
        result = true;
    }

    return result;
}
}
