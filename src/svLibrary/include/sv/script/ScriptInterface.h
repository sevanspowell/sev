//===-- sv/script/ScriptInteface.h - Game scripting methods -----*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines what methods the scripts can call and implements them by
/// delegating to appropriate classes.
///
/// The Script class uses the ScriptInterface to expose these methods to the
/// scripting language of it's choice. This class just exposes what methods may
/// be called by the script.
///
//===----------------------------------------------------------------------===//
#pragma once

#include <memory>

#include <sv/Engine.h>
#include <sv/console/Console.h>

namespace sv {
class ScriptInterface {
  public:
    ///-------------------------------------------------------------------------
    /// Create a script interface, all pointers to other systems must be ready
    /// to use (e.g. properly initialized).
    ///-------------------------------------------------------------------------
    ScriptInterface(const std::shared_ptr<Engine> &enginePtr,
                    const std::shared_ptr<Console> &consolePtr)
        : engine(enginePtr), console(consolePtr) {}

    ///-------------------------------------------------------------------------
    /// Get the number of milliseconds since some fixed point (such as OS start)
    /// for timing purposes.
    ///-------------------------------------------------------------------------
    uint32_t engineGetMilliseconds() const;

    bool consoleExecuteString(const std::string &str);
    bool consoleExecute();
    void consoleRegisterCommand(const std::string &name,
                                const std::shared_ptr<ConsoleCommand> &command);
    void consoleRemoveCommand(const std::string &name);
    bool consoleCommandWithNameExists(const std::string &name);
    void consoleAppendToOutputBuffer(const std::string &outputStr);
    void consoleAppendToErrorBuffer(const std::string &errorStr);
    void consoleAppendString(const std::string &str);
    const std::string &consoleGetOutputBuffer() const;
    const std::string &consoleGetErrorBuffer() const;

  private:
    std::shared_ptr<Engine> engine;
    std::shared_ptr<Console> console;
};
}
