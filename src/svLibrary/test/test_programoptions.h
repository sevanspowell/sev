#include <cstdlib>

#include <sv/ProgramOptions.h>

TEST(ProgramOptions, IntOption) {
    int argc = 3;
    const char *argv[3];
    argv[0] = "quake";
    argv[1] = "--setAlpha";
    argv[2] = "50";

    sv::ProgramOptions options(argc, argv);
    int32_t optionIndex = options.checkOption("--setAlpha");
    EXPECT_EQ(1, optionIndex);

    const char *value = options.getOption(optionIndex + 1);
    EXPECT_TRUE(value != nullptr);

    EXPECT_EQ(50, atoi(value));
}

TEST(ProgramOptions, StringOption) {
    int argc = 5;
    const char *argv[5];
    argv[0] = "quake";
    argv[1] = "--setAlpha";
    argv[2] = "50";
    argv[3] = "--game";
    argv[4] = "ad";

    sv::ProgramOptions options(argc, argv);
    int32_t optionIndex = options.checkOption("--game");
    EXPECT_EQ(3, optionIndex);

    const char *value = options.getOption(optionIndex + 1);
    EXPECT_TRUE(value != nullptr);

    EXPECT_TRUE(strcmp(argv[4], value) == 0);
}
