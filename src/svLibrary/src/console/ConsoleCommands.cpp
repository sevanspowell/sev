#include <sstream>

#include <sv/Common.h>
#include <sv/console/ConsoleCommands.h>

namespace sv {
bool BindCommand::execute(Console &console, int argc, char *argv[]) {
    bool result = false;

    if (argc != 3) {
        console.appendToErrorBuffer(
            "Usage: bind \"KEY_NAME\" \"COMMAND_STRING\"\n");
    } else {
        char *keyName   = sv::stripSurroundingQuotes(argv[1]);
        char *cmdString = sv::stripSurroundingQuotes(argv[2]);
        inputSystem.bind(keyName, cmdString);

        result = true;
    }

    return result;
}

bool UnbindAllCommand::execute(Console &console, int argc, char *argv[]) {
    bool result = false;

    if (argc != 1) {
        console.appendToErrorBuffer("Usage: unbindall\n");
    } else {
        inputSystem.clearAllBindings();
        result = true;
    }

    return result;
}

bool SetCommand::execute(Console &console, int argc, char *argv[]) {
    bool result = false;

    if (argc != 3) {
        console.appendToErrorBuffer(
            "Usage: set \"KEY_NAME\" \"VALUE_AS_STRING\"\n");
    } else {
        char *cvarName  = sv::stripSurroundingQuotes(argv[1]);
        char *cvarValue = sv::stripSurroundingQuotes(argv[2]);
        cvars.setStringValue(cvarName, cvarValue);
        result = true;
    }

    return result;
}

bool ExecCommand::execute(Console &console, int argc, char *argv[]) {
    bool result = false;

    if (argc != 2) {
        console.appendToErrorBuffer("Usage: exec \"file\"\n");
    } else {
        const char *fileToExecute = sv::stripSurroundingQuotes(argv[1]);

        Resource file(fileToExecute);
        std::shared_ptr<ResourceHandle> handle = resourceCache.getHandle(file);

        if (handle != nullptr) {
            // A resource buffer has no NULL-terminator, so get file size and
            // pass to string constructor
            const size_t fileSize = handle->getResourceSize();

            // File found
            result = console.executeString(
                std::string((char *)handle->getResourceBuffer(), fileSize));
        } else {
            // File not found
            std::stringstream err;
            err << "File not found: '" << fileToExecute << "'." << std::endl;
            console.appendToErrorBuffer(err.str());
        }
    }

    return result;
}

bool EchoCommand::execute(Console &console, int argc, char *argv[]) {
    bool result = false;

    if (argc != 2) {
        console.appendToErrorBuffer("Usage: echo \"msg\"\n");
    } else {
        const char *msg = sv::stripSurroundingQuotes(argv[1]);

        console.appendToOutputBuffer(msg);
        console.appendToOutputBuffer(std::string("\n"));

        result = true;
    }

    return result;
}
}
