#include "ui/terminal_ui.h"
#include "ui/icons.h"
#include <ftxui/dom/elements.hpp>
#include <algorithm>
#include <cstring>
#include <sstream>

using namespace ftxui;

namespace pnana {
namespace ui {

Element renderTerminal(features::Terminal& terminal, int height) {
    if (!terminal.isVisible()) {
        return text("");
    }
    
    auto& theme = terminal.getTheme();
    auto& colors = theme.getColors();
    
    // 输出区域 - 包含历史输出和当前输入行
    Elements output_lines;
    const auto& output_lines_data = terminal.getOutputLines();
    
    // 计算需要显示的行数（包括输入行）
    size_t total_lines_needed = output_lines_data.size() + 1;  // +1 为输入行
    size_t start_line = 0;
    if (total_lines_needed > static_cast<size_t>(height)) {
        // 确保输入行始终可见，所以从倒数 height-1 行开始显示
        start_line = total_lines_needed - height;
    }
    
    // 显示历史输出
    for (size_t i = start_line; i < output_lines_data.size(); ++i) {
        const auto& line = output_lines_data[i];
        Color line_color;
        if (line.is_command) {
            // 命令使用绿色（类似bash/zsh）
            line_color = Color::Green;
        } else {
            // 输出使用前景色
            line_color = colors.foreground;
        }
        output_lines.push_back(
            text(line.content) | color(line_color)
        );
    }
    
    // 输入行 - 作为输出区域的最后一行
    std::string current_input = terminal.getCurrentInput();
    size_t cursor_position = terminal.getCursorPosition();
    
    std::string before_cursor = current_input.substr(0, cursor_position);
    std::string cursor_char = cursor_position < current_input.length() ? 
                              current_input.substr(cursor_position, 1) : " ";
    std::string after_cursor = cursor_position < current_input.length() ? 
                               current_input.substr(cursor_position + 1) : "";
    
    Elements input_elements;
    
    // 第一部分：用户名@主机名（绿色）
    std::string user_host = terminal.getUsername() + "@" + terminal.getHostname();
    input_elements.push_back(
        text(user_host) | color(Color::Green) | bold
    );
    
    // 第二部分：时间戳（浅蓝色）
    std::string time_str = terminal.getCurrentTime();
    input_elements.push_back(text(" ") | color(Color::White));
    input_elements.push_back(
        text(time_str) | color(Color::Cyan) | dim
    );
    
    // 第三部分：目录路径（浅蓝色）
    std::string dir = terminal.getCurrentDir();
    const char* home = getenv("HOME");
    if (home && dir.find(home) == 0) {
        dir = "~" + dir.substr(strlen(home));
    }
    // 如果目录名太长，只显示最后一部分
    if (dir.length() > 30) {
        size_t last_slash = dir.find_last_of('/');
        if (last_slash != std::string::npos && last_slash < dir.length() - 1) {
            dir = "..." + dir.substr(last_slash);
        }
    }
    input_elements.push_back(text(" ") | color(Color::White));
    input_elements.push_back(
        text(dir) | color(Color::Cyan)
    );
    
    // 第四部分：Git 分支（红色，如果有）
    std::string git_branch = terminal.getGitBranch();
    if (!git_branch.empty()) {
        input_elements.push_back(text(" ") | color(Color::White));
        input_elements.push_back(
            text(git_branch) | color(Color::Red) | bold
        );
    }
    
    // 提示符结束符号（绿色箭头）
    input_elements.push_back(text(" ") | color(Color::White));
    input_elements.push_back(
        text(">") | color(Color::Green) | bold
    );
    input_elements.push_back(text(" ") | color(Color::White));
    
    // 用户输入
    input_elements.push_back(text(before_cursor) | color(colors.foreground));
    // 块状光标
    if (cursor_position < current_input.length()) {
        input_elements.push_back(
            text(cursor_char) | 
            bgcolor(colors.foreground) | 
            color(colors.background) | 
            bold
        );
        input_elements.push_back(text(after_cursor) | color(colors.foreground));
    } else {
        // 光标在行尾（显示为闪烁的竖线）
        input_elements.push_back(
            text("│") | 
            color(colors.foreground) | 
            bold
        );
    }
    
    // 将输入行添加到输出行列表的最后（始终显示）
    output_lines.push_back(hbox(input_elements));
    
    // 简约风格：无边框，使用深色背景
    return vbox(output_lines) | 
           size(HEIGHT, EQUAL, height) | 
           bgcolor(Color::RGB(20, 20, 25));  // 深色终端背景
}

} // namespace ui
} // namespace pnana

