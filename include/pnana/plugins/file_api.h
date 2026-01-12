#ifdef BUILD_LUA_SUPPORT

#ifndef PNANA_PLUGINS_FILE_API_H
#define PNANA_PLUGINS_FILE_API_H

#include <lua.hpp>

namespace pnana {
namespace core {
class Editor;
}
namespace plugins {

/**
 * @brief 文件操作相关的Lua API
 * 处理文件打开、保存、路径获取等文件操作
 */
class FileAPI {
  public:
    FileAPI(core::Editor* editor);
    ~FileAPI();

    // 注册所有文件相关的API函数
    void registerFunctions(lua_State* L);

  private:
    core::Editor* editor_;

    // 文件操作API函数
    static int lua_api_get_filepath(lua_State* L);
    static int lua_api_open_file(lua_State* L);
    static int lua_api_save_file(lua_State* L);
    static int lua_fn_readfile(lua_State* L);
    static int lua_fn_writefile(lua_State* L);

    // 辅助函数
    static core::Editor* getEditorFromLua(lua_State* L);
};

} // namespace plugins
} // namespace pnana

#endif // PNANA_PLUGINS_FILE_API_H
#endif // BUILD_LUA_SUPPORT
