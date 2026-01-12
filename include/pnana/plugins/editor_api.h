#ifdef BUILD_LUA_SUPPORT

#ifndef PNANA_PLUGINS_EDITOR_API_H
#define PNANA_PLUGINS_EDITOR_API_H

#include <lua.hpp>

namespace pnana {
namespace core {
class Editor;
}
namespace plugins {

/**
 * @brief 编辑器操作相关的Lua API
 * 处理文档操作、光标操作等编辑器核心功能
 */
class EditorAPI {
  public:
    EditorAPI(core::Editor* editor);
    ~EditorAPI();

    // 注册所有编辑器相关的API函数
    void registerFunctions(lua_State* L);

  private:
    core::Editor* editor_;

    // 编辑器操作API函数
    static int lua_api_get_current_line(lua_State* L);
    static int lua_api_get_line_count(lua_State* L);
    static int lua_api_get_cursor_pos(lua_State* L);
    static int lua_api_set_cursor_pos(lua_State* L);
    static int lua_api_get_line(lua_State* L);
    static int lua_api_set_line(lua_State* L);
    static int lua_api_insert_text(lua_State* L);
    static int lua_api_delete_line(lua_State* L);

    // 辅助函数
    static core::Editor* getEditorFromLua(lua_State* L);
};

} // namespace plugins
} // namespace pnana

#endif // PNANA_PLUGINS_EDITOR_API_H
#endif // BUILD_LUA_SUPPORT
