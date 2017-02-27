#pragma once

#include <sv/ClientVariables.h>
#include <sv/console/Console.h>
#include <sv/input/Input.h>
#include <sv/resource/ResourceCache.h>

namespace sv {
class BindCommand : public ConsoleCommand {
  public:
    BindCommand(Input &inputSystem_) : inputSystem(inputSystem_) {}

    ///-------------------------------------------------------------------------
    /// \copydoc ConsoleCommand::execute()
    ///
    /// Usage: bind "KEY_NAME" "COMMAND STRING"
    ///-------------------------------------------------------------------------
    virtual bool execute(Console &console, int argc, char *argv[]);

  private:
    Input &inputSystem;
};

/// Used to unbind all keys
class UnbindAllCommand : public ConsoleCommand {
  public:
    UnbindAllCommand(Input &inputSystem_) : inputSystem(inputSystem_) {}

    ///-------------------------------------------------------------------------
    /// \copydoc ConsoleCommand::execute()
    ///
    /// Usage: unbindall
    ///-------------------------------------------------------------------------
    virtual bool execute(Console &console, int argc, char *argv[]);

  private:
    Input &inputSystem;
};

/// Used to set client variables
class SetCommand : public ConsoleCommand {
  public:
    SetCommand(ClientVariables &cvars_) : cvars(cvars_) {}

    ///-------------------------------------------------------------------------
    /// \copydoc ConsoleCommand::execute()
    ///
    /// Usage: set "name" "value as string"
    ///-------------------------------------------------------------------------
    virtual bool execute(Console &console, int argc, char *argv[]);

  private:
    ClientVariables &cvars;
};

/// Used to execute script files
class ExecCommand : public ConsoleCommand {
  public:
    /// NOTE: ResourceCache must already be initialized and setup with resource
    /// collections before first time this command is executed.
    ExecCommand(ResourceCache &resourceCache_)
        : resourceCache(resourceCache_) {}

    ///-------------------------------------------------------------------------
    /// \copydoc ConsoleCommand::execute()
    ///
    /// Usage: exec "file"
    ///-------------------------------------------------------------------------
    virtual bool execute(Console &console, int argc, char *argv[]);

  private:
    ResourceCache &resourceCache;
};

/// Used to echo input messages to the output buffer of the console.
class EchoCommand : public ConsoleCommand {
  public:
    ///-------------------------------------------------------------------------
    /// \copydoc ConsoleCommand::execute()
    ///
    /// Usage: echo "msg"
    ///-------------------------------------------------------------------------
    virtual bool execute(Console &console, int argc, char *argv[]);
};
}
