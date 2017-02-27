//===-- sv/script/LuaScript.h - Lua script ----------------------*- C++ -*-===//
//
//                 The Special Engine Variant Game Engine
//
// This file is distributed under the MIT License. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Lua script implementation.
///
//===----------------------------------------------------------------------===//
#pragma once

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <sv/script/Script.h>

namespace sv {
class LuaScript {
  public:
    LuaScript(const std::shared_ptr<ScriptInterface> &scriptInterfacePtr);

    /// \copydoc Script::initialize()
    virtual bool initialize();

    /// \copydoc Script::shutdown()
    virtual void shutdown();

    /// \copydoc Script::handleEvent()
    virtual void handleEvent(const PlatformEvent &event);

  private:
    lua_State *luaState;
};
}
