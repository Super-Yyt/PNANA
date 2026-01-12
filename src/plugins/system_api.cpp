#ifdef BUILD_LUA_SUPPORT

#include "plugins/system_api.h"
#include "utils/logger.h"
#include <cstdio>
#include <lua.hpp>
#include <memory>

namespace pnana {
namespace plugins {

// 在 Lua 注册表中存储API实例的键
static const char* API_REGISTRY_KEY = "pnana_system_api";

SystemAPI::SystemAPI() {}

SystemAPI::~SystemAPI() {}

void SystemAPI::registerFunctions(lua_State* L) {
    // 在注册表中存储API实例
    lua_pushlightuserdata(L, this);
    lua_setfield(L, LUA_REGISTRYINDEX, API_REGISTRY_KEY);

    // 注册到vim.fn表
    lua_getglobal(L, "vim");
    lua_getfield(L, -1, "fn");

    lua_pushcfunction(L, lua_fn_system);
    lua_setfield(L, -2, "system");

    lua_pop(L, 2); // 弹出vim和fn表

    // 注册全局函数
    lua_pushcfunction(L, lua_api_notify);
    lua_setglobal(L, "pnana_notify");

    lua_pushcfunction(L, lua_api_command);
    lua_setglobal(L, "pnana_command");

    lua_pushcfunction(L, lua_api_keymap);
    lua_setglobal(L, "pnana_keymap");

    lua_pushcfunction(L, lua_api_autocmd);
    lua_setglobal(L, "pnana_autocmd");

    // 创建便捷别名（类似 Neovim）
    // 直接在 Lua 栈上设置，更可靠
    lua_getglobal(L, "vim");
    lua_getglobal(L, "pnana_notify");
    lua_setfield(L, -2, "notify");
    lua_getglobal(L, "pnana_command");
    lua_setfield(L, -2, "cmd");
    lua_getglobal(L, "pnana_keymap");
    lua_setfield(L, -2, "keymap");
    lua_getglobal(L, "pnana_autocmd");
    lua_setfield(L, -2, "autocmd");
    lua_pop(L, 1);
}

SystemAPI* SystemAPI::getAPIFromLua(lua_State* L) {
    lua_getfield(L, LUA_REGISTRYINDEX, API_REGISTRY_KEY);
    SystemAPI* api = static_cast<SystemAPI*>(lua_touserdata(L, -1));
    lua_pop(L, 1);
    return api;
}

// vim.fn.system(command) -> string
int SystemAPI::lua_fn_system(lua_State* L) {
    const char* command = lua_tostring(L, 1);
    if (!command) {
        lua_pushnil(L);
        return 1;
    }

    FILE* pipe = popen(command, "r");
    if (!pipe) {
        lua_pushnil(L);
        return 1;
    }

    std::string result;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);

    lua_pushstring(L, result.c_str());
    return 1;
}

// pnana_notify(message, level)
int SystemAPI::lua_api_notify(lua_State* L) {
    const char* message = lua_tostring(L, 1);
    if (message) {
        LOG("Plugin: " + std::string(message));
    }
    return 0;
}

// pnana_command(name, callback)
int SystemAPI::lua_api_command(lua_State* L) {
    SystemAPI* api = getAPIFromLua(L);
    if (!api) {
        return 0;
    }

    const char* name = lua_tostring(L, 1);
    const char* callback = lua_tostring(L, 2);

    if (name && callback) {
        // TODO: 实现命令注册逻辑
        // api->registerCommand(std::string(name), std::string(callback));
    }

    return 0;
}

// pnana_keymap(mode, keys, callback)
int SystemAPI::lua_api_keymap(lua_State* L) {
    SystemAPI* api = getAPIFromLua(L);
    if (!api) {
        return 0;
    }

    const char* mode = lua_tostring(L, 1);
    const char* keys = lua_tostring(L, 2);
    const char* callback = lua_tostring(L, 3);

    if (mode && keys && callback) {
        // TODO: 实现键映射注册逻辑
        // api->registerKeymap(std::string(mode), std::string(keys), std::string(callback));
    }

    return 0;
}

// pnana_autocmd(event, callback)
int SystemAPI::lua_api_autocmd(lua_State* L) {
    SystemAPI* api = getAPIFromLua(L);
    if (!api) {
        return 0;
    }

    const char* event = lua_tostring(L, 1);
    if (!event) {
        return 0;
    }

    // TODO: 实现自动命令注册逻辑
    // 检查第二个参数是字符串还是函数
    // if (lua_isstring(L, 2)) {
    //     // 字符串回调
    //     const char* callback = lua_tostring(L, 2);
    //     if (callback) {
    //         api->registerEventListener(std::string(event), std::string(callback));
    //     }
    // } else if (lua_isfunction(L, 2)) {
    //     // 函数回调
    //     api->registerEventListenerFunction(std::string(event));
    // }

    return 0;
}

} // namespace plugins
} // namespace pnana

#endif // BUILD_LUA_SUPPORT
