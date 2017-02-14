#include <SDL2/SDL.h>

#include <sv/Globals.h>
#include <sv/platform/Keycodes.h>
#include <sv/platform/SDL2Platform.h>

namespace sv {
Keycodes::Enum convertSDL2KeyToPlatformKey(SDL_Keycode keyCode);
Keycodes::Enum convertSDL2MouseButtonToPlatformKey(uint8_t button);

bool SDL2Platform::initialize() {
    bool result = true;

    if (SDL_InitSubSystem(SDL_INIT_TIMER) != 0) {
        result = false;
        sv_globals::log(LogArea::Enum::PLATFORM, LogLevel::Enum::ERROR,
                        "SDL2: Failed to initialize timer subsystem.");
    } else if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        result = false;
        sv_globals::log(LogArea::Enum::PLATFORM, LogLevel::Enum::ERROR,
                        "SDL2: Failed to initialize video subsystem.");
    }

    return result;
}

void SDL2Platform::shutdown() { SDL_Quit(); }

uint32_t SDL2Platform::getMilliseconds() const { return SDL_GetTicks(); }

bool SDL2Platform::getNextEvent(PlatformEvent *event) {
    bool result = false;

    if (event != nullptr) {
        SDL_Event sdlEvent;

        while (SDL_PollEvent(&sdlEvent) != 0) {
            // There was an event
            result = true;

            // PlatformEvent event;
            // event = convertToPlatformEvent(sdlEvent);
            // queueEvent(event);
            switch (sdlEvent.type) {
            case SDL_KEYDOWN: {
                PlatformEvent keyDownEvent;

                keyDownEvent.time = sdlEvent.key.timestamp;
                keyDownEvent.type = PlatformEventType::Key;
                keyDownEvent.value1 =
                    convertSDL2KeyToPlatformKey(sdlEvent.key.keysym.sym);
                keyDownEvent.value2 = 1;

                queueEvent(keyDownEvent);
                break;
            }
            case SDL_KEYUP: {
                PlatformEvent keyUpEvent;

                keyUpEvent.time = sdlEvent.key.timestamp;
                keyUpEvent.type = PlatformEventType::Key;
                keyUpEvent.value1 =
                    convertSDL2KeyToPlatformKey(sdlEvent.key.keysym.sym);
                keyUpEvent.value2 = 0;

                queueEvent(keyUpEvent);
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                PlatformEvent mouseButtonDownEvent;

                mouseButtonDownEvent.time = sdlEvent.button.timestamp;
                mouseButtonDownEvent.type = PlatformEventType::Key;
                mouseButtonDownEvent.value1 =
                    convertSDL2MouseButtonToPlatformKey(sdlEvent.button.button);
                mouseButtonDownEvent.value2 = 1;

                queueEvent(mouseButtonDownEvent);
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                PlatformEvent mouseButtonUpEvent;

                mouseButtonUpEvent.time = sdlEvent.button.timestamp;
                mouseButtonUpEvent.type = PlatformEventType::Key;
                mouseButtonUpEvent.value1 =
                    convertSDL2MouseButtonToPlatformKey(sdlEvent.button.button);
                mouseButtonUpEvent.value2 = 0;

                queueEvent(mouseButtonUpEvent);
                break;
            }
            case SDL_MOUSEWHEEL: {
                PlatformEvent mouseWheelEvent;

                mouseWheelEvent.time   = sdlEvent.wheel.timestamp;
                mouseWheelEvent.type   = PlatformEventType::Key;
                mouseWheelEvent.value1 = sdlEvent.wheel.y > 0
                                             ? Keycodes::Enum::KEY_MWHEELDOWN
                                             : Keycodes::Enum::KEY_MWHEELUP;
                mouseWheelEvent.value2 = 1;

                queueEvent(mouseWheelEvent);

                // Queue 'stop' mouseWheelEvent
                mouseWheelEvent.value2 = 0;

                queueEvent(mouseWheelEvent);
                break;
            }
            case SDL_MOUSEMOTION: {
                PlatformEvent mouseMotionEvent;

                mouseMotionEvent.time   = sdlEvent.motion.timestamp;
                mouseMotionEvent.type   = PlatformEventType::Mouse;
                mouseMotionEvent.value1 = sdlEvent.motion.xrel;
                mouseMotionEvent.value2 = sdlEvent.motion.yrel;

                queueEvent(mouseMotionEvent);
                break;
            }
            case SDL_QUIT: {
                PlatformEvent quitEvent;

                quitEvent.time = sdlEvent.quit.timestamp;
                quitEvent.type = PlatformEventType::Quit;

                queueEvent(quitEvent);
                break;
            }
            default: { break; }
            }
        }

        if (!queue.empty()) {
            // An event exists in the queue
            result = true;

            *event = queue.front();
            queue.pop();
        }
    }

    return result;
}

Keycodes::Enum convertSDL2KeyToPlatformKey(SDL_Keycode keyCode) {
    // Platform keys map directly to SDL2 keys, so we can just do a type cast.
    Keycodes::Enum platformKeycode = static_cast<Keycodes::Enum>(keyCode);

    return platformKeycode;
}

Keycodes::Enum convertSDL2MouseButtonToPlatformKey(uint8_t button) {
    Keycodes::Enum key = Keycodes::Enum::KEY_MOUSE1;

    switch (button) {
    case SDL_BUTTON_LEFT: {
        key = Keycodes::Enum::KEY_MOUSE1;
        break;
    }
    case SDL_BUTTON_MIDDLE: {
        key = Keycodes::Enum::KEY_MOUSE2;
        break;
    }
    case SDL_BUTTON_RIGHT: {
        key = Keycodes::Enum::KEY_MOUSE3;
        break;
    }
    case SDL_BUTTON_X1: {
        key = Keycodes::Enum::KEY_MOUSE4;
        break;
    }
    case SDL_BUTTON_X2: {
        key = Keycodes::Enum::KEY_MOUSE5;
        break;
    }
    default: { break; }
    }

    return key;
}
}
