#pragma once

#include <sv/ClientVariables.h>
#include <sv/console/Console.h>
#include <sv/input/Input.h>

namespace sv {
class BindCommand : public ConsoleCommand {
  public:
    BindCommand(const std::shared_ptr<Input> inputSystem_)
        : inputSystem(inputSystem_) {}

    ///-------------------------------------------------------------------------
    /// \copydoc ConsoleCommand::execute()
    ///
    /// Usage: bind "KEY_NAME" "COMMAND STRING"
    ///-------------------------------------------------------------------------
    virtual bool execute(Console *const console, int argc, char *argv[]);

  private:
    std::shared_ptr<Input> inputSystem;
};

/// Used to unbind all keys
class UnbindAllCommand : public ConsoleCommand {
  public:
    UnbindAllCommand(const std::shared_ptr<Input> inputSystem_)
        : inputSystem(inputSystem_) {}

    ///-------------------------------------------------------------------------
    /// \copydoc ConsoleCommand::execute()
    ///
    /// Usage: unbindall
    ///-------------------------------------------------------------------------
    virtual bool execute(Console *const console, int argc, char *argv[]);

  private:
    std::shared_ptr<Input> inputSystem;
};

/// Used to set client variables
class SetCommand : public ConsoleCommand {
  public:
    SetCommand(const std::shared_ptr<ClientVariables> &cvars_)
        : cvars(cvars_) {}

    ///-------------------------------------------------------------------------
    /// \copydoc ConsoleCommand::execute()
    ///
    /// Usage: set "name" "value as string"
    ///-------------------------------------------------------------------------
    virtual bool execute(Console *const console, int argc, char *argv[]);

  private:
    const std::shared_ptr<ClientVariables> cvars;
};
}
