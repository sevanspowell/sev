//===-- sv/platform/Platform.h - Operating System abstraction ---*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Operating system abstraction.
///
/// Operating system events (such as keypresses) are converted to an
/// intermediate format to be fed to the engine.
///
/// Based off Quake 3 Arena's operating system abstraction.
///
//===----------------------------------------------------------------------===//
#pragma once

#include <cstdint>
#include <memory>
#include <queue>

#include <sv/Singleton.h>

namespace sv {
enum class PlatformEventType {
    None = 0,  // No event, but time field is still valid
    Key,       // value1 is key code, value2 is down flag (1 for down)
    TextInput, // data is user's text input
    Mouse,     // value1 and value2 are relative, signed, x/y mouse movements
    // Packet,     // TODO
    Quit,
};

struct PlatformEvent {
    PlatformEvent()
        : time(0), type(PlatformEventType::None), value1(0), value2(0),
          data(nullptr) {}

    uint32_t time;
    PlatformEventType type;
    int value1, value2;
    std::shared_ptr<std::vector<char>> data;
};

/// Operating System abstraction
/// Is an abstract base class.
///
/// The 'initialize' method MUST be called before using the class and the
/// 'shutdown' method should be called when finished with the class.
class Platform {
  public:
    virtual ~Platform() {}

    ///-------------------------------------------------------------------------
    /// Initialize the platform system.
    ///
    /// \returns true on success, false otherwise.
    ///-------------------------------------------------------------------------
    virtual bool initialize() = 0;

    ///-------------------------------------------------------------------------
    /// Shutdown the platform system.
    ///
    /// Should be called when finished with platform system.
    ///-------------------------------------------------------------------------
    virtual void shutdown() = 0;

    ///-------------------------------------------------------------------------
    /// Get the number of milliseconds since some fixed point (such as OS start)
    /// for timing purposes.
    ///-------------------------------------------------------------------------
    virtual uint32_t getMilliseconds() const = 0;

    ///-------------------------------------------------------------------------
    /// Get oldest event on the event queue.
    ///
    /// \param   [out] event   If this method returns true, event is placed
    /// here.
    /// \returns True if there is an event to get, false if there is not.
    ///-------------------------------------------------------------------------
    virtual bool getNextEvent(PlatformEvent *event) = 0;

    ///-------------------------------------------------------------------------
    /// Push an event onto the queue
    ///-------------------------------------------------------------------------
    void queueEvent(const PlatformEvent &event);

    ///-------------------------------------------------------------------------
    /// Start interpreting keyboard events as text entry.
    ///-------------------------------------------------------------------------
    virtual void startTextInput() = 0;

    ///-------------------------------------------------------------------------
    /// Stop interpreting keyboard events as text entry.
    ///-------------------------------------------------------------------------
    virtual void stopTextInput() = 0;

    ///-------------------------------------------------------------------------
    /// Are we currently interpreting keyboard events as text entry?
    ///
    /// NOTE: text input starts INACTIVE after a call to Platform::initialize().
    ///
    /// \returns True if we are currently interpreting keyboard events as text
    /// entry, false if we aren't.
    ///-------------------------------------------------------------------------
    virtual bool isTextInputActive() const = 0;

    // static Platform &getSingleton();

    // static Platform *getSingletonPtr();

  protected:
    typedef std::queue<PlatformEvent> EventQueue;
    EventQueue queue;
};
}
