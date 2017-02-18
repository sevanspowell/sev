#include <iostream>

#include <sv/script/ScriptInterface.h>

// Test that the error buffer is populated correctly when input line has string
// not closed
TEST(ScriptInterfaceConsole, StringNotClosedErrorBuffer) {
    std::shared_ptr<sv::Console> consolePtr(new sv::Console());
    std::shared_ptr<sv::Engine> enginePtr(new sv::Engine());
    EXPECT_TRUE(enginePtr->initialize());
    sv::ScriptInterface console(enginePtr, consolePtr);
    console.consoleAppendString("echo Wo'rld");
    EXPECT_FALSE(console.consoleExecute());

    EXPECT_TRUE(std::string("String not closed at position 7.") ==
                console.consoleGetErrorBuffer());
}

// Test that the error buffer is populated correctly when input line has string
// not closed (long)
TEST(ScriptInterfaceConsole, StringNotClosedErrorBufferLong) {
    std::shared_ptr<sv::Console> consolePtr(new sv::Console());
    std::shared_ptr<sv::Engine> enginePtr(new sv::Engine());
    EXPECT_TRUE(enginePtr->initialize());
    sv::ScriptInterface console(enginePtr, consolePtr);
    const char *str = "echo World Test One Two Three Four Five Six'";
    EXPECT_FALSE(console.consoleExecuteString(str));

    EXPECT_TRUE(std::string("String not closed at position 43.") ==
                console.consoleGetErrorBuffer());
}

// Test that the error buffer is populated correctly when input line has
// string not closed (extra long)
TEST(ScriptInterfaceConsole, StringNotClosedErrorBufferExtraLong) {
    std::shared_ptr<sv::Console> consolePtr(new sv::Console());
    std::shared_ptr<sv::Engine> enginePtr(new sv::Engine());
    EXPECT_TRUE(enginePtr->initialize());
    sv::ScriptInterface console(enginePtr, consolePtr);
    const char *str = "echo World Test One Two Three Four Five Six Seven Eight "
                      "Nine Ten Eleven Twelve Thirteen'";
    EXPECT_FALSE(console.consoleExecuteString(str));

    EXPECT_TRUE(std::string("String not closed at position 87.") ==
                console.consoleGetErrorBuffer());
}

// Test that the error buffer is populated correctly when input line has a
// two successive command separators.
TEST(ScriptInterfaceConsole, TwoSuccessiveCommandSeparatorsErrorBuffer) {
    std::shared_ptr<sv::Console> consolePtr(new sv::Console());
    std::shared_ptr<sv::Engine> enginePtr(new sv::Engine());
    EXPECT_TRUE(enginePtr->initialize());
    sv::ScriptInterface console(enginePtr, consolePtr);
    const char *str = "echo World ; ;";
    EXPECT_FALSE(console.consoleExecuteString(str));

    EXPECT_TRUE(std::string("Two successive commands are separated by more "
                            "than one command separator: World ; ;") ==
                console.consoleGetErrorBuffer());
}

