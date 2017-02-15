#include <iostream>
#include <memory>

#include <SDL2/SDL.h>

#include <sv/platform/SDL2Platform.h>

// Test we can initialize and use SDL2Platform to get the number of milliseconds
TEST(SDL2Platform, GetMilliseconds) {
    sv::SDL2Platform platform;

    EXPECT_TRUE(platform.initialize());
    EXPECT_TRUE(platform.getMilliseconds() >= 0);

    platform.shutdown();
}

// Test that we can successfully get text input
// TEST(SDL2Platform, TextInput) {
//     sv::SDL2Platform platform;

//     EXPECT_TRUE(platform.initialize());
//     EXPECT_FALSE(platform.isTextInputActive());

//     bool done = false;
//     while (!done) {
//         sv::PlatformEvent event;
//         while (platform.getNextEvent(&event)) {
//             switch (event.type) {
//             case sv::PlatformEventType::Quit: {
//                 done = true;
//                 break;
//             }
//             case sv::PlatformEventType::Key: {
//                 if ((sv::Keycodes::Enum)event.value1 ==
//                         sv::Keycodes::Enum::KEY_BACKQUOTE &&
//                     event.value2 == 0) {
//                     if (platform.isTextInputActive() == false) {
//                         platform.startTextInput();
//                         EXPECT_TRUE(platform.isTextInputActive());
//                     } else {
//                         platform.stopTextInput();
//                         EXPECT_FALSE(platform.isTextInputActive());
//                     }
//                 }
//                 break;
//             }
//             case sv::PlatformEventType::TextInput: {
//                 if (event.data != nullptr) {
//                     std::string str(event.data->begin(), event.data->end());
//                     std::cout << "Text Received: " << str << std::endl;
//                 }
//                 break;
//             }
//             default: { break; }
//             }
//         }
//     }

//     platform.stopTextInput();
//     EXPECT_FALSE(platform.isTextInputActive());

//     platform.shutdown();
// }

// Example event loop testing 'getNextEvent'
// TEST(SDL2Platform, ExampleEventLoop) {
//     sv::SDL2Platform platform;

//     EXPECT_TRUE(platform.initialize());

//     SDL_InitSubSystem(SDL_INIT_VIDEO);

//     SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
//     SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
//     SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
//     SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

//     SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
//     SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

//     SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

//     uint32_t flags = 0x0;
//     flags |= SDL_WINDOW_OPENGL;
//     flags |= SDL_WINDOW_RESIZABLE;

//     // Create window
//     SDL_Window *window =
//         SDL_CreateWindow("Test bed", SDL_WINDOWPOS_CENTERED,
//                          SDL_WINDOWPOS_CENTERED, 800, 600, flags);

//     while (true) {
//         sv::PlatformEvent event;
//         while (platform.getNextEvent(&event)) {
//             std::cout << "An event occurred at time: " << event.time
//                       << std::endl;

//             switch (event.type) {
//             case sv::PlatformEventType::Key: {
//                 std::cout << "\tKey event: "
//                           << "'" << sv::getKeycodeString(
//                                         (sv::Keycodes::Enum)event.value1)
//                           << "' went " << (event.value2 == 1 ? "Down" :
//                           "Up")
//                           << "." << std::endl;
//                 break;
//             }
//             case sv::PlatformEventType::Mouse: {
//                 std::cout << "\tMouse event: mouse moved x: " <<
//                 event.value1
//                           << " y: " << event.value2 << "." << std::endl;
//                 break;
//             }
//             case sv::PlatformEventType::Quit: {
//                 std::cout << "\tQuit event." << std::endl;
//                 break;
//             }
//             default: {
//                 std::cout << "\tUnkown event." << std::endl;
//                 break;
//             }
//             }
//         }

//         if (event.type == sv::PlatformEventType::Quit) {
//             break;
//         }
//     }

//     platform.shutdown();
// }
