#include <sv/platform/Keycodes.h>

namespace sv {
std::string getKeycodeString(Keycodes::Enum keycode) {
    std::string str = "UNKNOWN KEY";

    // If printable character
    if (keycode >= Keycodes::Enum::KEY_EXCLAMATION &&
        keycode <= Keycodes::Enum::KEY_Z) {
        // Convert key to char and print
        str = std::string(1, (char)keycode);
    }
    // Non-printable or special characters
    else {
        switch (keycode) {
        case Keycodes::Enum::KEY_FIRST:
            str = "RESERVED KEY - FIRST";
            break;
        case Keycodes::Enum::KEY_LAST:
            str = "RESERVED KEY - LAST";
            break;
        case Keycodes::Enum::KEY_BACKSPACE:
            str = "Backspace";
            break;
        case Keycodes::Enum::KEY_TAB:
            str = "Tab";
            break;
        case Keycodes::Enum::KEY_RETURN:
            str = "Return";
            break;
        case Keycodes::Enum::KEY_ESCAPE:
            str = "Escape";
            break;
        case Keycodes::Enum::KEY_SPACEBAR:
            str = "Spacebar";
            break;
        case Keycodes::Enum::KEY_DELETE:
            str = "Delete";
            break;
        case Keycodes::Enum::KEY_CAPSLOCK:
            str = "CapsLock";
            break;
        case Keycodes::Enum::KEY_F1:
            str = "F1";
            break;
        case Keycodes::Enum::KEY_F2:
            str = "F2";
            break;
        case Keycodes::Enum::KEY_F3:
            str = "F3";
            break;
        case Keycodes::Enum::KEY_F4:
            str = "F4";
            break;
        case Keycodes::Enum::KEY_F5:
            str = "F5";
            break;
        case Keycodes::Enum::KEY_F6:
            str = "F6";
            break;
        case Keycodes::Enum::KEY_F7:
            str = "F7";
            break;
        case Keycodes::Enum::KEY_F8:
            str = "F8";
            break;
        case Keycodes::Enum::KEY_F9:
            str = "F9";
            break;
        case Keycodes::Enum::KEY_F10:
            str = "F10";
            break;
        case Keycodes::Enum::KEY_F11:
            str = "F11";
            break;
        case Keycodes::Enum::KEY_F12:
            str = "F12";
            break;
        case Keycodes::Enum::KEY_PRINTSCREEN:
            str = "PrintScreen";
            break;
        case Keycodes::Enum::KEY_SCROLLLOCK:
            str = "ScrollLock";
            break;
        case Keycodes::Enum::KEY_PAUSE:
            str = "Pause";
            break;
        case Keycodes::Enum::KEY_INSERT:
            str = "Insert";
            break;
        case Keycodes::Enum::KEY_HOME:
            str = "Home";
            break;
        case Keycodes::Enum::KEY_PAGEUP:
            str = "PageUp";
            break;
        case Keycodes::Enum::KEY_END:
            str = "End";
            break;
        case Keycodes::Enum::KEY_PAGEDOWN:
            str = "PageDown";
            break;
        case Keycodes::Enum::KEY_RIGHTARROW:
            str = "RightArrow";
            break;
        case Keycodes::Enum::KEY_LEFTARROW:
            str = "LeftArrow";
            break;
        case Keycodes::Enum::KEY_DOWNARROW:
            str = "DownArrow";
            break;
        case Keycodes::Enum::KEY_UPARROW:
            str = "UpArrow";
            break;
        case Keycodes::Enum::KEY_LEFTCTRL:
            str = "LeftCtrl";
            break;
        case Keycodes::Enum::KEY_LEFTSHIFT:
            str = "LeftShift";
            break;
        case Keycodes::Enum::KEY_LEFTALT:
            str = "LeftAlt";
            break;
        case Keycodes::Enum::KEY_RIGHTCTRL:
            str = "RightCtrl";
            break;
        case Keycodes::Enum::KEY_RIGHTSHIFT:
            str = "RightShift";
            break;
        case Keycodes::Enum::KEY_RIGHTALT:
            str = "RightAlt";
            break;
        case Keycodes::Enum::KEY_LEFTGUI:
            str = "LeftGUI";
            break;
        case Keycodes::Enum::KEY_MOUSE1:
            str = "Mouse1";
            break;
        case Keycodes::Enum::KEY_MOUSE2:
            str = "Mouse2";
            break;
        case Keycodes::Enum::KEY_MOUSE3:
            str = "Mouse3";
            break;
        case Keycodes::Enum::KEY_MOUSE4:
            str = "Mouse4";
            break;
        case Keycodes::Enum::KEY_MOUSE5:
            str = "Mouse5";
            break;
        case Keycodes::Enum::KEY_MWHEELDOWN:
            str = "MWheelDown";
            break;
        case Keycodes::Enum::KEY_MWHEELUP:
            str = "MWheelUp";
            break;
        default:
            break;
        }
    }

    return str;
}
}
