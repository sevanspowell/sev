#include <cstdio>
#include <cstring>

extern "C" {
#include <sv/console/commands.h>
}

// Ensure make null command works as expected
TEST(Commands, MakeNullCommand) {
    svCommand command;
    command.first = 100;
    command.last  = 0;
    command.sep   = "%";
    command.argv  = (char **)1000;

    svCommandsMakeNullCommand(&command);
    EXPECT_EQ(0, command.first);
    EXPECT_EQ(0, command.last);
    EXPECT_EQ(NULL, command.sep);
    EXPECT_EQ(NULL, command.argv);
}

// Ensure svCommandsValidateTokenArray errors if more than one command separator
// is used
TEST(Commands, ValidateMoreThanOneCommandSeparator) {
    const char *str = "echo \"Hello World\" ; ; exit";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(5, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[6];
    token[5] = NULL;
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));

    svCommandError error;
    EXPECT_TRUE(svCommandsValidateTokenArray(token, &error) != 0);
    EXPECT_EQ(-1, error.errorCode);
    EXPECT_EQ(2, error.tokenPos);
}

// Ensure svCommandsValidateTokenArray errors if first token is a command
// separator
TEST(Commands, ValidateFirstTokenIsCommandSeparator) {
    const char *str = "; echo \"Hello World\" ; exit";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(5, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[6];
    token[5]        = NULL;
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));

    svCommandError error;
    EXPECT_TRUE(svCommandsValidateTokenArray(token, &error) != 0);
    EXPECT_EQ(-2, error.errorCode);
    EXPECT_EQ(0, error.tokenPos);
}

// Ensure svCommandsValidateTokenArray works as expected with valid input
TEST(Commands, ValidateGoodInput) {
    const char *str = "echo \"Hello World today\" ; This is good";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(6, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[7];
    token[6]        = NULL;
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));

    EXPECT_EQ(0, svCommandsValidateTokenArray(token, NULL));
}

// Ensure svCommandsGetNum works if no commands are given
TEST(Commands, GetNumNone) {
    EXPECT_EQ(0, svCommandsGetNum(NULL, NULL));

    char *token[1];
    token[0] = NULL;
    EXPECT_EQ(0, svCommandsGetNum(token, NULL));
}

// Ensure svCommandsGetNum works with a simple example
TEST(Commands, GetNumValidInput) {
    const char *str = "echo \"Hello World today\" ; This is good";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(6, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[7];
    token[6]        = NULL;
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));

    EXPECT_EQ(0, svCommandsValidateTokenArray(token, NULL));
    EXPECT_EQ(2, svCommandsGetNum(token, NULL));
}

// Ensure svCommandsFreeContents works even if fed an empty or NULL array
TEST(Commands, FreeContentsEmptyArray) {
    svCommandsFreeContents(NULL);

    svCommand command[1];
    svCommandsMakeNullCommand(&command[0]);
    svCommandsFreeContents(command);
}

// Simple demonstration of tokenize->command pipeline
TEST(Commands, SimpleExample) {
    const char *str = "echo \"Hello World\"";
    size_t strLen   = strlen(str) + 1;
    char *inputLine = (char *)malloc(sizeof(char) * strLen);
    memset(inputLine, '\0', strLen);
    snprintf(inputLine, strLen, "%s", str);

    EXPECT_EQ(0, svTokenizerValidateInput(inputLine, NULL));
    EXPECT_EQ(2, svTokenizerGetNumTokens(inputLine, NULL));
    char *token[3];
    token[2]        = NULL;
    EXPECT_EQ(0, svTokenizerTokenize(inputLine, token, NULL));

    EXPECT_EQ(0, svCommandsValidateTokenArray(token, NULL));
    EXPECT_EQ(1, svCommandsGetNum(token, NULL));

    svCommand *commands = (svCommand *)malloc(
        sizeof(svCommand) * (svCommandsGetNum(token, NULL) + 1));
    svCommandsMakeNullCommand(commands + 1);

    EXPECT_EQ(0, svCommandsSeparate(token, commands, NULL));

    EXPECT_EQ(0, commands[0].first);
    EXPECT_EQ(1, commands[0].last);
    EXPECT_TRUE(strcmp(commands[0].sep, SV_SEP_SEQ) == 0);
    EXPECT_TRUE(strcmp(commands[0].argv[0], "echo") == 0);
    EXPECT_TRUE(strcmp(commands[0].argv[1], "\"Hello World\"") == 0);
    EXPECT_EQ(NULL, commands[0].argv[2]);

    svCommandsFreeContents(commands);
    free(commands);
}
