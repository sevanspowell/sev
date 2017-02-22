#include <sv/input/Input.h>

TEST(Input, BindCmd) {
    sv::Input input;
    sv::Console console;

    console.registerCommand("bind", input.bindCmd);
    EXPECT_TRUE(console.executeString("bind KEY_W +forward"));
    EXPECT_EQ(std::string("+forward"), input.getBinding("KEY_W"));
}

TEST(Input, UnbindAllCmd) {
    sv::Input input;
    sv::Console console;

    console.registerCommand("bind", input.bindCmd);
    console.registerCommand("unbindall", input.unbindAllCmd);

    EXPECT_TRUE(console.executeString("bind KEY_W +forward"));
    EXPECT_TRUE(std::string("+forward") == input.getBinding("KEY_W"));

    EXPECT_TRUE(console.executeString("unbindall"));
    EXPECT_TRUE(std::string("") == input.getBinding("KEY_W"));
}
