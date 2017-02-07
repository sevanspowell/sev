#include <SDL.h>

#include <sv/platform/SDL2Platform.h>

namespace sv {
bool SDL2Platform::initialize() {
    bool result = true;

    if (SDL_InitSubSystem(SDL_INIT_TIMER) != 0) {
        result = false;
        // TODO log error message
    }

    return result;
}

void SDL2Platform::shutdown() { SDL_Quit(); }

uint32_t SDL2Platform::getMilliseconds() const { return SDL_GetTicks(); }

PlatformEvent SDL2Platform::getNextEvent() { return PlatformEvent(); }

}
