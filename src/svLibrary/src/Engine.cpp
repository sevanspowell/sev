#include <cassert>

#include <sv/Engine.h>
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

bool Engine::initialize() {
    isInitialized = platform->initialize();
    return isInitialized;
}

uint32_t Engine::getMilliseconds() const {
    assert(isInitialized == true);
    return platform->getMilliseconds();
}
}
