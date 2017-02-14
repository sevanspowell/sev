#include <gtest/gtest.h>

TEST(Test, One) { EXPECT_EQ(1, 1); }

#include "test_tokenizer.h"
#include "test_commands.h"
#include "test_shell.h"
#include "test_console.h"
#include "test_log.h"
#include "test_sdl2platform.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
