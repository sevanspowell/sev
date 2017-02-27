#include <sv/script/LuaScript.h>

namespace sv {
LuaScript::LuaScript(
    const std::shared_ptr<ScriptInterface> &scriptInterfacePtr) {
    Script(scriptInterfacePtr);
    luaState = nullptr;
}

bool LuaScript::initialize() {
    bool result = false;

    // Create lua state
    luaState = luaL_newstate();

    if (luaState) {
        // Register all functions as members of a table (Console table, Renderer
        // table)

        // Push scriptPtr onto lua stack, so free functions can access it.
    }

    return result;
}

void LuaScript::shutdown() {
    if (luaState != nullptr) {
        lua_close(luaState);
    }
}

void LuaScript::handleEvent(const PlatformEvent &event) {}
}
