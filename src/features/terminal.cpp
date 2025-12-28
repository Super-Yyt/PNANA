#include "features/terminal.h"
#include "ui/icons.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <ftxui/dom/elements.hpp>
#include <filesystem>
#include <ctime>
#include <iomanip>

namespace fs = std::filesystem;

using namespace ftxui;

namespace pnana {
namespace features {

Terminal::Terminal(ui::Theme& theme)
    : theme_(theme),
      visible_(false),
      history_index_(0),
      max_history_size_(100),
      current_input_(""),
      cursor_position_(0),
      max_output_lines_(1000),
      current_directory_(".") {
    // 初始化当前目录
    char* cwd = getcwd(nullptr, 0);
    if (cwd) {
        current_directory_ = cwd;
        free(cwd);
    }
    
    // 不显示欢迎信息，保持简约
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
    }
}

void Terminal::executeCommand(const std::string& command) {
    if (command.empty()) {
        // 空命令，不添加任何输出，输入行会显示提示符
        return;
    }
    
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
    
    // 解析命令
    std::vector<std::string> args = parseCommand(command);
    if (args.empty()) {
        // 解析失败，不添加提示符（输入行会显示）
        return;
    }
    
    std::string cmd = args[0];
    args.erase(args.begin());
    
    // 执行内置命令
    std::string result = executeBuiltinCommand(cmd, args);
    if (!result.empty()) {
        // 如果结果包含多行，需要分割
        std::istringstream iss(result);
        std::string line;
        while (std::getline(iss, line)) {
            addOutputLine(line, false);
        }
    }
    
    // 命令执行完成后，不添加提示符到历史输出
    // 输入行会始终显示在最后，作为当前输入状态
}

std::vector<std::string> Terminal::parseCommand(const std::string& command) {
    std::vector<std::string> args;
    std::istringstream iss(command);
    std::string arg;
    
    bool in_quotes = false;
    char quote_char = '\0';
    std::string current_arg;
    
    for (size_t i = 0; i < command.length(); ++i) {
        char c = command[i];
        
        if (in_quotes) {
            if (c == quote_char) {
                in_quotes = false;
                quote_char = '\0';
                if (!current_arg.empty()) {
                    args.push_back(current_arg);
                    current_arg.clear();
                }
            } else {
                current_arg += c;
            }
        } else {
            if (c == '"' || c == '\'') {
                if (!current_arg.empty()) {
                    args.push_back(current_arg);
                    current_arg.clear();
                }
                in_quotes = true;
                quote_char = c;
            } else if (std::isspace(c)) {
                if (!current_arg.empty()) {
                    args.push_back(current_arg);
                    current_arg.clear();
                }
            } else {
                current_arg += c;
            }
        }
    }
    
    if (!current_arg.empty()) {
        args.push_back(current_arg);
    }
    
    return args;
}

std::string Terminal::executeBuiltinCommand(const std::string& command, const std::vector<std::string>& args) {
    if (command == "help" || command == "h") {
        return "Available commands:\n"
               "  help, h          - Show this help message\n"
               "  clear, cls       - Clear terminal output\n"
               "  pwd              - Print current directory\n"
               "  cd <dir>         - Change directory\n"
               "  ls [dir]         - List directory contents\n"
               "  cat <file>       - Display file contents\n"
               "  echo <text>      - Print text\n"
               "  whoami           - Print current user\n"
               "  hostname         - Print hostname\n"
               "  exit, quit       - Close terminal";
    } else if (command == "clear" || command == "cls") {
        clear();
        return "";
    } else if (command == "pwd") {
        return current_directory_;
    } else if (command == "cd") {
        if (args.empty()) {
            // 切换到用户主目录
            const char* home = getenv("HOME");
            if (home) {
                current_directory_ = home;
            }
        } else {
            std::string target = args[0];
            if (target == "~") {
                const char* home = getenv("HOME");
                if (home) {
                    target = home;
                }
            } else if (target[0] == '~') {
                const char* home = getenv("HOME");
                if (home) {
                    target = std::string(home) + target.substr(1);
                }
            }
            
            fs::path new_path;
            if (fs::path(target).is_absolute()) {
                new_path = target;
            } else {
                new_path = fs::path(current_directory_) / target;
            }
            
            try {
                new_path = fs::canonical(new_path);
                if (fs::exists(new_path) && fs::is_directory(new_path)) {
                    current_directory_ = new_path.string();
                } else {
                    return "cd: " + target + ": No such file or directory";
                }
            } catch (const std::exception& e) {
                return "cd: " + target + ": " + e.what();
            }
        }
        return "";
    } else if (command == "ls") {
        std::string target_dir = args.empty() ? current_directory_ : args[0];
        
        if (target_dir[0] == '~') {
            const char* home = getenv("HOME");
            if (home) {
                target_dir = std::string(home) + target_dir.substr(1);
            }
        }
        
        fs::path dir_path;
        if (fs::path(target_dir).is_absolute()) {
            dir_path = target_dir;
        } else {
            dir_path = fs::path(current_directory_) / target_dir;
        }
        
        try {
            dir_path = fs::canonical(dir_path);
            if (!fs::exists(dir_path) || !fs::is_directory(dir_path)) {
                return "ls: " + target_dir + ": No such file or directory";
            }
            
            std::vector<std::string> items;
            for (const auto& entry : fs::directory_iterator(dir_path)) {
                std::string name = entry.path().filename().string();
                if (entry.is_directory()) {
                    items.push_back(name + "/");
                } else {
                    items.push_back(name);
                }
            }
            
            std::sort(items.begin(), items.end());
            
            std::ostringstream oss;
            for (size_t i = 0; i < items.size(); ++i) {
                if (i > 0) oss << "  ";
                oss << items[i];
            }
            return oss.str();
        } catch (const std::exception& e) {
            return "ls: " + target_dir + ": " + e.what();
        }
    } else if (command == "cat") {
        if (args.empty()) {
            return "cat: missing file argument";
        }
        
        std::string file_path = args[0];
        if (file_path[0] == '~') {
            const char* home = getenv("HOME");
            if (home) {
                file_path = std::string(home) + file_path.substr(1);
            }
        }
        
        fs::path full_path;
        if (fs::path(file_path).is_absolute()) {
            full_path = file_path;
        } else {
            full_path = fs::path(current_directory_) / file_path;
        }
        
        try {
            full_path = fs::canonical(full_path);
            if (!fs::exists(full_path) || !fs::is_directory(full_path)) {
                std::ifstream file(full_path);
                if (file.is_open()) {
                    std::ostringstream oss;
                    oss << file.rdbuf();
                    return oss.str();
                } else {
                    return "cat: " + file_path + ": Cannot open file";
                }
            } else {
                return "cat: " + file_path + ": Is a directory";
            }
        } catch (const std::exception& e) {
            return "cat: " + file_path + ": " + e.what();
        }
    } else if (command == "echo") {
        std::ostringstream oss;
        for (size_t i = 0; i < args.size(); ++i) {
            if (i > 0) oss << " ";
            oss << args[i];
        }
        return oss.str();
    } else if (command == "whoami") {
        return getUsername();
    } else if (command == "hostname") {
        return getHostname();
    } else if (command == "exit" || command == "quit") {
        // 这个命令由 Editor 处理，关闭终端
        return "";
    } else {
        // 尝试执行系统命令
        return executeSystemCommand(command, args);
    }
}

std::string Terminal::executeSystemCommand(const std::string& command, const std::vector<std::string>& args) {
    // 构建命令字符串
    std::ostringstream cmd_stream;
    cmd_stream << command;
    for (const auto& arg : args) {
        cmd_stream << " " << arg;
    }
    
    // 执行命令并捕获输出
    std::string cmd_str = cmd_stream.str();
    FILE* pipe = popen(cmd_str.c_str(), "r");
    if (!pipe) {
        return "Command not found: " + command;
    }
    
    std::ostringstream result;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result << buffer;
    }
    
