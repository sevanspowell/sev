#include <memory>

#include <sv/ClientVariables.h>
#include <sv/Precision.h>
#include <sv/console/ConsoleCommands.h>

// Test that isFloat works correctly
TEST(ClientVariables, IsFloat) {
    EXPECT_TRUE(sv::isFloat("3.14"));
    EXPECT_TRUE(sv::isFloat("3"));
    EXPECT_FALSE(sv::isFloat("3.14 hello"));
    EXPECT_FALSE(sv::isFloat("pardoned 3.14"));
    EXPECT_FALSE(sv::isFloat("bubbles frozen in the ocean"));
    EXPECT_TRUE(sv::isFloat("10014.4"));
    EXPECT_FALSE(sv::isFloat("10014.4f"));
    EXPECT_FALSE(sv::isFloat("10014.4.4"));
    EXPECT_TRUE(sv::isFloat("9001.0000001"));
}

// Test that isInt works correctly
TEST(ClientVariables, IsInt) {
    EXPECT_TRUE(sv::isInt("3"));
    EXPECT_TRUE(sv::isInt("3199919"));
    EXPECT_TRUE(sv::isInt("838"));
    EXPECT_FALSE(sv::isInt("3.14"));
    EXPECT_FALSE(sv::isInt("2 goodbye"));
    EXPECT_FALSE(sv::isInt("2i"));
    EXPECT_FALSE(sv::isInt("random 9"));
    EXPECT_FALSE(sv::isInt("a inexplicable text string"));
    EXPECT_FALSE(sv::isInt("9.1.7"));
}

// Ensure value of a single float can be stored and retrieved successfully
TEST(ClientVariables, FloatValueSuccess) {
    sv::ClientVariables cvars;
    cvars.setFloatValue("r_farclip", 100.91f);
    float farClip = 100.0f;
    EXPECT_TRUE(cvars.getFloatValue("r_farclip", &farClip));
    EXPECT_TRUE(sv::floatsAreEqual(100.91f, farClip));

    cvars.setStringValue("r_farclip", "100.91");
    farClip = 100.0f;
    EXPECT_TRUE(cvars.getFloatValue("r_farclip", &farClip));
    EXPECT_TRUE(sv::floatsAreEqual(100.91f, farClip));
}

// Ensure value of float out stays same if get method fails.
TEST(ClientVariables, FloatValueFail) {
    sv::ClientVariables cvars;
    float farClip = 900.751f;
    EXPECT_FALSE(cvars.getFloatValue("r_farclip", &farClip));
    EXPECT_EQ(900.751f, farClip);
}

// Test that getting and setting float values works with more than one variable
// in the system.
TEST(ClientVariables, FloatValueMultiple) {
    sv::ClientVariables cvars;
    cvars.setFloatValue("r_farclip", 901.3f);
    cvars.setFloatValue("r_nearclip", 21.83f);

    float farClip  = 100.0f;
    float nearClip = 1.0f;
    EXPECT_TRUE(cvars.getFloatValue("r_farclip", &farClip));
    EXPECT_TRUE(cvars.getFloatValue("r_nearclip", &nearClip));

    EXPECT_TRUE(sv::floatsAreEqual(farClip, 901.3f));
    EXPECT_TRUE(sv::floatsAreEqual(nearClip, 21.83f));
}

// Ensure value of a single int can be stored and retrieved successfully
TEST(ClientVariables, IntValueSuccess) {
    sv::ClientVariables cvars;
    cvars.setIntValue("crosshair", 3);
    int crosshair = 1;
    EXPECT_TRUE(cvars.getIntValue("crosshair", &crosshair));
    EXPECT_TRUE(crosshair == 3);

    cvars.setStringValue("crosshair", "3");
    crosshair = 1;
    EXPECT_TRUE(cvars.getIntValue("crosshair", &crosshair));
    EXPECT_TRUE(crosshair == 3);
}

// Ensure value of int out stays same if get method fails.
TEST(ClientVariables, IntValueFail) {
    sv::ClientVariables cvars;
    int crosshair = 9;
    EXPECT_FALSE(cvars.getIntValue("crosshair", &crosshair));
    EXPECT_EQ(9, crosshair);
}

// Test that getting and setting int values works with more than one variable
// in the system.
TEST(ClientVariables, IntValueMultiple) {
    sv::ClientVariables cvars;
    cvars.setIntValue("skin", 38);
    cvars.setIntValue("wep", 12);

    int skin = 2;
    int wep  = 3;
    EXPECT_TRUE(cvars.getIntValue("skin", &skin));
    EXPECT_TRUE(cvars.getIntValue("wep", &wep));

    EXPECT_EQ(skin, 38);
    EXPECT_EQ(wep, 12);
}

// Ensure value of a single string can be stored and retrieved successfully
TEST(ClientVariables, StringValueSuccess) {
    sv::ClientVariables cvars;
    cvars.setStringValue("windowTitle", "Hello world1");
    std::string windowTitle = "My Window";
    EXPECT_TRUE(cvars.getStringValue("windowTitle", &windowTitle));
    EXPECT_EQ(windowTitle, std::string("Hello world1"));

    cvars.setStringValue("windowTitle", "New window");
    windowTitle = "My Window";
    EXPECT_TRUE(cvars.getStringValue("windowTitle", &windowTitle));
    EXPECT_EQ(windowTitle, std::string("New window"));
}

// Ensure value of string out stays same if get method fails.
TEST(ClientVariables, StringValueFail) {
    sv::ClientVariables cvars;
    std::string test = "Hello";
    EXPECT_FALSE(cvars.getStringValue("windowTitle", &test));
    EXPECT_EQ(test, std::string("Hello"));
}

// Test that getting and setting string values works with more than one variable
// in the system.
TEST(ClientVariables, StringValueMultiple) {
    sv::ClientVariables cvars;
    cvars.setStringValue("textureMode", "mipmapLinear");
    cvars.setStringValue("map", "e1m8.bsp");

    std::string textureMode = "noMipmaps";
    std::string map         = "title.bsp";
    EXPECT_TRUE(cvars.getStringValue("textureMode", &textureMode));
    EXPECT_TRUE(cvars.getStringValue("map", &map));

    EXPECT_EQ(textureMode, std::string("mipmapLinear"));
    EXPECT_EQ(map, std::string("e1m8.bsp"));
}

// Test that removing a variable works
TEST(ClientVariables, RemoveVariable) {
    sv::ClientVariables cvars;
    cvars.setStringValue("textureMode", "mipmapLinear");
    cvars.setStringValue("map", "e1m8.bsp");

    std::string textureMode = "noMipmaps";
    std::string map         = "title.bsp";
    cvars.removeVariable("map");
    EXPECT_TRUE(cvars.getStringValue("textureMode", &textureMode));
    EXPECT_FALSE(cvars.getStringValue("map", &map));

    EXPECT_EQ(textureMode, std::string("mipmapLinear"));
    EXPECT_EQ(map, std::string("title.bsp"));
}

// Test that using the 'set' console command works
TEST(ClientVariables, SetCmd) {
    sv::ClientVariables cvars;
    std::shared_ptr<sv::SetCommand> setCmd(new sv::SetCommand(cvars));

    sv::Console console;

    console.registerCommand("set", setCmd);

    EXPECT_TRUE(console.executeString("set \"sensitivity\" \"100.0\""));
    float sensitivity = 1.0f;
    EXPECT_TRUE(cvars.getFloatValue("sensitivity", &sensitivity));
    EXPECT_TRUE(sv::floatsAreEqual(100.0f, sensitivity));
}
