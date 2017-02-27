//===-- sv/Engine.h - Game engine entry point -------------------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Game engine entry point, responsible for creating and updating engine
/// components.
///
/// The game engine retrieves platform events from a Platform interface
/// implementation and pushes those events to the client and server.
///
//===----------------------------------------------------------------------===//
#pragma once
#pragma once

#include <sv/client/Client.h>
#include <sv/platform/Platform.h>

namespace sv {
class Engine {
  public:
    Engine();
    ~Engine();

    ///-------------------------------------------------------------------------
    /// Initialize the engine.
    ///
    /// Must be called before other method calls.
    ///
    /// \param argc Number of command-line arguments.
    /// \param argv Command-line arguments.
    /// \returns True if initialization was successful, false otherwise.
    ///-------------------------------------------------------------------------
    bool initialize(int argc = 0, char *argv[] = nullptr);

    ///-------------------------------------------------------------------------
    /// Get the number of milliseconds since some fixed point (such as OS start)
    /// for timing purposes.
    ///-------------------------------------------------------------------------
    uint32_t getMilliseconds() const;

  private:
    bool isInitialized;

    std::shared_ptr<Platform> platform;

    Client client;
};
}