    int status = pclose(pipe);
    if (status != 0) {
        return "Command failed with exit code " + std::to_string(status);
    }
    
    std::string output = result.str();
    // 移除末尾的换行符
    if (!output.empty() && output.back() == '\n') {
        output.pop_back();
    }
    
    return output;
}

ftxui::Element Terminal::render(int /* height */) {
    // 渲染逻辑已迁移到 ui/terminal_ui.cpp
    // 这里保留是为了向后兼容，实际应该使用 ui::renderTerminal
    return text("");  // 占位符，实际不会使用
}

void Terminal::addOutputLine(const std::string& line, bool is_command) {
    output_lines_.push_back(TerminalLine(line, is_command));
    
    // 限制输出行数
    if (output_lines_.size() > max_output_lines_) {
        output_lines_.erase(output_lines_.begin(), 
                           output_lines_.begin() + (output_lines_.size() - max_output_lines_));
    }
}

std::string Terminal::buildPrompt() const {
    // 构建提示符字符串（用于输出历史）
    std::ostringstream oss;
    
    // 第一部分：用户名@主机名
    oss << getUsername() << "@" << getHostname();
    
    // 第二部分：时间戳
    oss << " " << getCurrentTime();
    
    // 第三部分：目录路径
    std::string dir = getCurrentDir();
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
    oss << " " << dir;
    
    // 第四部分：Git 分支（如果有）
    std::string git_branch = getGitBranch();
    if (!git_branch.empty()) {
        oss << " " << git_branch;
    }
    
    // 提示符结束符号（与输入行保持一致）
    oss << " > ";
    
    return oss.str();
}

