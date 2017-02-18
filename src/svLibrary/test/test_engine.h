#include <sv/Engine.h>

TEST(Engine, Initialize) {
    sv::Engine engine;

    EXPECT_TRUE(engine.initialize());
}

TEST(Engine, GetMilliseconds) {
    sv::Engine engine;

    EXPECT_TRUE(engine.initialize());
    EXPECT_TRUE(engine.getMilliseconds() >= 0);
}
