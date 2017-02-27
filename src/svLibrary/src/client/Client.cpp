#include <iostream>
#include <sstream>

#include <sv/Globals.h>
#include <sv/client/Client.h>
#include <sv/console/ConsoleCommands.h>
#include <sv/resource/ResourceFolderPC.h>

namespace sv {
bool Client::initialize(const ProgramOptions &options) {
    bool result = false;

    if (resourceCache.initialize()) {
        // Find game to load - determines asset folder to load
        std::string game("default");
        int32_t optionIndex = options.checkOption("--game");
        if (optionIndex >= 0) {
            const char *gameOption = options.getOption(optionIndex + 1);
            if (gameOption != nullptr) {
                game = std::string(gameOption);
            }
        }

        // Load game asset folder
        if (resourceCache.registerResourceCollection(
                std::shared_ptr<ResourceFolderPC>(
                    new ResourceFolderPC(game)))) {
            // Create and register console commands
            std::shared_ptr<BindCommand> bindCmd(new BindCommand(input));
            std::shared_ptr<UnbindAllCommand> unbindAllCmd(
                new UnbindAllCommand(input));
            std::shared_ptr<SetCommand> setCmd(new SetCommand(cvars));
            std::shared_ptr<EchoCommand> echoCmd(new EchoCommand());
            std::shared_ptr<ExecCommand> execCmd(
                new ExecCommand(resourceCache));

            console.registerCommand("bind", bindCmd);
            console.registerCommand("unbindall", unbindAllCmd);
            console.registerCommand("set", setCmd);
            console.registerCommand("echo", echoCmd);
            console.registerCommand("exec", execCmd);

            // Load config file
            std::string configFilePath("config.cfg");
            std::stringstream execConfigFile;
            execConfigFile << "exec " << configFilePath;

            if (console.executeString(execConfigFile.str())) {
            } else {
                std::stringstream err;
                err << "Client failed to load config file '" << configFilePath
                    << "'.";
                sv_globals::log(LogArea::Enum::COMMON, LogLevel::Enum::WARNING,
                                err.str());
            }

            result = true;
        } else {
            std::stringstream err;
            err << "Client failed to load resource folder '" << game << "'.";
            sv_globals::log(LogArea::Enum::COMMON, LogLevel::Enum::ERROR,
                            err.str());
        }
    } else {
        sv_globals::log(LogArea::Enum::COMMON, LogLevel::Enum::ERROR,
                        "Client failed to initialize resource cache.");
    }

    // Create and register console commands
    // std::shared_ptr<BindCommand> bindCmd(new BindCommand(input));
    // std::shared_ptr<UnbindAllCommand> unbindAllCmd(new
    // UnbindAllCommand(input));
    // std::shared_ptr<SetCommand> setCmd(new SetCommand(cvars));

    // console.registerCommand("bind", bindCmd);
    // console.registerCommand("unbindall", unbindAllCmd);
    // console.registerCommand("set", setCmd);
    // console.registerCommand("exec", execCmd);

    // Execute config file
    // std::stringstream executeCfgStr;
    // executeCfgStr << "exec " << configFilePath;
    // if (console.executeString(executeCfgStr.str()) == false) {
    //     result = false;

    //     std::stringstream err;
    //     err << "Failed to execute config file: " << configFilePath <<
    //     std::endl;
    //     sv_globals::log(LogArea::Enum::COMMON, LogLevel::Enum::ERROR,
    //                     err.str());
    // }

    return result;
}
}