// Test that the error buffer is populated correctly when input line has a
// command separator as the first token.
TEST(ScriptInterfaceConsole, FirstTokenIsCommandSeperatorErrorBuffer) {
    std::shared_ptr<sv::Console> consolePtr(new sv::Console());
    std::shared_ptr<sv::Engine> enginePtr(new sv::Engine());
    EXPECT_TRUE(enginePtr->initialize());
    sv::ScriptInterface console(enginePtr, consolePtr);
    const char *str = "; echo World";
    EXPECT_FALSE(console.consoleExecuteString(str));

    EXPECT_TRUE(std::string("First token is a command separator.") ==
                console.consoleGetErrorBuffer());
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

// Test simple execution of command
TEST(ScriptInterfaceConsole, CommandExecution) {
    std::shared_ptr<sv::Console> consolePtr(new sv::Console());
    std::shared_ptr<sv::Engine> enginePtr(new sv::Engine());
    EXPECT_TRUE(enginePtr->initialize());
    sv::ScriptInterface console(enginePtr, consolePtr);
    std::shared_ptr<sv::ExampleConsoleCmd> cmd(new sv::ExampleConsoleCmd);

    console.consoleRegisterCommand("test", cmd);
    EXPECT_TRUE(console.consoleExecuteString("test hello world"));
    EXPECT_TRUE(cmd->called);
}

// Test that command is still called if following command fails
TEST(ScriptInterfaceConsole, CommandExecutionMultipleCommandsFail) {
    std::shared_ptr<sv::Console> consolePtr(new sv::Console());
    std::shared_ptr<sv::Engine> enginePtr(new sv::Engine());
    EXPECT_TRUE(enginePtr->initialize());
    sv::ScriptInterface console(enginePtr, consolePtr);
    std::shared_ptr<sv::ExampleConsoleCmd> cmd(new sv::ExampleConsoleCmd);

    console.consoleRegisterCommand("test", cmd);
    EXPECT_FALSE(console.consoleExecuteString(
        "test hello world ; test hello world ; fake command ; command fake"));
    EXPECT_EQ(1, cmd->called);
}

// Test that removing commands by name works as expected
TEST(ScriptInterfaceConsole, RemoveCommand) {
    std::shared_ptr<sv::Console> consolePtr(new sv::Console());
    std::shared_ptr<sv::Engine> enginePtr(new sv::Engine());
    EXPECT_TRUE(enginePtr->initialize());
    sv::ScriptInterface console(enginePtr, consolePtr);
    std::shared_ptr<sv::ExampleConsoleCmd> cmd(new sv::ExampleConsoleCmd);

    console.consoleRegisterCommand("test", cmd);
    EXPECT_TRUE(
        console.consoleExecuteString("test hello world ; test hello world"));
    EXPECT_EQ(1, cmd->called);
    cmd->called = 0;
    console.consoleRemoveCommand("test");
    EXPECT_FALSE(
        console.consoleExecuteString("test hello world ; test hello world"));
    EXPECT_EQ(0, cmd->called);
}

// Test that overwriting commands works as expected
TEST(ScriptInterfaceConsole, OverwriteCommand) {
    std::shared_ptr<sv::Console> consolePtr(new sv::Console());
    std::shared_ptr<sv::Engine> enginePtr(new sv::Engine());
    EXPECT_TRUE(enginePtr->initialize());
    sv::ScriptInterface console(enginePtr, consolePtr);
    std::shared_ptr<sv::ExampleConsoleCmd> cmd1(new sv::ExampleConsoleCmd);
    std::shared_ptr<sv::OverwriteConsoleCmd> cmd2(new sv::OverwriteConsoleCmd);

    console.consoleRegisterCommand("test", cmd1);
    console.consoleRegisterCommand("test", cmd2);
    EXPECT_TRUE(console.consoleExecuteString("test hello world"));

    EXPECT_TRUE(cmd2->called);
    EXPECT_FALSE(cmd1->called);
}

// Test that writing to the output and error buffers works
TEST(ScriptInterfaceConsole, WriteToBufferTest) {
    std::shared_ptr<sv::Console> consolePtr(new sv::Console());
    std::shared_ptr<sv::Engine> enginePtr(new sv::Engine());
    EXPECT_TRUE(enginePtr->initialize());
    sv::ScriptInterface console(enginePtr, consolePtr);
    std::shared_ptr<sv::EchoConsoleCmd> cmd(new sv::EchoConsoleCmd);

    console.consoleRegisterCommand("echo", cmd);
    EXPECT_TRUE(
        console.consoleExecuteString("echo \"Hello output buffer!\", Hey"));
    EXPECT_TRUE(std::string("\"Hello output buffer!\",Hey") ==
                console.consoleGetOutputBuffer());
    EXPECT_TRUE(std::string("\"Hello output buffer!\",Hey") ==
                console.consoleGetErrorBuffer());
}

// Test a command that returns failure
TEST(ScriptInterfaceConsole, CommandReturnsFail) {
    std::shared_ptr<sv::Console> consolePtr(new sv::Console());
    std::shared_ptr<sv::Engine> enginePtr(new sv::Engine());
    EXPECT_TRUE(enginePtr->initialize());
    sv::ScriptInterface console(enginePtr, consolePtr);
    std::shared_ptr<sv::FailCmd> cmd(new sv::FailCmd);

    console.consoleRegisterCommand("fail", cmd);
    EXPECT_FALSE(console.consoleExecuteString("fail"));
    EXPECT_TRUE(std::string("") == console.consoleGetOutputBuffer());
    EXPECT_TRUE(std::string("") == console.consoleGetErrorBuffer());
}

// Test that we can successfully the existance of a command
TEST(ScriptInterfaceConsole, CommandWithNameExists) {
    std::shared_ptr<sv::Console> consolePtr(new sv::Console());
    std::shared_ptr<sv::Engine> enginePtr(new sv::Engine());
    EXPECT_TRUE(enginePtr->initialize());
    sv::ScriptInterface console(enginePtr, consolePtr);
    std::shared_ptr<sv::FailCmd> cmd(new sv::FailCmd);

    EXPECT_FALSE(console.consoleCommandWithNameExists("fail"));
    console.consoleRegisterCommand("fail", cmd);
    EXPECT_TRUE(console.consoleCommandWithNameExists("fail"));
    console.consoleRemoveCommand("fail");
    EXPECT_FALSE(console.consoleCommandWithNameExists("fail"));
}
