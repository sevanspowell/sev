#include <iostream>
#include <memory>

#include <sv/platform/SDL2Platform.h>

// Test we can initialize and use SDL2Platform
TEST(SDL2Platform, GetMilliseconds) {
    sv::SDL2Platform platform;

    EXPECT_TRUE(platform.initialize());
    EXPECT_TRUE(platform.getMilliseconds() >= 0);
}
