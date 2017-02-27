#include <cassert>
#include <sstream>

#include <sv/Engine.h>
#include <sv/Globals.h>
#include <sv/ProgramOptions.h>
#include <sv/platform/SDL2Platform.h>

namespace sv {
Engine::Engine() {
    isInitialized = false;
    platform      = std::shared_ptr<Platform>(new SDL2Platform());
}

Engine::~Engine() {
    if (isInitialized == true) {
        platform->shutdown();
    }
}

bool Engine::initialize(int argc, char *argv[]) {
    isInitialized = true;

    ProgramOptions options(argc, (const char **)argv);

    // Find game to load
    // std::string game("default");
    // int32_t optionIndex = options.checkOption("--game");
    // if (optionIndex >= 0) {
    //     const char *gameOption = options.getOption(optionIndex + 1);
    //     if (gameOption != nullptr) {
    //         game = std::string(gameOption);
    //     }
    // }

    // Get config file path
    // std::stringstream configFilePath;
    // configFilePath << game << "/config.cfg" << std::endl;

    isInitialized &= client.initialize(options);

    isInitialized &= platform->initialize();

    return isInitialized;
}

uint32_t Engine::getMilliseconds() const {
    assert(isInitialized == true);
    return platform->getMilliseconds();
}
}
