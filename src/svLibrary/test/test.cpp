#include <gtest/gtest.h>

TEST(Test, One) { EXPECT_EQ(1, 1); }

#include "test_tokenizer.h"
#include "test_commands.h"
#include "test_shell.h"
#include "test_console.h"
#include "test_log.h"
#include "test_keycodes.h"
#include "test_sdl2platform.h"
#include "test_engine.h"
#include "test_scriptinterface.h"
#include "test_programoptions.h"
#include "test_input.h"
#include "test_clientvariables.h"
#include "test_common.h"
#include "test_datetime.h"
#include "test_resourcefolderpc.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
