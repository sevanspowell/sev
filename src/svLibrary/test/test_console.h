#include <iostream>

#include <sv/console/Console.h>

// Test that the error buffer is populated correctly when input line has string
// not closed
TEST(Console, StringNotClosedErrorBuffer) {
    sv::Console console;
    console.appendString("echo Wo'rld");
    EXPECT_FALSE(console.execute());

    EXPECT_TRUE(std::string("String not closed at position 7.") ==
                console.getErrorBuffer());
}

// Test that the error buffer is populated correctly when input line has string
// not closed (long)
TEST(Console, StringNotClosedErrorBufferLong) {
    sv::Console console;
    const char *str = "echo World Test One Two Three Four Five Six'";
    EXPECT_FALSE(console.executeString(str));

    EXPECT_TRUE(std::string("String not closed at position 43.") ==
                console.getErrorBuffer());
}

// Test that the error buffer is populated correctly when input line has
// string not closed (extra long)
TEST(Console, StringNotClosedErrorBufferExtraLong) {
    sv::Console console;
    const char *str = "echo World Test One Two Three Four Five Six Seven Eight "
                      "Nine Ten Eleven Twelve Thirteen'";
    EXPECT_FALSE(console.executeString(str));

    EXPECT_TRUE(std::string("String not closed at position 87.") ==
                console.getErrorBuffer());
}

// Test that the error buffer is populated correctly when input line has a
// two successive command separators.
TEST(Console, TwoSuccessiveCommandSeparatorsErrorBuffer) {
    sv::Console console;
    const char *str = "echo World ; ;";
    EXPECT_FALSE(console.executeString(str));

    EXPECT_TRUE(std::string("Two successive commands are separated by more "
                            "than one command separator: World ; ;") ==
                console.getErrorBuffer());
}

// Test that the error buffer is populated correctly when input line has a
// command separator as the first token.
TEST(Console, FirstTokenIsCommandSeperatorErrorBuffer) {
    sv::Console console;
    const char *str = "; echo World";
    EXPECT_FALSE(console.executeString(str));

    EXPECT_TRUE(std::string("First token is a command separator.") ==
                console.getErrorBuffer());
}

// static int called = 0;
// int exampleShellFunc(int argc, char **argv) {
//     EXPECT_EQ(3, argc);
//     EXPECT_TRUE(strcmp(argv[0], "test") == 0);
//     EXPECT_TRUE(strcmp(argv[1], "hello") == 0);
//     EXPECT_TRUE(strcmp(argv[2], "world") == 0);
//     called = 1;

//     return 0;
// }
namespace sv {
class ExampleConsoleCmd : public ConsoleCommand {
  public:
    ExampleConsoleCmd() : called(false) {}

    virtual bool execute(Console &console, int argc, char *argv[]) {
        EXPECT_EQ(argc, 3);
        EXPECT_TRUE(strcmp(argv[0], "test") == 0);
        EXPECT_TRUE(strcmp(argv[1], "hello") == 0);
        EXPECT_TRUE(strcmp(argv[2], "world") == 0);
        called = true;

        return true;
    }

    bool called;
};
}

// Test simple execution of command
TEST(Console, CommandExecution) {
    sv::Console console;
    std::shared_ptr<sv::ExampleConsoleCmd> cmd(new sv::ExampleConsoleCmd);

    console.registerCommand("test", cmd);
    EXPECT_TRUE(console.executeString("test hello world"));
    EXPECT_TRUE(cmd->called);
}

// Test that command is still called if following command fails
TEST(Console, CommandExecutionMultipleCommandsFail) {
    sv::Console console;
    std::shared_ptr<sv::ExampleConsoleCmd> cmd(new sv::ExampleConsoleCmd);

    console.registerCommand("test", cmd);
    EXPECT_FALSE(console.executeString(
        "test hello world ; test hello world ; fake command ; command fake"));
    EXPECT_EQ(1, cmd->called);
}

// Test that removing commands by name works as expected
TEST(Console, RemoveCommand) {
    sv::Console console;
    std::shared_ptr<sv::ExampleConsoleCmd> cmd(new sv::ExampleConsoleCmd);

    console.registerCommand("test", cmd);
    EXPECT_TRUE(console.executeString("test hello world ; test hello world"));
    EXPECT_EQ(1, cmd->called);
    cmd->called = 0;
    console.removeCommand("test");
    EXPECT_FALSE(console.executeString("test hello world ; test hello world"));
    EXPECT_EQ(0, cmd->called);
}

namespace sv {
class OverwriteConsoleCmd : public ConsoleCommand {
  public:
    OverwriteConsoleCmd() : called(false) {}

    bool execute(Console &console, int argc, char *argv[]) {
        called = 1;
        return true;
    }

    bool called;
};
}

// Test that overwriting commands works as expected
TEST(Console, OverwriteCommand) {
    sv::Console console;
    std::shared_ptr<sv::ExampleConsoleCmd> cmd1(new sv::ExampleConsoleCmd);
    std::shared_ptr<sv::OverwriteConsoleCmd> cmd2(new sv::OverwriteConsoleCmd);

    console.registerCommand("test", cmd1);
    console.registerCommand("test", cmd2);
    EXPECT_TRUE(console.executeString("test hello world"));

    EXPECT_TRUE(cmd2->called);
    EXPECT_FALSE(cmd1->called);
}

namespace sv {
class EchoConsoleCmd : public ConsoleCommand {
  public:
    EchoConsoleCmd() {}

    bool execute(Console &console, int argc, char *argv[]) {
        for (int i = 1; i < argc; ++i) {
            console.appendToOutputBuffer(argv[i]);
            console.appendToErrorBuffer(argv[i]);
        }

        return true;
    }
};
}

// Test that writing to the output and error buffers works
TEST(Console, WriteToBufferTest) {
    sv::Console console;
    std::shared_ptr<sv::EchoConsoleCmd> cmd(new sv::EchoConsoleCmd);

    console.registerCommand("echo", cmd);
    EXPECT_TRUE(console.executeString("echo \"Hello output buffer!\", Hey"));
    EXPECT_TRUE(std::string("\"Hello output buffer!\",Hey") ==
                console.getOutputBuffer());
    EXPECT_TRUE(std::string("\"Hello output buffer!\",Hey") ==
                console.getErrorBuffer());
}

namespace sv {
class FailCmd : public ConsoleCommand {
    virtual bool execute(Console &console, int argc, char *argv[]) {
        return false;
    }
};
}

// Test a command that returns failure
TEST(Console, CommandReturnsFail) {
    sv::Console console;
    std::shared_ptr<sv::FailCmd> cmd(new sv::FailCmd);

    console.registerCommand("fail", cmd);
    EXPECT_FALSE(console.executeString("fail"));
    EXPECT_TRUE(std::string("") == console.getOutputBuffer());
    EXPECT_TRUE(std::string("") == console.getErrorBuffer());
}

TEST(Console, CommandWithNameExists) {
    sv::Console console;
    std::shared_ptr<sv::FailCmd> cmd(new sv::FailCmd);

    EXPECT_FALSE(console.commandWithNameExists("fail"));
    console.registerCommand("fail", cmd);
    EXPECT_TRUE(console.commandWithNameExists("fail"));
    console.removeCommand("fail");
    EXPECT_FALSE(console.commandWithNameExists("fail"));
}
