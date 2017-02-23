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

class Input {
  public:
    ///-------------------------------------------------------------------------
    /// Get the command string associated with the given key name.
    ///
    /// \returns Command string associated with given key name or empty string
    /// if no binding exists for that key.
    ///-------------------------------------------------------------------------
    std::string getBinding(const std::string &keyName) const;

    ///-------------------------------------------------------------------------
    /// Associate a key name with a command string.
    ///-------------------------------------------------------------------------
    void bind(const std::string &keyName, const std::string &commandStr);

    ///-------------------------------------------------------------------------
    /// \returns Pointer to binding with the given name or nullptr if not found.
    ///-------------------------------------------------------------------------
    Binding *findBinding(const std::string &keyName);

    ///-------------------------------------------------------------------------
    /// \returns Pointer to binding with the given name or nullptr if not found.
    ///-------------------------------------------------------------------------
    const Binding *findBinding(const std::string &keyName) const;

    ///-------------------------------------------------------------------------
    /// Remove all bindings from the input system.
    ///-------------------------------------------------------------------------
    void clearAllBindings();

  private:
    Bindings bindings;
};
}
