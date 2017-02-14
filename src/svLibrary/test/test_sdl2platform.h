#include <iostream>
#include <memory>

#include <sv/platform/SDL2Platform.h>

// Test we can initialize and use SDL2Platform to get the number of milliseconds
TEST(SDL2Platform, GetMilliseconds) {
    sv::SDL2Platform platform;

    EXPECT_TRUE(platform.initialize());
    EXPECT_TRUE(platform.getMilliseconds() >= 0);

    platform.shutdown();
}

// Example event loop testing 'getNextEvent'
TEST(SDL2Platform, ExampleEventLoop) {
    sv::SDL2Platform platform;

    EXPECT_TRUE(platform.initialize());

    while (true) {
        sv::PlatformEvent event;
        while (platform.getNextEvent(&event)) {
            std::cout << "An event occurred at time: " << event.time
                      << std::endl;

            switch (event.type) {
            case sv::PlatformEventType::Key: {
                std::cout << "\tKey event: "
                          << "'" << sv::getKeycodeString(
                                        (sv::Keycodes::Enum)event.value1)
                          << "' went " << (event.value2 == 1 ? "Down" : "Up")
                          << "." << std::endl;
                break;
            }
            case sv::PlatformEventType::Mouse: {
                std::cout << "\tMouse event: mouse moved x: " << event.value1
                          << " y: " << event.value2 << "." << std::endl;
                break;
            }
            case sv::PlatformEventType::Quit: {
                std::cout << "\tQuit event." << std::endl;
                break;
            }
            default: {
                std::cout << "\tUnkown event." << std::endl;
                break;
            }
            }
        }

        if (event.type == sv::PlatformEventType::Quit) {
            break;
        }
    }

    platform.shutdown();
}
