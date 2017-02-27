//===-- sv/console/Console.h - Console class -------------------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Console system similiar to a OS shell or a dropdown console in a
/// game.
///
/// Associates ConsoleCommands (some sort of functionality) with names which
/// trigger the execution of those commands when those names are found in an
/// input buffer.
///
/// An input buffer may be a series of one or more commands (and it's
/// arguments). Each occurence of a command and it's optional argument's is
/// seperated by a ';' character. If there are more than one command in an input
/// line, each command will be executed sequentially from left to right. If any
/// command fails, the following commands will NOT be executed.
///
/// A typical input line might look something like this:
/// server start --port 9001 ; draw_ui --text "Connecting..." -x 0.8 -y 0.5
///
/// That is:
///     <command> (<arguments>) (; <command> (<arguments>) ; (...))
/// where elements in parentheses are optional. The last command need not have a
/// ';' separator at the end.
///
/// Each registered command name maps to one ConsoleCommand. If two commands of
/// the same name are entered, the second will overwrite the first.
///
//===----------------------------------------------------------------------===//
#pragma once

#include <map>
#include <memory>
#include <string>

namespace sv {
class Console;

/// Console command interface
class ConsoleCommand {
  public:
    virtual ~ConsoleCommand() {}

    ///-------------------------------------------------------------------------
    /// Called by the Console when this command's registered name is found in
    /// the input buffer of the console. Arguments found after the command are
    /// passed to this function in the same way arguments are passed to main. A
    /// reference to the console executing this command is given, so commands
    /// can print error and output messages to the console.
    ///
    /// \param   console   Console executing this command.
    /// \param   argc      Number of arguments.
    /// \param   argv      Array of arguments.
    /// \returns True if command executed successfully, false otherwise.
    ///-------------------------------------------------------------------------
    virtual bool execute(Console &console, int argc, char *argv[]) = 0;

    // Rollback changes of command execution.
    // virtual void rollback() = 0;
};

/// Console system
class Console {
  public:
    ///-------------------------------------------------------------------------
    /// Registers a command with the console. Console will execute the command
    /// if the command's name is found in the input buffer when a call to
    /// Console::execute() occurs. Anything following the command name before
    /// the next command or end of input will be passed to the console command
    /// in the same way arguments are passed to main.
    ///
    /// Will overwrite existing console command with the same name if one
    /// exists. Use 'commandWithNameExists' to check.
    ///-------------------------------------------------------------------------
    void registerCommand(const std::string &name,
                         const std::shared_ptr<ConsoleCommand> &command);

    ///-------------------------------------------------------------------------
    /// Remove the command with the given name from the console.
    ///-------------------------------------------------------------------------
    void removeCommand(const std::string &name);

    ///-------------------------------------------------------------------------
    /// \returns True if a command with the given name is already registered
    /// with the console.
    ///-------------------------------------------------------------------------
    bool commandWithNameExists(const std::string &name) const;

    ///-------------------------------------------------------------------------
    /// Append to the output buffer of this console. Should only be called by
    /// ConsoleCommands wanting to output useful information. Do not use this
    /// for errors, use 'appendToErrorBuffer' instead.
    ///
    /// \post No newlines or special characters are added to the given string.
    ///-------------------------------------------------------------------------
    void appendToOutputBuffer(const std::string &outputStr);

    ///-------------------------------------------------------------------------
    /// Append to the error buffer of this console. Should only be called by
    /// ConsoleCommands wanting to ouput errors. Reservere using this only for
    /// when an error occurs.
    ///
    /// \post No newlines or special characters are added to the given string.
    ///-------------------------------------------------------------------------
    void appendToErrorBuffer(const std::string &errorStr);

    ///-------------------------------------------------------------------------
    /// Get the output buffer of the console, where useful information from
    /// console commands is placed.
    ///-------------------------------------------------------------------------
    const std::string &getOutputBuffer() const;

    ///-------------------------------------------------------------------------
    /// Get the error buffer of the console, where errors from console commands
    /// are placed.
    ///
    /// \returns NULL-terminated string.
    ///-------------------------------------------------------------------------
    const std::string &getErrorBuffer() const;

    ///-------------------------------------------------------------------------
    /// Append a string to the to the console, to be executed when 'execute()'
    /// is
    /// called.
    ///-------------------------------------------------------------------------
    void appendString(const std::string &str);

    ///-------------------------------------------------------------------------
    /// Execute the input that has been added to the console's input buffer so
    /// far.
    ///
    /// NOTE: Each call to execute clears the output, error and input buffers.
    ///
    /// \returns True if all commands executed successfully, false otherwise.
    /// Note that execution stops at the first unsuccessful command.
    ///-------------------------------------------------------------------------
    bool execute();

    ///-------------------------------------------------------------------------
    /// Execute the given string immediately.
    ///
    /// NOTE: Each call to execute clears the output, error and input buffers.
    ///
    /// \returns True if all commands executed successfully, false otherwise.
    /// Note that execution stops at the first unsuccessful command.
    ///-------------------------------------------------------------------------
    bool executeString(const std::string &str);

  private:
    typedef std::map<std::string, std::shared_ptr<ConsoleCommand>>
        ConsoleCommandMap;
    ConsoleCommandMap consoleCommands;

    std::string inputBuffer;
    std::string outputBuffer;
    std::string errorBuffer;
};
}
