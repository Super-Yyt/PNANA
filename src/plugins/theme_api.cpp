#ifdef BUILD_LUA_SUPPORT

#include "plugins/theme_api.h"
#include "core/editor.h"
#include "ui/statusbar.h"
#include "ui/theme.h"
#include <ftxui/screen/color.hpp>
#include <lua.hpp>

namespace pnana {
namespace plugins {

// 在 Lua 注册表中存储编辑器指针的键
static const char* EDITOR_REGISTRY_KEY = "pnana_editor";

ThemeAPI::ThemeAPI(core::Editor* editor) : editor_(editor) {}

ThemeAPI::~ThemeAPI() {}

void ThemeAPI::registerFunctions(lua_State* L) {
    // 在注册表中存储编辑器指针
    lua_pushlightuserdata(L, editor_);
    lua_setfield(L, LUA_REGISTRYINDEX, EDITOR_REGISTRY_KEY);

    // 注册API函数到vim.api表
    lua_getglobal(L, "vim");
    lua_getfield(L, -1, "api");

    lua_pushcfunction(L, lua_api_get_theme);
    lua_setfield(L, -2, "get_theme");

    lua_pushcfunction(L, lua_api_get_current_theme);
    lua_setfield(L, -2, "get_current_theme");

    lua_pushcfunction(L, lua_api_set_theme);
    lua_setfield(L, -2, "set_theme");

    lua_pushcfunction(L, lua_api_add_theme);
    lua_setfield(L, -2, "add_theme");

    lua_pushcfunction(L, lua_api_remove_theme);
    lua_setfield(L, -2, "remove_theme");

    lua_pushcfunction(L, lua_api_set_status_message);
    lua_setfield(L, -2, "set_status_message");

    lua_pushcfunction(L, lua_api_set_statusbar_beautify);
    lua_setfield(L, -2, "set_statusbar_beautify");

    lua_pop(L, 2); // 弹出vim和api表
}

core::Editor* ThemeAPI::getEditorFromLua(lua_State* L) {
    lua_getfield(L, LUA_REGISTRYINDEX, EDITOR_REGISTRY_KEY);
    core::Editor* editor = static_cast<core::Editor*>(lua_touserdata(L, -1));
    lua_pop(L, 1);
    return editor;
}

ui::ThemeColors ThemeAPI::parseThemeColorsFromLua(lua_State* L, int tableIndex) {
    ui::ThemeColors colors;

    // 辅助函数：从表中获取RGB数组并转换为Color
    auto get_color = [&](const char* key) -> ftxui::Color {
        lua_getfield(L, tableIndex, key);
        if (lua_istable(L, -1)) {
            std::vector<int> rgb;
            for (int i = 1; i <= 3; ++i) {
                lua_rawgeti(L, -1, i);
                if (lua_isnumber(L, -1)) {
                    rgb.push_back(static_cast<int>(lua_tonumber(L, -1)));
                }
                lua_pop(L, 1);
            }
            lua_pop(L, 1); // 弹出RGB表
            if (rgb.size() == 3) {
                return ftxui::Color::RGB(rgb[0], rgb[1], rgb[2]);
            }
        } else {
            lua_pop(L, 1); // 弹出nil值
        }
        return ftxui::Color::Default;
    };

    // 解析所有颜色字段
    colors.background = get_color("background");
    colors.foreground = get_color("foreground");
    colors.current_line = get_color("current_line");
    colors.selection = get_color("selection");
    colors.line_number = get_color("line_number");
    colors.line_number_current = get_color("line_number_current");
    colors.statusbar_bg = get_color("statusbar_bg");
    colors.statusbar_fg = get_color("statusbar_fg");
    colors.menubar_bg = get_color("menubar_bg");
    colors.menubar_fg = get_color("menubar_fg");
    colors.helpbar_bg = get_color("helpbar_bg");
    colors.helpbar_fg = get_color("helpbar_fg");
    colors.helpbar_key = get_color("helpbar_key");
    colors.keyword = get_color("keyword");
    colors.string = get_color("string");
    colors.comment = get_color("comment");
    colors.number = get_color("number");
    colors.function = get_color("function");
    colors.type = get_color("type");
    colors.operator_color = get_color("operator_color");
    colors.error = get_color("error");
    colors.warning = get_color("warning");
    colors.info = get_color("info");
    colors.success = get_color("success");

    return colors;
}

ui::StatusbarBeautifyConfig ThemeAPI::parseStatusbarConfigFromLua(lua_State* L, int tableIndex) {
    ui::StatusbarBeautifyConfig config;

    // 获取 enabled 字段
    lua_getfield(L, tableIndex, "enabled");
    if (lua_isboolean(L, -1)) {
        config.enabled = lua_toboolean(L, -1);
    }
    lua_pop(L, 1);

    // 获取 bg_color 字段（RGB 数组）
    lua_getfield(L, tableIndex, "bg_color");
    if (lua_istable(L, -1)) {
        std::vector<int> rgb;
        for (int i = 1; i <= 3; ++i) {
            lua_rawgeti(L, -1, i);
            if (lua_isnumber(L, -1)) {
                rgb.push_back(static_cast<int>(lua_tonumber(L, -1)));
            }
            lua_pop(L, 1);
        }
        if (rgb.size() == 3) {
            config.bg_color = rgb;
        }
    }
    lua_pop(L, 1);

    // 获取 fg_color 字段（RGB 数组）
    lua_getfield(L, tableIndex, "fg_color");
    if (lua_istable(L, -1)) {
        std::vector<int> rgb;
        for (int i = 1; i <= 3; ++i) {
            lua_rawgeti(L, -1, i);
            if (lua_isnumber(L, -1)) {
                rgb.push_back(static_cast<int>(lua_tonumber(L, -1)));
            }
            lua_pop(L, 1);
        }
        if (rgb.size() == 3) {
            config.fg_color = rgb;
        }
    }
    lua_pop(L, 1);

    // 获取其他布尔字段
    lua_getfield(L, tableIndex, "show_gradient");
    if (lua_isboolean(L, -1)) {
        config.show_gradient = lua_toboolean(L, -1);
    }
    lua_pop(L, 1);

    lua_getfield(L, tableIndex, "show_shadows");
    if (lua_isboolean(L, -1)) {
        config.show_shadows = lua_toboolean(L, -1);
    }
    lua_pop(L, 1);

    lua_getfield(L, tableIndex, "rounded_corners");
    if (lua_isboolean(L, -1)) {
        config.rounded_corners = lua_toboolean(L, -1);
    }
    lua_pop(L, 1);

    // 获取 icon_style 字段
    lua_getfield(L, tableIndex, "icon_style");
    if (lua_isstring(L, -1)) {
        config.icon_style = lua_tostring(L, -1);
    }
    lua_pop(L, 1);

    // 获取 icon_enhancement 字段（图标增强配置）
    lua_getfield(L, tableIndex, "icon_enhancement");
    if (lua_istable(L, -1)) {
        // 解析 file_icons
        lua_getfield(L, -1, "file_icons");
        if (lua_istable(L, -1)) {
            lua_pushnil(L);
            while (lua_next(L, -2) != 0) {
                if (lua_isstring(L, -2) && lua_isstring(L, -1)) {
                    config.file_icons[lua_tostring(L, -2)] = lua_tostring(L, -1);
                }
                lua_pop(L, 1);
            }
        }
        lua_pop(L, 1);

        // 解析 region_icons
        lua_getfield(L, -1, "region_icons");
        if (lua_istable(L, -1)) {
            lua_pushnil(L);
            while (lua_next(L, -2) != 0) {
                if (lua_isstring(L, -2) && lua_isstring(L, -1)) {
                    config.region_icons[lua_tostring(L, -2)] = lua_tostring(L, -1);
                }
                lua_pop(L, 1);
            }
        }
        lua_pop(L, 1);

        // 解析 status_icons
        lua_getfield(L, -1, "status_icons");
        if (lua_istable(L, -1)) {
            lua_pushnil(L);
            while (lua_next(L, -2) != 0) {
                if (lua_isstring(L, -2) && lua_isstring(L, -1)) {
                    config.status_icons[lua_tostring(L, -2)] = lua_tostring(L, -1);
                }
                lua_pop(L, 1);
            }
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    // 获取 color_enhancement 字段（颜色增强配置）
    lua_getfield(L, tableIndex, "color_enhancement");
    if (lua_istable(L, -1)) {
        lua_getfield(L, -1, "elements");
        if (lua_istable(L, -1)) {
            lua_pushnil(L);
            while (lua_next(L, -2) != 0) {
                if (lua_isstring(L, -2) && lua_istable(L, -1)) {
                    std::vector<int> rgb;
                    for (int i = 1; i <= 3; ++i) {
                        lua_rawgeti(L, -1, i);
                        if (lua_isnumber(L, -1)) {
                            rgb.push_back(static_cast<int>(lua_tonumber(L, -1)));
                        }
                        lua_pop(L, 1);
                    }
                    if (rgb.size() == 3) {
                        config.element_colors[lua_tostring(L, -2)] = rgb;
                    }
                }
                lua_pop(L, 1);
            }
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    // 获取 layout_enhancement 字段（布局增强配置）
    lua_getfield(L, tableIndex, "layout_enhancement");
    if (lua_istable(L, -1)) {
        lua_getfield(L, -1, "spacing");
        if (lua_isnumber(L, -1)) {
            // 可以在这里存储间距配置（暂时保留）
        }
        lua_pop(L, 1);

        lua_getfield(L, -1, "padding");
        if (lua_isnumber(L, -1)) {
            // 可以在这里存储填充配置（暂时保留）
        }
        lua_pop(L, 1);

        lua_getfield(L, -1, "shadow");
        if (lua_isboolean(L, -1)) {
            config.show_shadows = lua_toboolean(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, -1, "gradient");
        if (lua_isboolean(L, -1)) {
            config.show_gradient = lua_toboolean(L, -1);
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);

    return config;
}

// vim.api.get_theme() -> string
int ThemeAPI::lua_api_get_theme(lua_State* L) {
    // TODO: 实现获取主题
    lua_pushstring(L, "monokai");
    return 1;
}

// vim.api.get_current_theme() -> string
int ThemeAPI::lua_api_get_current_theme(lua_State* L) {
    core::Editor* editor = getEditorFromLua(L);
    if (!editor) {
        lua_pushstring(L, "monokai");
        return 1;
    }

    std::string current_theme = editor->getTheme().getCurrentThemeName();
    lua_pushstring(L, current_theme.c_str());
    return 1;
}

// vim.api.set_theme(theme_name)
int ThemeAPI::lua_api_set_theme(lua_State* L) {
    core::Editor* editor = getEditorFromLua(L);
    if (!editor) {
        return 0;
    }

    const char* theme = lua_tostring(L, 1);
    if (theme) {
        editor->setTheme(std::string(theme));
    }
    return 0;
}

// vim.api.add_theme(theme_name, theme_config)
int ThemeAPI::lua_api_add_theme(lua_State* L) {
    core::Editor* editor = getEditorFromLua(L);
    if (!editor) {
        lua_pushboolean(L, false);
        return 1;
    }

    const char* theme_name = lua_tostring(L, 1);
    if (!theme_name || !lua_istable(L, 2)) {
        lua_pushboolean(L, false);
        return 1;
    }

    ui::ThemeColors colors = parseThemeColorsFromLua(L, 2);

    // 添加到主题系统
    bool success = editor->getTheme().loadCustomTheme(std::string(theme_name), colors);
    if (success) {
        // 通知编辑器刷新主题菜单
        std::vector<std::string> available_themes;
        auto& config = editor->getConfigManager().getConfig();
        if (!config.available_themes.empty()) {
            available_themes = config.available_themes;
        } else {
            // 使用 Theme 类提供的所有可用主题
            available_themes = ::pnana::ui::Theme::getAvailableThemes();
        }

        // 添加自定义主题（包括插件添加的主题）
        std::vector<std::string> custom_themes = editor->getTheme().getCustomThemeNames();
        available_themes.insert(available_themes.end(), custom_themes.begin(), custom_themes.end());

        editor->getThemeMenu().setAvailableThemes(available_themes);
    }
    lua_pushboolean(L, success);
    return 1;
}

// vim.api.remove_theme(theme_name) -> boolean
int ThemeAPI::lua_api_remove_theme(lua_State* L) {
    core::Editor* editor = getEditorFromLua(L);
    if (!editor) {
        lua_pushboolean(L, false);
        return 1;
    }

    const char* theme_name = lua_tostring(L, 1);
    if (!theme_name) {
        lua_pushboolean(L, false);
        return 1;
    }

    // 移除自定义主题
    bool success = editor->getTheme().removeCustomTheme(std::string(theme_name));
    if (success) {
        // 通知编辑器刷新主题菜单
        std::vector<std::string> available_themes;
        auto& config = editor->getConfigManager().getConfig();
        if (!config.available_themes.empty()) {
            available_themes = config.available_themes;
        } else {
            // 使用 Theme 类提供的所有可用主题
            available_themes = ::pnana::ui::Theme::getAvailableThemes();
        }

        // 添加自定义主题（包括插件添加的主题）
        std::vector<std::string> custom_themes = editor->getTheme().getCustomThemeNames();
        available_themes.insert(available_themes.end(), custom_themes.begin(), custom_themes.end());

        editor->getThemeMenu().setAvailableThemes(available_themes);
    }
    lua_pushboolean(L, success);
    return 1;
}

// vim.api.set_status_message(message)
int ThemeAPI::lua_api_set_status_message(lua_State* L) {
    core::Editor* editor = getEditorFromLua(L);
    if (!editor) {
        return 0;
    }

    const char* message = lua_tostring(L, 1);
    if (message) {
        // LuaAPI 是 Editor 的友元类，可以访问私有方法
        editor->setStatusMessageForLua(std::string(message));
    }
    return 0;
}

// vim.api.set_statusbar_beautify(config)
int ThemeAPI::lua_api_set_statusbar_beautify(lua_State* L) {
    core::Editor* editor = getEditorFromLua(L);
    if (!editor) {
        lua_pushboolean(L, false);
        return 1;
    }

    if (!lua_istable(L, 1)) {
        lua_pushboolean(L, false);
        return 1;
    }

    ui::StatusbarBeautifyConfig config = parseStatusbarConfigFromLua(L, 1);

    // 应用配置到状态栏
    editor->setStatusbarBeautify(config);

    lua_pushboolean(L, true);
    return 1;
}

} // namespace plugins
} // namespace pnana

#endif // BUILD_LUA_SUPPORT
