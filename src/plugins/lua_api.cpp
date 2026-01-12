#ifdef BUILD_LUA_SUPPORT

#include "plugins/lua_api.h"
#include "core/document.h"
#include "core/editor.h"
#include "utils/logger.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

namespace pnana {
namespace plugins {

// 在 Lua 注册表中存储编辑器指针的键
static const char* EDITOR_REGISTRY_KEY = "pnana_editor";
static const char* API_REGISTRY_KEY = "pnana_api";

LuaAPI::LuaAPI(core::Editor* editor) : editor_(editor), engine_(nullptr) {
    // 初始化各个API组件
    editor_api_ = std::make_unique<EditorAPI>(editor);
    file_api_ = std::make_unique<FileAPI>(editor);
    theme_api_ = std::make_unique<ThemeAPI>(editor);
    system_api_ = std::make_unique<SystemAPI>();
}

LuaAPI::~LuaAPI() {}

void LuaAPI::initialize(LuaEngine* engine) {
    engine_ = engine;
    if (!engine_ || !engine_->getState()) {
        LOG_ERROR("LuaAPI: Engine not initialized");
        return;
    }

    lua_State* L = engine_->getState();

    // 在注册表中存储 API 实例
    lua_pushlightuserdata(L, this);
    lua_setfield(L, LUA_REGISTRYINDEX, API_REGISTRY_KEY);

    // 在注册表中存储编辑器指针
    lua_pushlightuserdata(L, editor_);
    lua_setfield(L, LUA_REGISTRYINDEX, EDITOR_REGISTRY_KEY);

    // 创建 vim 全局表
    engine_->createTable("vim");

    // 创建嵌套表（自动处理）
    engine_->createNestedTable("vim.api");
    engine_->createNestedTable("vim.fn");

    // 初始化各个API组件
    editor_api_->registerFunctions(L);
    file_api_->registerFunctions(L);
    theme_api_->registerFunctions(L);
    system_api_->registerFunctions(L);
}

core::Editor* LuaAPI::getEditorFromLua(lua_State* L) {
    lua_getfield(L, LUA_REGISTRYINDEX, EDITOR_REGISTRY_KEY);
    core::Editor* editor = static_cast<core::Editor*>(lua_touserdata(L, -1));
    lua_pop(L, 1);
    return editor;
}

LuaAPI* LuaAPI::getAPIFromLua(lua_State* L) {
    lua_getfield(L, LUA_REGISTRYINDEX, API_REGISTRY_KEY);
    LuaAPI* api = static_cast<LuaAPI*>(lua_touserdata(L, -1));
    lua_pop(L, 1);
    return api;
}

void LuaAPI::triggerEvent(const std::string& event, const std::vector<std::string>& args) {
    LOG("LuaAPI::triggerEvent: triggering event '" + event + "' with " +
        std::to_string(args.size()) + " args");

    if (!engine_) {
        LOG("LuaAPI::triggerEvent: engine is null");
        return;
    }

    lua_State* L = engine_->getState();

    // 处理字符串回调
    auto it = event_listeners_.find(event);
    if (it != event_listeners_.end()) {
        for (const auto& callback : it->second) {
            lua_getglobal(L, callback.c_str());
            if (lua_isfunction(L, -1)) {
                // 推送参数
                for (const auto& arg : args) {
                    lua_pushstring(L, arg.c_str());
                }

                int result = lua_pcall(L, static_cast<int>(args.size()), 0, 0);
                if (result != LUA_OK) {
                    const char* error = lua_tostring(L, -1);
                    LOG_ERROR("Event callback error: " + std::string(error));
                    lua_pop(L, 1);
                }
            } else {
                lua_pop(L, 1);
            }
        }
    }

    // 处理函数引用回调
    auto func_it = event_function_listeners_.find(event);
    if (func_it != event_function_listeners_.end()) {
        for (int ref : func_it->second) {
            lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
            if (lua_isfunction(L, -1)) {
                // 推送参数
                for (const auto& arg : args) {
                    lua_pushstring(L, arg.c_str());
                }

                int result = lua_pcall(L, static_cast<int>(args.size()), 0, 0);
                if (result != LUA_OK) {
                    const char* error = lua_tostring(L, -1);
                    LOG_ERROR("Event function callback error: " + std::string(error));
                    lua_pop(L, 1);
                }
            } else {
                lua_pop(L, 1);
            }
        }
    }
}

void LuaAPI::registerEventListener(const std::string& event, const std::string& callback) {
    event_listeners_[event].push_back(callback);
}

void LuaAPI::registerEventListenerFunction(const std::string& event) {
    if (!engine_ || !engine_->getState()) {
        return;
    }

    lua_State* L = engine_->getState();
    // 函数在栈顶（位置 2），复制到栈顶
    lua_pushvalue(L, 2);
    // 创建引用
    int ref = luaL_ref(L, LUA_REGISTRYINDEX);
    event_function_listeners_[event].push_back(ref);
}

void LuaAPI::registerCommand(const std::string& name, const std::string& callback) {
    commands_[name] = callback;
}

void LuaAPI::registerKeymap(const std::string& mode, const std::string& keys,
                            const std::string& callback) {
    keymaps_[mode][keys] = callback;
}

} // namespace plugins
} // namespace pnana

#endif // BUILD_LUA_SUPPORT
