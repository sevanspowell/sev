#include <memory>

#include <sv/console/ConsoleCommands.h>
#include <sv/input/Input.h>

// Test const and non-const 'findBinding' methods
TEST(Input, FindBinding) {
    sv::Input input;

    input.bind("KEY_A", "+left");
    EXPECT_TRUE(input.findBinding("KEY_A") != nullptr);
    EXPECT_TRUE(input.findBinding("KEY_W") == nullptr);

    const sv::Input *inputConst = &input;
    EXPECT_TRUE(inputConst->findBinding("KEY_A") != nullptr);
    EXPECT_TRUE(inputConst->findBinding("KEY_W") == nullptr);
}

TEST(Input, GetBinding) {
    sv::Input input;

    input.bind("KEY_A", "+left");
    EXPECT_EQ(std::string("+left"), input.getBinding("KEY_A"));
}

TEST(Input, ClearAllBindings) {
    sv::Input input;

    input.bind("KEY_W", "+forward");
    EXPECT_EQ(std::string("+forward"), input.getBinding("KEY_W"));
    input.bind("KEY_A", "+left");
    EXPECT_EQ(std::string("+left"), input.getBinding("KEY_A"));

    input.clearAllBindings();
    EXPECT_EQ(std::string(""), input.getBinding("KEY_W"));
    EXPECT_EQ(std::string(""), input.getBinding("KEY_A"));
}

TEST(Input, BindCmd) {
    std::shared_ptr<sv::Input> input(new sv::Input);
    std::shared_ptr<sv::BindCommand> bindCmd(new sv::BindCommand(input));
    sv::Console console;

    console.registerCommand("bind", bindCmd);
    EXPECT_TRUE(console.executeString("bind KEY_W +forward"));
    EXPECT_EQ(std::string("+forward"), input->getBinding("KEY_W"));

    EXPECT_TRUE(console.executeString("bind \"KEY_W\" \"+left\""));
    EXPECT_EQ(std::string("+left"), input->getBinding("KEY_W"));
}

TEST(Input, UnbindAllCmd) {
    std::shared_ptr<sv::Input> input(new sv::Input);
    std::shared_ptr<sv::BindCommand> bindCmd(new sv::BindCommand(input));
    std::shared_ptr<sv::UnbindAllCommand> unbindAllCmd(
        new sv::UnbindAllCommand(input));
    sv::Console console;

    console.registerCommand("bind", bindCmd);
    console.registerCommand("unbindall", unbindAllCmd);

    EXPECT_TRUE(console.executeString("bind KEY_W +forward"));
    EXPECT_TRUE(std::string("+forward") == input->getBinding("KEY_W"));

    EXPECT_TRUE(console.executeString("unbindall"));
    EXPECT_TRUE(std::string("") == input->getBinding("KEY_W"));
}
