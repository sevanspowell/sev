#include <cstdio>
#include <cstring>
#include <sstream>

#include <sv/console/Console.h>

extern "C" {
#include <sv/console/Commands.h>
#include <sv/console/Tokenizer.h>
}

namespace sv {
void Console::registerCommand(const std::string &name,
                              const std::shared_ptr<ConsoleCommand> &command) {
    consoleCommands[name] = command;
}

void Console::removeCommand(const std::string &name) {
    consoleCommands.erase(name);
}

bool Console::commandWithNameExists(const std::string &name) const {
    bool result = false;

    auto search = consoleCommands.find(name);
    // Command with name found
    if (search != consoleCommands.end()) {
        result = true;
    }

    return result;
}

void Console::appendToOutputBuffer(const std::string &outputStr) {
    outputBuffer += outputStr;
}

void Console::appendToErrorBuffer(const std::string &errorStr) {
    errorBuffer += errorStr;
}

const std::string &Console::getOutputBuffer() const { return outputBuffer; }

const std::string &Console::getErrorBuffer() const { return errorBuffer; }

void Console::appendString(const std::string &str) {
    inputBuffer += str;
} // TODO Make sure previous command
  // ends with a sequential
  // seperator?

bool Console::execute() {
    bool result = true;

    // Output and error buffers are cleared each time execute is called
    outputBuffer.clear();
    errorBuffer.clear();

    // Validate input buffer
    svTokenizerError tokenizerError;
    if (svTokenizerValidateInput(inputBuffer.c_str(), &tokenizerError) != 0) {
        // Error occurred
        switch (tokenizerError.errorCode) {
        case -1: {
            std::ostringstream errorMsg;
            errorMsg << "String not closed at position ";
            errorMsg << tokenizerError.characterPos << ".";
            errorBuffer += errorMsg.str();
            break;
        }
        default: { break; }
        }

        result = false;
    } else {
        // No error occurred during tokenization
        // Allocate tokens array
        unsigned int numTokens =
            svTokenizerGetNumTokens(inputBuffer.c_str(), NULL) +
            1; // NULL for error out because we already validated input above
        char **token = (char **)malloc(sizeof(char **) * numTokens);
        memset(token, '\0', sizeof(char *) * numTokens);
        svTokenizerTokenize(&inputBuffer[0], token, NULL);

        // Validate token array
        svCommandError commandError;
        if (svCommandsValidateTokenArray(token, &commandError) != 0) {
            // Error occurred
            switch (commandError.errorCode) {
            case -1: {
                // Two successive commands are separated by more than one
                // command seperator
                std::ostringstream errorMsg;
                errorMsg << "Two successive commands are separated by more "
                            "than one command separator:";
                errorMsg << " " << token[commandError.tokenPos - 1];
                errorMsg << " " << token[commandError.tokenPos];
                errorMsg << " " << token[commandError.tokenPos + 1];
                errorBuffer += errorMsg.str();
                break;
            }
            case -2: {
                std::ostringstream errorMsg;
                errorMsg << "First token is a command separator.";
                errorBuffer += errorMsg.str();
                break;
            }
            default:
                break;
            }

            result = false;
        } else {
            // No error occurred
            // Allocate space for commands, +1 for NULL command required.
            unsigned int numCommands = svCommandsGetNum(token, NULL) + 1;
            svCommand *commands =
                (svCommand *)malloc(sizeof(svCommand) * numCommands);
            memset(commands, '\0', sizeof(svCommand) * numCommands);
            svCommandsMakeNullCommand(commands + numCommands - 1);

            svCommandsSeparate(token, commands, NULL); // Already validate input

            // Process commands
            // For each command (ignoring NULL command): calculate argc, send
            // argv, argc to console command
            for (unsigned int i = 0; i < numCommands - 1; ++i) {
                svCommand *command = commands + i;

                auto search =
                    consoleCommands.find(std::string(command->argv[0]));
                if (search != consoleCommands.end()) {
                    // Command with name found
                    // Calc argc
                    int argc = 0;
                    for (int j = 0; command->argv[j] != NULL; ++j) {
                        ++argc;
                    }

                    result = search->second->execute(this, argc, command->argv);
                    // Don't execute any more commands
                    if (result == false) {
                        break;
                    }
                } else {
                    std::ostringstream errorMsg;
                    errorMsg << "No command with name '";
                    errorMsg << command->argv[0] << "'.";
                    errorBuffer += errorMsg.str();

                    result = false;
                    // Don't execute any more commands
                    break;
                }
            }

            svCommandsFreeContents(commands);
            free(commands);
        }

        free(token);
    }

    inputBuffer.clear();

    return result;
}

bool Console::executeString(const std::string &str) {
    inputBuffer.clear();
    outputBuffer.clear();
    errorBuffer.clear();

    appendString(str);
    bool result = execute();

    inputBuffer.clear();

    return result;
}
}
