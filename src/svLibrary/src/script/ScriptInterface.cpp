#include "sv/script/ScriptInterface.h"

namespace sv {
uint32_t ScriptInterface::engineGetMilliseconds() const {
    return engine->getMilliseconds();
}

bool ScriptInterface::consoleExecuteString(const std::string &str) {
    return console->executeString(str);
}

bool ScriptInterface::consoleExecute() { return console->execute(); }

void ScriptInterface::consoleRegisterCommand(
    const std::string &name, const std::shared_ptr<ConsoleCommand> &command) {
    console->registerCommand(name, command);
}

void ScriptInterface::consoleRemoveCommand(const std::string &name) {
    console->removeCommand(name);
}

bool ScriptInterface::consoleCommandWithNameExists(const std::string &name) {
    return console->commandWithNameExists(name);
}

void ScriptInterface::consoleAppendToOutputBuffer(
    const std::string &outputStr) {
    console->appendToOutputBuffer(outputStr);
}

void ScriptInterface::consoleAppendToErrorBuffer(const std::string &errorStr) {
    console->appendToErrorBuffer(errorStr);
}

void ScriptInterface::consoleAppendString(const std::string &str) {
    console->appendString(str);
}

const std::string &ScriptInterface::consoleGetOutputBuffer() const {
    return console->getOutputBuffer();
}

const std::string &ScriptInterface::consoleGetErrorBuffer() const {
    return console->getErrorBuffer();
}
}