std::string Terminal::getGitBranch() const {
    // 检测 Git 分支
    fs::path git_dir = fs::path(current_directory_) / ".git";
    
    // 检查当前目录或父目录是否有 .git
    fs::path check_dir = current_directory_;
    for (int i = 0; i < 10; ++i) {  // 最多向上查找10级
        git_dir = check_dir / ".git";
        if (fs::exists(git_dir)) {
            // 读取 HEAD 文件
            fs::path head_file = git_dir / "HEAD";
            if (fs::exists(head_file)) {
                std::ifstream file(head_file);
                if (file.is_open()) {
                    std::string line;
                    if (std::getline(file, line)) {
                        // 格式: ref: refs/heads/branch_name
                        size_t pos = line.find("refs/heads/");
                        if (pos != std::string::npos) {
                            std::string branch = line.substr(pos + 11);
                            // 移除换行符
                            if (!branch.empty() && branch.back() == '\n') {
                                branch.pop_back();
                            }
                            return branch;
                        }
                    }
                }
            }
            break;
        }
        
        // 检查父目录
        if (check_dir == check_dir.parent_path()) {
            break;  // 已到达根目录
        }
        check_dir = check_dir.parent_path();
    }
    
    return "";
}

std::string Terminal::getCurrentTime() const {
    // 获取当前时间，格式: HH:MM:SS
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << tm.tm_hour << ":"
        << std::setfill('0') << std::setw(2) << tm.tm_min << ":"
        << std::setfill('0') << std::setw(2) << tm.tm_sec;
    
    return oss.str();
}

std::string Terminal::getHostname() const {
    struct utsname info;
    if (uname(&info) == 0) {
        return info.nodename;
    }
    return "localhost";
}

std::string Terminal::getUsername() const {
    struct passwd* pw = getpwuid(getuid());
    if (pw) {
        return pw->pw_name;
    }
    const char* user = getenv("USER");
    return user ? user : "user";
}

std::string Terminal::getCurrentDir() const {
    return current_directory_;
}

ftxui::Color Terminal::getPromptColor() const {
    return Color::Green;  // 使用绿色，更像真实终端
}

ftxui::Color Terminal::getCommandColor() const {
    return Color::Green;  // 命令也使用绿色
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

} // namespace features
} // namespace pnana

