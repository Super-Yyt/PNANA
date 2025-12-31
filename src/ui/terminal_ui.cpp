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
    
    // 输出区域和输入行
    Elements output_lines;
    const auto& output_lines_data = terminal.getOutputLines();
    
    // 计算可用高度：总高度 - 1（为输入行预留）
    int available_height = height - 1;
    if (available_height < 1) {
        available_height = 1;  // 至少保留1行用于输出
    }
    
    // 计算要显示的历史输出行数
    size_t output_count = output_lines_data.size();
    size_t start_line = 0;
    
    // 如果输出行数超过可用高度，只显示最近的输出
    if (output_count > static_cast<size_t>(available_height)) {
        start_line = output_count - available_height;
    }
    
    // 显示历史输出（确保不超过可用高度）
    for (size_t i = start_line; i < output_lines_data.size() && 
         (i - start_line) < static_cast<size_t>(available_height); ++i) {
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
    
    // 输入行 - 始终固定在最后一行（确保始终可见）
    std::string current_input = terminal.getCurrentInput();
    size_t cursor_position = terminal.getCursorPosition();
    
    std::string before_cursor = current_input.substr(0, cursor_position);
    std::string cursor_char = cursor_position < current_input.length() ? 
                              current_input.substr(cursor_position, 1) : " ";
    std::string after_cursor = cursor_position < current_input.length() ? 
                               current_input.substr(cursor_position + 1) : "";
    
    Elements input_elements;
    
    // 优化后的提示符样式（类似 zsh/powerlevel10k）
    // 第一部分：用户名（绿色，粗体）
    std::string username = terminal.getUsername();
    input_elements.push_back(
        text(username) | color(Color::Green) | bold
    );
    
    // @ 符号（白色）
    input_elements.push_back(
        text("@") | color(Color::White) | dim
    );
    
    // 主机名（绿色）
    std::string hostname = terminal.getHostname();
    input_elements.push_back(
        text(hostname) | color(Color::Green) | bold
    );
    
    // 分隔符（白色点）
    input_elements.push_back(
        text(" · ") | color(Color::White) | dim
    );
    
    // 第二部分：时间戳（浅蓝色，小写）
    std::string time_str = terminal.getCurrentTime();
    input_elements.push_back(
        text(time_str) | color(Color::Cyan) | dim
    );
    
    // 分隔符
    input_elements.push_back(
        text(" · ") | color(Color::White) | dim
    );
    
    // 第三部分：目录路径（蓝色，高亮）
    std::string dir = terminal.getCurrentDir();
    const char* home = getenv("HOME");
    if (home && dir.find(home) == 0) {
        dir = "~" + dir.substr(strlen(home));
    }
    // 如果目录名太长，只显示最后一部分
    if (dir.length() > 25) {
        size_t last_slash = dir.find_last_of('/');
        if (last_slash != std::string::npos && last_slash < dir.length() - 1) {
            dir = "..." + dir.substr(last_slash);
        }
    }
    input_elements.push_back(
        text(dir) | color(Color::Blue) | bold
    );
    
    // 第四部分：Git 分支（如果有，使用更醒目的颜色）
    std::string git_branch = terminal.getGitBranch();
    if (!git_branch.empty()) {
        input_elements.push_back(
            text(" · ") | color(Color::White) | dim
        );
        // Git 分支图标和名称
        input_elements.push_back(
            text("git:") | color(Color::Yellow) | dim
        );
        input_elements.push_back(
            text(" " + git_branch) | color(Color::Yellow) | bold
        );
    }
    
    // 提示符结束符号（绿色箭头，更醒目）
    input_elements.push_back(
        text(" → ") | color(Color::Green) | bold
    );
    
    // 用户输入（白色，更清晰）
    input_elements.push_back(text(before_cursor) | color(Color::White));
    
    // 块状光标（更醒目的样式）
    if (cursor_position < current_input.length()) {
        input_elements.push_back(
            text(cursor_char) | 
            bgcolor(Color::Green) | 
            color(Color::Black) | 
            bold
        );
        input_elements.push_back(text(after_cursor) | color(Color::White));
    } else {
        // 光标在行尾（显示为绿色竖线）
        input_elements.push_back(
            text("│") | 
            color(Color::Green) | 
            bold
        );
    }
    
    // 构建最终布局：输出区域 + 输入行（固定底部）
    Element output_area = vbox(output_lines);
    Element input_line = hbox(input_elements);
    
    // 使用 flex 确保输出区域可以滚动，输入行固定在底部
    return vbox({
        output_area | flex,  // 输出区域可滚动
        input_line            // 输入行固定在底部
    }) | 
           size(HEIGHT, EQUAL, height) | 
           bgcolor(Color::RGB(20, 20, 25));  // 深色终端背景
}

} // namespace ui
} // namespace pnana

