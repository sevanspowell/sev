#include <cstdio>
#include <cstring>

extern "C" {
#include <sv/console/Shell.h>
}

// Test that the error buffer is populated correctly when input line has string
// not closed
TEST(Shell, StringNotClosedErrorBuffer) {
    const char *str = "echo Wo'rld";
    EXPECT_TRUE(svShellExecute(str) != 0);

    size_t bufferSize = svShellGetErrorBufferSize();
    char *buffer      = (char *)malloc(bufferSize);
    svShellGetErrorBuffer(buffer);

    EXPECT_TRUE(strcmp("String not closed at position 7.", buffer) == 0);

    free(buffer);
}

// Test that the error buffer is populated correctly when input line has string
// not closed (long)
TEST(Shell, StringNotClosedErrorBufferLong) {
    const char *str = "echo World Test One Two Three Four Five Six'";
    EXPECT_TRUE(svShellExecute(str) != 0);

    size_t bufferSize = svShellGetErrorBufferSize();
    char *buffer      = (char *)malloc(bufferSize);
    svShellGetErrorBuffer(buffer);

    EXPECT_TRUE(strcmp("String not closed at position 43.", buffer) == 0);

    free(buffer);
}

// Test that the error buffer is populated correctly when input line has string
// not closed (extra long)
TEST(Shell, StringNotClosedErrorBufferExtraLong) {
    const char *str = "echo World Test One Two Three Four Five Six Seven Eight "
                      "Nine Ten Eleven Twelve Thirteen'";
    EXPECT_TRUE(svShellExecute(str) != 0);

    size_t bufferSize = svShellGetErrorBufferSize();
    char *buffer      = (char *)malloc(bufferSize);
    svShellGetErrorBuffer(buffer);

    EXPECT_TRUE(strcmp("String not closed at position 87.", buffer) == 0);

    free(buffer);
}

// Test that the error buffer is populated correctly when input line has a
// two successive command separators.
TEST(Shell, TwoSuccessiveCommandSeparatorsErrorBuffer) {
    const char *str = "echo World ; ;";
    EXPECT_TRUE(svShellExecute(str) != 0);

    size_t bufferSize = svShellGetErrorBufferSize();
    char *buffer      = (char *)malloc(bufferSize);
    svShellGetErrorBuffer(buffer);

    EXPECT_TRUE(strcmp("Two successive commands are separated by more than one "
                       "command separator: World ; ;",
                       buffer) == 0);

    free(buffer);
}

// Test that the error buffer is populated correctly when input line has a
// command separator as the first token.
TEST(Shell, FirstTokenIsCommandSeperatorErrorBuffer) {
    const char *str = "; echo World";
    EXPECT_TRUE(svShellExecute(str) != 0);

    size_t bufferSize = svShellGetErrorBufferSize();
    char *buffer      = (char *)malloc(bufferSize);
    svShellGetErrorBuffer(buffer);

    EXPECT_TRUE(strcmp("First token is a command separator.", buffer) == 0);

    free(buffer);
}

static int called = 0;
int exampleShellFunc(int argc, char **argv) {
    EXPECT_EQ(3, argc);
    EXPECT_TRUE(strcmp(argv[0], "test") == 0);
    EXPECT_TRUE(strcmp(argv[1], "hello") == 0);
    EXPECT_TRUE(strcmp(argv[2], "world") == 0);
    called = 1;

    return 0;
}

TEST(Shell, CommandExecution) {
    called = 0;
    EXPECT_EQ(0, svShellAddCommand("test", exampleShellFunc));
    EXPECT_EQ(0, svShellExecute("test hello world"));
    EXPECT_EQ(1, called);
    called = 0;
    svShellRemoveCommand("test");
}

TEST(Shell, CommandExecutionMultipleCommandsFail) {
    called = 0;
    EXPECT_EQ(0, svShellAddCommand("test", exampleShellFunc));
    EXPECT_TRUE(0 !=
                svShellExecute("test hello world ; test hello world ; fake "
                               "command ; command fake"));
    EXPECT_EQ(1, called);
    called = 0;
    svShellRemoveCommand("test");
}

TEST(Shell, RemoveCommand) {
    called = 0;
    EXPECT_EQ(0, svShellAddCommand("test", exampleShellFunc));
    EXPECT_EQ(0, svShellExecute("test hello world ; test hello world"));
    EXPECT_EQ(1, called);
    called = 0;
    svShellRemoveCommand("test");
    EXPECT_TRUE(0 != svShellExecute("test hello world ; test hello world"));
    EXPECT_EQ(0, called);
}

int overwriteMe(int argc, char **argv) { return 0; }

TEST(Shell, OverwriteCommand) {
    EXPECT_EQ(0, svShellAddCommand("overwrite", overwriteMe));
    EXPECT_EQ(-2, svShellAddCommand("overwrite", overwriteMe));
    svShellRemoveCommand("overwrite");
}

int writeOutputFunc(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        svShellPrintOutputBuffer("%s", argv[i]);
    }

    return 0;
}

TEST(Shell, WriteToOutputBufferTest) {
    EXPECT_EQ(0, svShellAddCommand("echo", writeOutputFunc));
    EXPECT_EQ(0, svShellExecute("echo \"Hello output buffer!\", Hey!"));
    char *outBuffer =
        (char *)malloc(sizeof(char) * svShellGetOutputBufferSize());
    svShellGetOutputBuffer(outBuffer);
    EXPECT_TRUE(strcmp(outBuffer, "\"Hello output buffer!\",Hey!") == 0);
    svShellRemoveCommand("echo");
}

int returnsOne(int argc, char **argv) {
    svShellPrintErrorBuffer("Command failed");
    return 1;
}

TEST(Shell, CommandReturnsNotZero) {
    EXPECT_EQ(0, svShellAddCommand("one", returnsOne));
    EXPECT_TRUE(svShellExecute("one") != 0);
    char *outBuffer =
        (char *)malloc(sizeof(char) * svShellGetErrorBufferSize());
    svShellGetErrorBuffer(outBuffer);
    EXPECT_TRUE(strcmp(outBuffer, "Command failed") == 0);
    svShellRemoveCommand("one");
}
