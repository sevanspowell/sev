//===-- sv/script/Script.h - Script base class ------------------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the interface that all scripting implementations must conform
/// to.
///
/// The script class is responsible for exposing the methods of the
/// ScriptInterface class to the chosen scripting language type and also for
/// providing init, update, shutdown and event hooks.
///
//===----------------------------------------------------------------------===//
#pragma once

#include <sv/script/ScriptInterface.h>

namespace sv {
class Script {
  public:
    ///-------------------------------------------------------------------------
    /// Script class constructor.
    ///-------------------------------------------------------------------------
    virtual Script(const std::shared_ptr<ScriptInterface> &scriptInterfacePtr)
        : scriptInterface(scriptInterfacePtr) {}

    virtual ~Script() {}

    ///-------------------------------------------------------------------------
    /// Initialize the script.
    ///
    /// \returns True if initialization successful, false otherwise.
    ///-------------------------------------------------------------------------
    virtual bool initialize() = 0;

    ///-------------------------------------------------------------------------
    /// Shutdown and free resources.
    ///-------------------------------------------------------------------------
    virtual void shutdown() = 0;

    ///-------------------------------------------------------------------------
    /// Allow a script to handle a platform event. 
    ///-------------------------------------------------------------------------
    virtual void handleEvent(const PlatformEvent &event) = 0;

  protected:
    std::shared_ptr<ScriptInterface> scriptInterface;
};
}
