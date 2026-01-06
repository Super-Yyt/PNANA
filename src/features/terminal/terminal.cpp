#include "features/terminal.h"
#include "features/terminal/terminal_builtin.h"
#include "features/terminal/terminal_color.h"
#include "features/terminal/terminal_completion.h"
#include "features/terminal/terminal_parser.h"
#include "features/terminal/terminal_shell.h"
#include "features/terminal/terminal_utils.h"
#include "ui/icons.h"
#include <cstdlib>
#include <ftxui/dom/elements.hpp>
#include <signal.h>
#include <unistd.h>

using namespace ftxui;

namespace pnana {
namespace features {

Terminal::Terminal(ui::Theme& theme)
    : theme_(theme), visible_(false), history_index_(0), max_history_size_(100), current_input_(""),
      cursor_position_(0), max_output_lines_(1000), scroll_offset_(0), current_directory_("."),
      command_running_(false), current_pid_(0) {
    // 初始化当前目录
    char* cwd = getcwd(nullptr, 0);
    if (cwd) {
        current_directory_ = cwd;
        free(cwd);
    }
}

void Terminal::setVisible(bool visible) {
    visible_ = visible;
    if (visible) {
        // 重置历史索引
        history_index_ = 0;
    }
}

void Terminal::handleInput(const std::string& input) {
    current_input_ = input;
    // 保持光标位置在有效范围内
    if (cursor_position_ > current_input_.length()) {
        cursor_position_ = current_input_.length();
    }
}

void Terminal::setCursorPosition(size_t pos) {
    cursor_position_ = std::min(pos, current_input_.length());
}

void Terminal::handleKeyEvent(const std::string& key) {
    if (key == "ArrowLeft") {
        if (cursor_position_ > 0) {
            cursor_position_--;
        }
    } else if (key == "ArrowRight") {
        if (cursor_position_ < current_input_.length()) {
            cursor_position_++;
        }
    } else if (key == "Home") {
        cursor_position_ = 0;
    } else if (key == "End") {
        cursor_position_ = current_input_.length();
    } else if (key == "ArrowUp") {
        // 浏览命令历史
        if (!command_history_.empty()) {
            if (history_index_ < command_history_.size()) {
                history_index_++;
            }
            if (history_index_ > 0 && history_index_ <= command_history_.size()) {
                current_input_ = command_history_[command_history_.size() - history_index_];
                cursor_position_ = current_input_.length();
            }
        }
    } else if (key == "ArrowDown") {
        // 浏览命令历史（向下）
        if (history_index_ > 0) {
            history_index_--;
            if (history_index_ == 0) {
                current_input_ = "";
            } else {
                current_input_ = command_history_[command_history_.size() - history_index_];
            }
            cursor_position_ = current_input_.length();
        }
    } else if (key == "Backspace") {
        if (cursor_position_ > 0) {
            current_input_.erase(cursor_position_ - 1, 1);
            cursor_position_--;
        }
    } else if (key == "Delete") {
        if (cursor_position_ < current_input_.length()) {
            current_input_.erase(cursor_position_, 1);
        }
    } else if (key == "Ctrl+C") {
        // Ctrl+C: 中断当前命令或清空输入行
        if (command_running_) {
            // 如果有命令正在运行，中断它
            interruptCommand();
        } else if (!current_input_.empty()) {
            // 如果有输入，清空当前输入行
            current_input_.clear();
            cursor_position_ = 0;
            addOutputLine("^C", false);
        }
    } else if (key == "Ctrl+D") {
        // Ctrl+D: 退出终端（如果输入行为空）或删除字符
        if (current_input_.empty()) {
            // 输入行为空，关闭终端
            setVisible(false);
        } else {
            // 删除光标处字符（同Delete）
            if (cursor_position_ < current_input_.length()) {
                current_input_.erase(cursor_position_, 1);
            }
        }
    } else if (key == "Ctrl+L") {
        // Ctrl+L: 清屏
        executeCommand("clear");
    } else if (key == "Ctrl+U") {
        // Ctrl+U: 删除光标前所有字符
        if (cursor_position_ > 0) {
            current_input_.erase(0, cursor_position_);
            cursor_position_ = 0;
        }
    } else if (key == "Ctrl+K") {
        // Ctrl+K: 删除光标后所有字符
        if (cursor_position_ < current_input_.length()) {
            current_input_.erase(cursor_position_);
        }
    } else if (key == "Ctrl+A") {
        // Ctrl+A: 移动到行首
        cursor_position_ = 0;
    } else if (key == "Ctrl+E") {
        // Ctrl+E: 移动到行尾
        cursor_position_ = current_input_.length();
    } else if (key == "Ctrl+W") {
        // Ctrl+W: 删除前一个单词
        if (cursor_position_ > 0) {
            size_t new_pos = cursor_position_;
            // 跳过当前单词结尾的空格
            while (new_pos > 0 && current_input_[new_pos - 1] == ' ') {
                new_pos--;
            }
            // 删除单词
            while (new_pos > 0 && current_input_[new_pos - 1] != ' ') {
                new_pos--;
            }
            current_input_.erase(new_pos, cursor_position_ - new_pos);
            cursor_position_ = new_pos;
        }
    } else if (key == "PageUp") {
        // PageUp: 向上滚动页面
        scrollUp();
    } else if (key == "PageDown") {
        // PageDown: 向下滚动页面
        scrollDown();
    }
}

void Terminal::executeCommand(const std::string& command) {
    if (command.empty()) {
        // 空命令，不添加任何输出，输入行会显示提示符
        return;
    }

    using namespace terminal;

    // 检查是否是后台命令
    std::string cmd;
    bool is_background = CommandParser::isBackgroundCommand(command, cmd);

    // 添加到历史
    if (command_history_.empty() || command_history_.back() != command) {
        command_history_.push_back(command);
        if (command_history_.size() > max_history_size_) {
            command_history_.pop_front();
        }
    }
    history_index_ = 0;

    // 显示命令（带提示符）
    addOutputLine(buildPrompt() + command, true);

    // 方案：所有命令都通过系统 shell 执行，以支持所有 Linux 命令和参数
    // 这样可以：
    // 1. 支持所有系统命令（ls, grep, find, git, python, 等）
    // 2. 支持所有命令参数（-al, --help, -r, 等）
    // 3. 支持所有 shell 特性（管道、重定向、环境变量等）
    // 4. 自动处理命令别名、PATH 查找等

    // 特殊处理：cd 命令需要更新当前目录
    std::vector<std::string> args = CommandParser::parse(cmd);
    if (!args.empty() && args[0] == "cd") {
        // cd 命令需要特殊处理，因为它需要改变终端的工作目录
        std::string cd_result = BuiltinCommandExecutor::execute(
            "cd",
            args.size() > 1 ? std::vector<std::string>(args.begin() + 1, args.end())
                            : std::vector<std::string>(),
            current_directory_, output_lines_);
        // cd 命令通常没有输出，但如果有错误会返回错误信息
        if (!cd_result.empty()) {
            // 添加错误图标和更好的格式
            addOutputLine(pnana::ui::icons::ERROR + std::string(" ") + cd_result, false);
        } else {
            // 成功时显示新目录（可选，通过环境变量控制）
            const char* show_cd = getenv("PNANA_TERMINAL_SHOW_CD");
            if (show_cd && std::string(show_cd) == "1") {
                addOutputLine(pnana::ui::icons::FOLDER + std::string(" Changed directory to: ") +
                                  current_directory_,
                              false);
            }
        }
        return;
    }

    // 特殊处理：clear 命令需要清空输出
    if (!args.empty() && (args[0] == "clear" || args[0] == "cls")) {
        BuiltinCommandExecutor::execute("clear", std::vector<std::string>(), current_directory_,
                                        output_lines_);
        return;
    }

    // 所有其他命令都通过 shell 执行
    // 这样可以支持所有 Linux 命令和参数，无需手动解析
    std::string result =
        ShellCommandExecutor::executeShellCommand(cmd, is_background, current_directory_);

    if (!result.empty()) {
        // 检查是否是错误输出（通常错误信息会以"Error:"或特定模式开头）
        bool is_error = (result.find("Error:") == 0 || result.find("Failed to") == 0 ||
                         result.find("Command failed") == 0 || result.find("cd:") == 0 ||
                         result.find("ls:") == 0 || result.find("cat:") == 0);

        // 优化多行输出处理，避免使用 stringstream
        size_t start = 0;
        size_t end = 0;

        // 预分配输出行向量以提高性能
        std::vector<std::string> lines;
        lines.reserve(std::count(result.begin(), result.end(), '\n') + 1);

        while ((end = result.find('\n', start)) != std::string::npos) {
            std::string line = result.substr(start, end - start);
            if (is_error && !line.empty()) {
                line = pnana::ui::icons::ERROR + std::string(" ") + line; // 为错误行添加错误图标
            }
            lines.emplace_back(line);
            start = end + 1;
        }

        // 处理最后一行（如果没有以换行符结尾）
        if (start < result.length()) {
            std::string last_line = result.substr(start);
            if (is_error && !last_line.empty()) {
                last_line = pnana::ui::icons::ERROR + std::string(" ") + last_line;
            }
            lines.emplace_back(last_line);
        }

        // 批量添加输出行（更高效）
        addOutputLines(lines, false);
    } else if (is_background) {
        // 后台命令成功启动的反馈
        addOutputLine(pnana::ui::icons::SUCCESS + std::string(" Command started in background"),
                      false);
    }
}

ftxui::Element Terminal::render(int /* height */) {
    // 渲染逻辑已迁移到 ui/terminal_ui.cpp
    // 这里保留是为了向后兼容，实际应该使用 ui::renderTerminal
    return text(""); // 占位符，实际不会使用
}

void Terminal::addOutputLine(const std::string& line, bool is_command) {
    // 使用环形缓冲区优化：当达到最大行数时，移除最旧的行
    if (output_lines_.size() >= max_output_lines_) {
        // 移除最旧的行（从开头移除）
        output_lines_.erase(output_lines_.begin());
    }

    bool has_ansi = terminal::AnsiColorParser::hasAnsiCodes(line);
    output_lines_.push_back(TerminalLine(line, is_command, has_ansi));
}

void Terminal::addOutputLines(const std::vector<std::string>& lines, bool is_command) {
    // 批量添加多行输出，预先计算需要移除的行数
    size_t total_lines = output_lines_.size() + lines.size();
    size_t lines_to_remove = 0;

    if (total_lines > max_output_lines_) {
        lines_to_remove = total_lines - max_output_lines_;
        if (lines_to_remove > output_lines_.size()) {
            lines_to_remove = output_lines_.size();
        }
    }

    // 移除多余的行
    if (lines_to_remove > 0) {
        output_lines_.erase(output_lines_.begin(), output_lines_.begin() + lines_to_remove);
    }

    // 批量添加新行
    output_lines_.reserve(output_lines_.size() + lines.size());
    for (const auto& line : lines) {
        bool has_ansi = terminal::AnsiColorParser::hasAnsiCodes(line);
        output_lines_.emplace_back(line, is_command, has_ansi);
    }
}

std::string Terminal::buildPrompt() const {
    using namespace terminal;

    // 预计算各个部分的大小以优化内存分配
    static const std::string separator = " · ";
    static const std::string prompt_end = " → ";

    std::string username = TerminalUtils::getUsername();
    std::string hostname = TerminalUtils::getHostname();
    std::string time_str = TerminalUtils::getCurrentTime();
    std::string dir = TerminalUtils::simplifyPath(current_directory_);
    dir = TerminalUtils::truncatePath(dir, 25);
    std::string git_branch = TerminalUtils::getGitBranch(current_directory_);

    // 预估总长度以减少重新分配
    size_t estimated_size = username.length() + hostname.length() + time_str.length() +
                            dir.length() + separator.length() * 3 + prompt_end.length() + 10;

    if (!git_branch.empty()) {
        estimated_size += git_branch.length() + 5; // "git:" + separator
    }

    std::string result;
    result.reserve(estimated_size);

    // 构建提示符字符串
    result += username;
    result += "@";
    result += hostname;
    result += separator;
    result += time_str;
    result += separator;
    result += dir;

    // Git 分支（如果有）
    if (!git_branch.empty()) {
        result += separator;
        result += "git:";
        result += git_branch;
    }

    result += prompt_end;

    return result;
}

std::string Terminal::getUsername() const {
    return terminal::TerminalUtils::getUsername();
}

std::string Terminal::getHostname() const {
    return terminal::TerminalUtils::getHostname();
}

std::string Terminal::getCurrentDir() const {
    return current_directory_;
}

std::string Terminal::getGitBranch() const {
    return terminal::TerminalUtils::getGitBranch(current_directory_);
}

std::string Terminal::getCurrentTime() const {
    return terminal::TerminalUtils::getCurrentTime();
}

ftxui::Color Terminal::getPromptColor() const {
    return Color::Green; // 使用绿色，更像真实终端
}

ftxui::Color Terminal::getCommandColor() const {
    return Color::Green; // 命令也使用绿色
}

ftxui::Color Terminal::getOutputColor() const {
    auto& colors = theme_.getColors();
    return colors.foreground;
}

ftxui::Color Terminal::getErrorColor() const {
    return Color::Red;
}

void Terminal::clear() {
    output_lines_.clear();
    // 清空后不显示任何消息，更像真实终端
}

void Terminal::interruptCommand() {
    if (command_running_ && current_pid_ > 0) {
        // 发送SIGINT信号中断进程
        kill(current_pid_, SIGINT);
        command_running_ = false;
        current_pid_ = 0;
        addOutputLine("^C", false);
    }
}

// 保留旧的方法以保持兼容性（已移至各个模块）
std::vector<std::string> Terminal::parseCommand(const std::string& command) {
    return terminal::CommandParser::parse(command);
}

std::string Terminal::executeBuiltinCommand(const std::string& command,
                                            const std::vector<std::string>& args) {
    return terminal::BuiltinCommandExecutor::execute(command, args, current_directory_,
                                                     output_lines_);
}

std::string Terminal::executeSystemCommand(const std::string& command,
                                           const std::vector<std::string>& args) {
    return terminal::ShellCommandExecutor::executeSystemCommand(command, args, current_directory_);
}

std::string Terminal::executeShellCommand(const std::string& command, bool background) {
    return terminal::ShellCommandExecutor::executeShellCommand(command, background,
                                                               current_directory_);
}

bool Terminal::handleTabCompletion() {
    std::string completed;
    size_t new_pos;

    bool success = terminal::TerminalCompletion::complete(current_input_, cursor_position_,
                                                          current_directory_, completed, new_pos);

    if (success) {
        current_input_ = completed;
        cursor_position_ = new_pos;
        return true;
    }

    return false;
}

// 滚动功能实现
void Terminal::scrollUp() {
    // 向上滚动：增加偏移量，最多滚动到输出历史的开始
    if (scroll_offset_ < output_lines_.size()) {
        scroll_offset_ += 1;
    }
}

void Terminal::scrollDown() {
    // 向下滚动：减少偏移量，最少滚动到最新输出
    if (scroll_offset_ > 0) {
        scroll_offset_ -= 1;
    }
}

void Terminal::scrollToTop() {
    // 滚动到最顶部
    scroll_offset_ = output_lines_.size();
}

void Terminal::scrollToBottom() {
    // 滚动到最底部（最新输出）
    scroll_offset_ = 0;
}

} // namespace features
} // namespace pnana
