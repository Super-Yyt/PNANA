#ifdef BUILD_LUA_SUPPORT

#ifndef PNANA_PLUGINS_SYSTEM_API_H
#define PNANA_PLUGINS_SYSTEM_API_H

#include <lua.hpp>

namespace pnana {
namespace plugins {

/**
 * @brief 系统工具相关的Lua API
 * 处理系统命令执行、通知、命令注册等
 */
class SystemAPI {
  public:
    SystemAPI();
    ~SystemAPI();

    // 注册所有系统相关的API函数
    void registerFunctions(lua_State* L);

  private:
    // 系统工具API函数
    static int lua_fn_system(lua_State* L);
    static int lua_api_notify(lua_State* L);
    static int lua_api_command(lua_State* L);
    static int lua_api_keymap(lua_State* L);
    static int lua_api_autocmd(lua_State* L);

    // 辅助函数
    static SystemAPI* getAPIFromLua(lua_State* L);
};

} // namespace plugins
} // namespace pnana

#endif // PNANA_PLUGINS_SYSTEM_API_H
#endif // BUILD_LUA_SUPPORT
