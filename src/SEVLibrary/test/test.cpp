#include <gtest/gtest.h>

TEST(Test, One) { EXPECT_EQ(1, 1); }

#include "test_tokenizer.h"
#include "test_commands.h"
#include "test_shell.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}