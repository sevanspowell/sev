//===-- sv/input/Input.h - Input system -------------------------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief The input system is used to associate key names with command strings,
/// which can be executed by the console.
///
/// The input system also exposes some console commands that allow
/// binding/unbinding to be done from the console.
///
//===----------------------------------------------------------------------===//
#pragma once

#include <string>
#include <vector>

#include <sv/console/Console.h>

namespace sv {

/// Associates a key with a command string
struct Binding {
    Binding() : keyName(std::string("")), cmdString(std::string("")) {}
    Binding(const std::string &keyName_, const std::string &cmdString_)
        : keyName(keyName_), cmdString(cmdString_) {}

    std::string keyName;
    std::string cmdString;
};

typedef std::vector<Binding> Bindings;

/// Used to bind a key to a particular command string.
class BindCommand : public ConsoleCommand {
  public:
    BindCommand(const std::shared_ptr<Bindings> bindings_)
        : bindings(bindings_) {}

    ///-------------------------------------------------------------------------
    /// \copydoc ConsoleCommand::execute()
    ///
    /// Usage: bind "KEY_NAME" "COMMAND STRING"
    ///-------------------------------------------------------------------------
    virtual bool execute(Console *const console, int argc, char *argv[]);

  private:
    std::shared_ptr<Bindings> bindings;
};

/// Used to unbind all keys
class UnbindAllCommand : public ConsoleCommand {
  public:
    UnbindAllCommand(const std::shared_ptr<Bindings> bindings_)
        : bindings(bindings_) {}

    ///-------------------------------------------------------------------------
    /// \copydoc ConsoleCommand::execute()
    ///
    /// Usage: unbindall
    ///-------------------------------------------------------------------------
    virtual bool execute(Console *const console, int argc, char *argv[]);

  private:
    std::shared_ptr<Bindings> bindings;
};

class Input {
  public:
    Input()
        : bindings(new Bindings()), bindCmd(new BindCommand(bindings)),
          unbindAllCmd(new UnbindAllCommand(bindings)) {}

    ///-------------------------------------------------------------------------
    /// Get the command string associated with the given key name.
    ///
    /// \returns Command string associated with given key name or empty string
    /// if no binding exists for that key.
    ///-------------------------------------------------------------------------
    std::string getBinding(const std::string &keyName) const;

  private:
    std::shared_ptr<Bindings> bindings;

  public:
    std::shared_ptr<BindCommand> bindCmd;
    std::shared_ptr<UnbindAllCommand> unbindAllCmd;
};
}
