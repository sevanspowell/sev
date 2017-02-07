/*===-- sev/console/Shell.h - Shell API -----------------------------*- C -*-===
 *
 *                  The Special Engine Variant Game Engine
 *
 * This file is distributed under the MIT License. See LICENSE.txt for details.
 *
 *===----------------------------------------------------------------------===*/
/**
 * \file
 * \brief A system that associates names with function pointers and calls the
 * appropriate function pointer when it encounters the name in an input line.
 *
 * An input line may be a series of one or more commands (and it's arguments).
 * Each occurence of command and it's arguments is separated by a ';' character
 * (if there are more than one such occurences). If there are more than one
 * command, each command will be executed sequentially from left to right. If
 * any command fails, following commands will not be executed.
 *
 * A typical input line might look something like this:
 *
 *     server start --port 9001 ; draw_ui --text "Connecting..." -x 0.8 -y 0.5 ;
 * exec
 *
 * That is:
 *     <command> (<arguments>) (; <command> (<arguments>) ; ...)
 * where elements in parentheses are optional.
 *
 * A maximum number of ... commands can be parsed, with each command being of
 * length less than ... characters (including the command itself and arguments).
 * If more that this number of commands is entered, no commands will be executed
 * and an error will be printed to the error buffer.
 *
 * The shell has no built-in commands. All commands are populated by the user.
 * The user may do this by ...
 * All functions must be of the signature: "int functionName(int argc, char
 * **argv)". Each function will be passed the arguments of it's associated
 * command in the standard way (e.g. how arguments are passed to "main" in a
 * typical C program). Functions must return 0 if the command completed
 * successfully and any other number if it did not. Because these functions
 * cannot use printf to print to the shell, the shell API provides the
 * following functions to print to the shell: ... They are identical in
 * signature to printf, one printing to the output buffer (for general, useful
 * output) and one printing to the error buffer (for errors). These two buffers
 * are cleared and reused before every call to "svShellExecute", so if you
 * are interested in the contents of these buffers, you should collect them via
 * ... before calling "svShellExecute" again.
 *
 * Each given command name maps to one function pointer. If two commands of the
 * same name are entered, the second will not be added.
 *
 * If any command in a series of commands entered returns an integer other than
 * 0 or cannot be found, no following commands will be executed.
 *===----------------------------------------------------------------------===*/
#pragma once

#include <stddef.h>

/**
 * Add a new command to the shell. The command will be given the name \p
 * commandName and when that name is found in an input line, the given \p
 * commandFunction will be executed. The arguments following the command (if
 * any) in the input line will be passed to the \p commandFunction in a standard
 * way (see argc, argv in "main").
 *
 * Adding a command with the same name as an existing command will fail.
 *
 * \note commandName must be a string literal/be around for as long as the
 * command may be used.
 *
 * \returns 0 if command added successfully, -1 if too many commands already, -2
 * if command with given \p command name already exists.
 */
int svShellAddCommand(const char *commandName,
                      int (*commandFunction)(int, char **));

/**
 * Remove a command with the given name \p commandName from the shell.
 */
void svShellRemoveCommand(const char *commandName);

/**
 * Parses an input line (which is made up of one or more commands and their
 * arguments, each command and it's arguments being separated by a ';' character
 * if there are more than one command).
 *
 * \pre \p inputLine must be a NULL-terminated string.
 *
 * \returns 0 if all commands executed successfully, other if any errors
 * occurred.
 */
int svShellExecute(const char *inputLine);

/**
 * Get the current size of the output buffer. This is useful for ensuring that
 * the buffer you provide to "svShellGetOutputBuffer" is of the correct
 * size.
 *
 * \returns Size of the output buffer in bytes.
 */
size_t svShellGetOutputBufferSize();

/**
 * Get the output buffer for the last set of commands executed.
 *
 * \pre \p outBuffer must have a sufficient number of bytes to store the
 * output buffer (see "svShellGetOutputBufferSize").
 *
 * \param [out] outBuffer Output buffer for the last set of commands is dumped
 * here.
 */
void svShellGetOutputBuffer(void *outBuffer);

/**
 * Get the current size of the error buffer. This is useful for ensuring that
 * the buffer you provide to "svShellGetErrorBuffer" is of the correct
 * size.
 *
 * \returns Size of the error buffer in bytes.
 */
size_t svShellGetErrorBufferSize();

/**
 * Get the error buffer for the last set of commands executed.
 *
 * \pre \p outBuffer must have a sufficient number of bytes to store the
 * error buffer (see "svShellGetErrorBufferSize").
 *
 * \param [out] outBuffer Error buffer for the last set of commands is dumped
 * here.
 */
void svShellGetErrorBuffer(void *outBuffer);

/**
 * Print to shell output buffer, function mimics printf.
 */
void svShellPrintOutputBuffer(const char *format, ...);

/**
 * Print to shell error buffer, function mimics printf.
 */
void svShellPrintErrorBuffer(const char *format, ...);
