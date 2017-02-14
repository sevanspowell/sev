#include <gtest/gtest.h>

#include <sv/platform/Keycodes.h>

// Test that correct string representations are produced for each keycode
TEST(Keycodes, getKeycodeString) {
    std::string expected;

    expected = "!";
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_EXCLAMATION));

    expected = "\"";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_DBLQUOTE));

    expected = "#";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_HASH));

    expected = "$";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_DOLLAR));

    expected = '%';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_PERCENT));

    expected = '&';
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_AMPERSAND));

    expected = '\'';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_QUOTE));

    expected = '(';
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_LEFTPAREN));

    expected = ')';
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_RIGHTPAREN));

    expected = '*';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_ASTERISK));

    expected = '+';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_PLUS));

    expected = ',';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_COMMA));

    expected = '-';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_MINUS));

    expected = '.';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_PERIOD));

    expected = '/';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_SLASH));

    expected = '0';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_0));

    expected = '1';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_1));

    expected = '2';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_2));

    expected = '3';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_3));

    expected = '4';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_4));

    expected = '5';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_5));

    expected = '6';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_6));

    expected = '7';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_7));

    expected = '8';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_8));

    expected = '9';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_9));

    expected = ':';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_COLON));

    expected = ';';
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_SEMICOLON));

    expected = '<';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_LESS));

    expected = '>';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_GREATER));

    expected = '?';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_QUESTION));

    expected = '@';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_AT));

    expected = '[';
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_LEFTBRACKET));

    expected = ']';
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_RIGHTBRACKET));

    expected = '^';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_CARET));

    expected = '_';
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_UNDERSCORE));

    expected = '`';
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_BACKQUOTE));

    expected = 'a';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_A));

    expected = 'b';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_B));

    expected = 'c';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_C));

    expected = 'd';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_D));

    expected = 'e';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_E));

    expected = 'f';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_F));

    expected = 'g';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_G));

    expected = 'h';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_H));

    expected = 'i';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_I));

    expected = 'j';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_J));

    expected = 'k';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_K));

    expected = 'l';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_L));

    expected = 'm';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_M));

    expected = 'n';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_N));

    expected = 'o';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_O));

    expected = 'p';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_P));

    expected = 'q';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_Q));

    expected = 'r';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_R));

    expected = 's';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_S));

    expected = 't';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_T));

    expected = 'u';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_U));

    expected = 'v';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_V));

    expected = 'w';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_W));

    expected = 'x';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_X));

    expected = 'y';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_Y));

    expected = 'z';
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_Z));

    expected = "RESERVED KEY - FIRST";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_FIRST));

    expected = "RESERVED KEY - LAST";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_LAST));

    expected = "Backspace";
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_BACKSPACE));

    expected = "Tab";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_TAB));

    expected = "Return";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_RETURN));

    expected = "Escape";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_ESCAPE));

    expected = "Spacebar";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_SPACEBAR));

    expected = "Delete";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_DELETE));

    expected = "CapsLock";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_CAPSLOCK));

    expected = "F1";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_F1));

    expected = "F2";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_F2));

    expected = "F3";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_F3));

    expected = "F4";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_F4));

    expected = "F5";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_F5));

    expected = "F6";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_F6));

    expected = "F7";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_F7));

    expected = "F8";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_F8));

    expected = "F9";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_F9));

    expected = "F10";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_F10));

    expected = "F11";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_F11));

    expected = "F12";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_F12));

    expected = "PrintScreen";
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_PRINTSCREEN));

    expected = "ScrollLock";
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_SCROLLLOCK));

    expected = "Pause";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_PAUSE));

    expected = "Insert";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_INSERT));

    expected = "Home";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_HOME));

    expected = "PageUp";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_PAGEUP));

    expected = "End";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_END));

    expected = "PageDown";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_PAGEDOWN));

    expected = "RightArrow";
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_RIGHTARROW));

    expected = "LeftArrow";
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_LEFTARROW));

    expected = "DownArrow";
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_DOWNARROW));

    expected = "UpArrow";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_UPARROW));

    expected = "LeftCtrl";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_LEFTCTRL));

    expected = "LeftShift";
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_LEFTSHIFT));

    expected = "LeftAlt";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_LEFTALT));

    expected = "RightCtrl";
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_RIGHTCTRL));

    expected = "RightShift";
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_RIGHTSHIFT));

    expected = "RightAlt";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_RIGHTALT));

    expected = "LeftGUI";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_LEFTGUI));

    expected = "Mouse1";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_MOUSE1));

    expected = "Mouse2";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_MOUSE2));

    expected = "Mouse3";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_MOUSE3));

    expected = "Mouse4";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_MOUSE4));

    expected = "Mouse5";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_MOUSE5));

    expected = "MWheelDown";
    EXPECT_EQ(expected,
              sv::getKeycodeString(sv::Keycodes::Enum::KEY_MWHEELDOWN));

    expected = "MWheelUp";
    EXPECT_EQ(expected, sv::getKeycodeString(sv::Keycodes::Enum::KEY_MWHEELUP));
}
