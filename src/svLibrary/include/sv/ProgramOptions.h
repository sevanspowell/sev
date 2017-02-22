//===-- sv/ProgramOptions.h - Command-line arguments ------------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Used to parse and query command-line arguments to the program.
///
/// Use 'checkOption' to check the existence of a command-line argument and get
/// it's index in the argument list (if it exists).
///
/// Use 'getOption' to get an argument at a particular index in the
/// command-line argument list.
///
/// Typical usage:
///     ProgramOptions options(argc, argv);
///     int32_t index = options.checkOption("-setAlpha");
///     if (index >= 0) {
///         const char *arg = options.getOption(index + 1);
///         int alpha = atoi(arg);
///         // Do something with alpha value
///     }
///
//===----------------------------------------------------------------------===//
#pragma once

#include <cstdint>
#include <string>

namespace sv {
///-----------------------------------------------------------------------------
/// Class used to parse and query command-line arguments to the program.
///-----------------------------------------------------------------------------
class ProgramOptions {
  public:
    ///-------------------------------------------------------------------------
    /// Construct a ProgramOptions instance from command-line arguments from
    /// main.
    ///-------------------------------------------------------------------------
    ProgramOptions(int argc_, const char *argv_[]) : argc(argc_), argv(argv_) {}

    ///-------------------------------------------------------------------------
    /// Get the index of the given option string in the program argument list.
    ///
    /// \returns Index of given option string in the program argument list or -1
    /// if no option with that name.
    ///-------------------------------------------------------------------------
    int32_t checkOption(const std::string &option) const;

    ///-------------------------------------------------------------------------
    /// Get the option at the given index in the argument list.
    ///
    /// \returns Option at the given index or nullptr if no option at that index
    /// in the argument list.
    ///-------------------------------------------------------------------------
    const char *getOption(size_t index) const;

  private:
    int argc;
    const char **argv;
};
}
