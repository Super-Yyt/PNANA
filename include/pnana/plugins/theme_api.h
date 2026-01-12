#ifdef BUILD_LUA_SUPPORT

#ifndef PNANA_PLUGINS_THEME_API_H
#define PNANA_PLUGINS_THEME_API_H

#include <lua.hpp>
#include <string>
#include <vector>

namespace pnana {
namespace core {
class Editor;
}
namespace ui {
struct ThemeColors;
struct StatusbarBeautifyConfig;
} // namespace ui
namespace plugins {

/**
 * @brief 主题和外观相关的Lua API
 * 处理主题设置、自定义主题添加/删除、状态栏美化等
 */
class ThemeAPI {
  public:
    ThemeAPI(core::Editor* editor);
    ~ThemeAPI();

    // 注册所有主题相关的API函数
    void registerFunctions(lua_State* L);

  private:
    core::Editor* editor_;

    // 主题操作API函数
    static int lua_api_get_theme(lua_State* L);
    static int lua_api_get_current_theme(lua_State* L);
    static int lua_api_set_theme(lua_State* L);
    static int lua_api_add_theme(lua_State* L);
    static int lua_api_remove_theme(lua_State* L);
    static int lua_api_set_status_message(lua_State* L);
    static int lua_api_set_statusbar_beautify(lua_State* L);

    // 辅助函数
    static ui::ThemeColors parseThemeColorsFromLua(lua_State* L, int tableIndex);
    static ui::StatusbarBeautifyConfig parseStatusbarConfigFromLua(lua_State* L, int tableIndex);
    static core::Editor* getEditorFromLua(lua_State* L);
};

} // namespace plugins
} // namespace pnana

#endif // PNANA_PLUGINS_THEME_API_H
#endif // BUILD_LUA_SUPPORT
