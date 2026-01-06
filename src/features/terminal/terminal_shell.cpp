#include "features/terminal/terminal_shell.h"
#include "features/terminal/terminal_parser.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <sstream>
#include <unistd.h>

namespace pnana {
namespace features {
namespace terminal {

// 静态成员初始化
std::unordered_map<std::string, CommandCacheEntry> ShellCommandExecutor::command_cache_;
std::mutex ShellCommandExecutor::cache_mutex_;
size_t ShellCommandExecutor::max_cache_size_ = 100;
const std::chrono::seconds ShellCommandExecutor::CACHE_TTL(30); // 30秒缓存

std::string ShellCommandExecutor::escapeCommand(const std::string& command) {
    std::string escaped = command;
    size_t pos = 0;

    // 转义双引号
    while ((pos = escaped.find('"', pos)) != std::string::npos) {
        escaped.replace(pos, 1, "\\\"");
        pos += 2;
    }

    // 转义反引号
    pos = 0;
    while ((pos = escaped.find('`', pos)) != std::string::npos) {
        escaped.replace(pos, 1, "\\`");
        pos += 2;
    }

    return escaped;
}

std::string ShellCommandExecutor::escapeDirectory(const std::string& directory) {
    std::string escaped = directory;
    size_t pos = 0;
    while ((pos = escaped.find('"', pos)) != std::string::npos) {
        escaped.replace(pos, 1, "\\\"");
        pos += 2;
    }
    return escaped;
}

std::string ShellCommandExecutor::executeSystemCommand(const std::string& command,
                                                       const std::vector<std::string>& args,
                                                       const std::string& current_directory) {
    // 构建完整命令字符串（包含所有参数）
    // 这个方法现在主要用于向后兼容，实际应该使用 executeShellCommand
    std::ostringstream cmd_stream;
    cmd_stream << command;
    for (const auto& arg : args) {
        // 如果参数包含空格或特殊字符，需要加引号
        if (arg.find(' ') != std::string::npos || arg.find('|') != std::string::npos ||
            arg.find('>') != std::string::npos || arg.find('<') != std::string::npos ||
            arg.find('&') != std::string::npos || arg.find('$') != std::string::npos) {
            // 转义引号
            std::string escaped = arg;
            size_t pos = 0;
            while ((pos = escaped.find('"', pos)) != std::string::npos) {
                escaped.replace(pos, 1, "\\\"");
                pos += 2;
            }
            cmd_stream << " \"" << escaped << "\"";
        } else {
            cmd_stream << " " << arg;
        }
    }

    std::string cmd_str = cmd_stream.str();

    // 直接使用 executeShellCommand，这样可以统一处理所有命令
    return executeShellCommand(cmd_str, false, current_directory);
}

void ShellCommandExecutor::clearCache() {
    std::lock_guard<std::mutex> lock(cache_mutex_);
    command_cache_.clear();
}

void ShellCommandExecutor::setCacheSize(size_t max_size) {
    std::lock_guard<std::mutex> lock(cache_mutex_);
    max_cache_size_ = max_size;

    // 如果当前缓存超过新限制，清理最旧的条目
    if (command_cache_.size() > max_cache_size_) {
        // 简单清理策略：移除一半的条目
        auto it = command_cache_.begin();
        size_t to_remove = command_cache_.size() - max_cache_size_;
        for (size_t i = 0; i < to_remove && it != command_cache_.end(); ++i) {
            it = command_cache_.erase(it);
        }
    }
}

bool ShellCommandExecutor::isInteractiveCommand(const std::string& command) {
    // 检查是否是需要交互式处理的命令
    std::vector<std::string> args = CommandParser::parse(command);
    if (args.empty()) {
        return false;
    }

    std::string cmd = args[0];

    // Python 相关命令
    if (cmd == "python" || cmd == "python3" || cmd == "python2") {
        // 检查是否有 -i 参数（交互式）或没有脚本文件参数
        bool has_script = false;
        bool has_interactive = false;
        bool has_command = false;

        for (size_t i = 1; i < args.size(); ++i) {
            const std::string& arg = args[i];
            if (arg == "-i") {
                has_interactive = true;
            } else if (arg == "-c") {
                has_command = true;
            } else if (arg[0] != '-') {
                // 非选项参数，可能是脚本文件
                has_script = true;
            }
        }

        // 如果没有脚本文件、没有-c命令，或者明确指定了 -i，就认为是交互式
        return !has_script && !has_command && (has_interactive || args.size() == 1);
    }

    // Node.js
    if (cmd == "node" || cmd == "nodejs") {
        // 检查参数，如果没有脚本文件或有-e参数，就是交互式
        bool has_script = false;
        bool has_eval = false;

        for (size_t i = 1; i < args.size(); ++i) {
            const std::string& arg = args[i];
            if (arg == "-e" || arg == "--eval") {
                has_eval = true;
            } else if (arg[0] != '-') {
                has_script = true;
            }
        }

        return !has_script && !has_eval;
    }

    // 其他交互式程序
    return (cmd == "lua" || cmd == "ruby" || cmd == "irb" || cmd == "ghci" || cmd == "mysql" ||
            cmd == "psql" || cmd == "sqlite3" || cmd == "bc" || cmd == "dc");
}

bool ShellCommandExecutor::isPseudoTerminalCommand(const std::string& command) {
    // 检查是否需要伪终端支持的命令（真正的交互式程序）
    std::vector<std::string> args = CommandParser::parse(command);
    if (args.empty()) {
        return false;
    }

    std::string cmd = args[0];

    // 这些程序需要真正的伪终端支持来正确工作
    return (cmd == "python" || cmd == "python3" || cmd == "python2" || cmd == "node" ||
            cmd == "nodejs" || cmd == "lua" || cmd == "mysql" || cmd == "psql" ||
            cmd == "sqlite3" || cmd == "bc" || cmd == "dc" || cmd == "htop" || cmd == "vim" ||
            cmd == "nano" || cmd == "emacs" || cmd == "less" || cmd == "more");
}

std::string ShellCommandExecutor::executeInteractiveCommand(const std::string& command,
                                                            bool background,
                                                            const std::string& current_directory) {
    // 对于交互式命令，我们需要特殊处理以支持更好的用户体验
    std::vector<std::string> args = CommandParser::parse(command);
    if (args.empty()) {
        return "";
    }

    std::string cmd = args[0];
    std::string full_cmd;

    // 构建基本命令
    if (cmd == "python" || cmd == "python3" || cmd == "python2") {
        // 对于Python，确保使用交互式模式
        full_cmd = "cd \"";
        full_cmd += escapeDirectory(current_directory);
        full_cmd += "\" && ";

        // 检查是否已经有 -i 参数
        bool has_interactive = false;
        for (size_t i = 1; i < args.size(); ++i) {
            if (args[i] == "-i") {
                has_interactive = true;
                break;
            }
        }

        // 如果没有 -i 参数，添加它以确保交互式模式
        full_cmd += cmd;
        if (!has_interactive) {
            full_cmd += " -i";
        }

        // 添加其他参数
        for (size_t i = 1; i < args.size(); ++i) {
            full_cmd += " ";
            full_cmd += args[i];
        }

        // 设置PYTHONPATH环境变量以包含当前目录
        full_cmd =
            "PYTHONPATH=\"" + escapeDirectory(current_directory) + ":$PYTHONPATH\" " + full_cmd;
    } else {
        // 其他交互式命令的标准处理
        full_cmd = "cd \"";
        full_cmd += escapeDirectory(current_directory);
        full_cmd += "\" && ";
        full_cmd += command;
    }

    if (background) {
        full_cmd += " &";
    }

    // 对于需要伪终端的命令，尝试使用更好的方法
    if (isPseudoTerminalCommand(command)) {
        return executePseudoTerminalCommand(full_cmd, current_directory);
    }

    // 对于交互式命令，优先使用更好的shell
    std::vector<std::string> preferred_shells = {"/bin/zsh",      "/usr/bin/zsh", "/bin/bash",
                                                 "/usr/bin/bash", "/bin/sh",      "/usr/bin/sh"};
    std::string shell_path = "/bin/sh"; // 默认值

    for (const auto& shell : preferred_shells) {
        if (access(shell.c_str(), X_OK) == 0) {
            shell_path = shell;
            break;
        }
    }

    std::string shell_cmd = shell_path + " -c \"";
    shell_cmd += escapeCommand(full_cmd);
    shell_cmd += "\"";

    // 执行命令并捕获输出
    const size_t BUFFER_SIZE = 16384;
    std::vector<char> buffer(BUFFER_SIZE);
    std::string result;
    result.reserve(4096);

    FILE* pipe = popen(shell_cmd.c_str(), "r");
    if (!pipe) {
        std::string error_msg = "Error: Failed to execute interactive command '" + command + "'\n";
        error_msg += "This command requires interactive terminal support.\n";
        error_msg += "Try using a different shell or check if the program is installed.\n";
        return error_msg;
    }

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    int status = pclose(pipe);

    if (status != 0 && result.empty()) {
        result = "Interactive command failed with exit code " + std::to_string(status);
    }

    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}

std::string ShellCommandExecutor::executePseudoTerminalCommand(
    const std::string& command, const std::string& current_directory) {
    // 对于需要伪终端的命令，尝试使用script命令来提供更好的终端模拟
    // script命令可以创建一个伪终端环境，更好地支持交互式程序

    std::vector<std::string> preferred_shells = {"/bin/zsh",      "/usr/bin/zsh", "/bin/bash",
                                                 "/usr/bin/bash", "/bin/sh",      "/usr/bin/sh"};
    std::string shell_path = "/bin/sh";

    for (const auto& shell : preferred_shells) {
        if (access(shell.c_str(), X_OK) == 0) {
            shell_path = shell;
            break;
        }
    }

    // 使用script命令包装以提供伪终端支持
    std::string script_cmd = "cd \"";
    script_cmd += escapeDirectory(current_directory);
    script_cmd += "\" && script -q -c \"";
    script_cmd += escapeCommand(command);
    script_cmd += "\" /dev/null";

    std::string full_cmd = shell_path + " -c \"";
    full_cmd += escapeCommand(script_cmd);
    full_cmd += "\"";

    // 执行命令并捕获输出
    const size_t BUFFER_SIZE = 16384;
    std::vector<char> buffer(BUFFER_SIZE);
    std::string result;
    result.reserve(4096);

    FILE* pipe = popen(full_cmd.c_str(), "r");
    if (!pipe) {
        // 如果script不可用，尝试直接执行
        std::string direct_cmd = "cd \"";
        direct_cmd += escapeDirectory(current_directory);
        direct_cmd += "\" && ";
        direct_cmd += command;

        std::string shell_cmd = shell_path + " -c \"";
        shell_cmd += escapeCommand(direct_cmd);
        shell_cmd += "\"";

        FILE* direct_pipe = popen(shell_cmd.c_str(), "r");
        if (!direct_pipe) {
            std::string error_msg =
                "Error: Failed to execute pseudo-terminal command '" + command + "'\n";
            error_msg += "This command requires advanced terminal emulation.\n";
            error_msg += "Try using 'script' command or check system configuration.\n";
            return error_msg;
        }
        pipe = direct_pipe;
    }

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    int status = pclose(pipe);

    // 清理script的输出（移除开头和结尾的script信息）
    if (!result.empty()) {
        // 移除可能的script启动信息
        size_t script_start = result.find("Script started");
        if (script_start != std::string::npos) {
            size_t newline_pos = result.find('\n', script_start);
            if (newline_pos != std::string::npos) {
                result = result.substr(newline_pos + 1);
            }
        }

        // 移除结尾的script结束信息
        size_t script_end = result.rfind("Script done");
        if (script_end != std::string::npos) {
            result = result.substr(0, script_end);
        }

        // 移除末尾的换行符
        while (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }
    }

    if (status != 0 && result.empty()) {
        result = "Pseudo-terminal command failed with exit code " + std::to_string(status);
    }

    return result;
}

std::string ShellCommandExecutor::getCacheKey(const std::string& command,
                                              const std::string& directory) {
    return directory + "|" + command;
}

bool ShellCommandExecutor::isCacheValid(const CommandCacheEntry& entry) {
    auto now = std::chrono::steady_clock::now();
    return (now - entry.timestamp) < CACHE_TTL;
}

void ShellCommandExecutor::addToCache(const std::string& key, const std::string& output,
                                      int exit_code) {
    std::lock_guard<std::mutex> lock(cache_mutex_);

    // 如果缓存已满，清理过期的条目
    if (command_cache_.size() >= max_cache_size_) {
        auto it = command_cache_.begin();
        while (it != command_cache_.end()) {
            if (!isCacheValid(it->second)) {
                it = command_cache_.erase(it);
            } else {
                ++it;
            }
        }

        // 如果仍然超过限制，移除最旧的条目
        if (command_cache_.size() >= max_cache_size_) {
            auto oldest_it = command_cache_.begin();
            for (auto it = command_cache_.begin(); it != command_cache_.end(); ++it) {
                if (it->second.timestamp < oldest_it->second.timestamp) {
                    oldest_it = it;
                }
            }
            command_cache_.erase(oldest_it);
        }
    }

    CommandCacheEntry entry{output, std::chrono::steady_clock::now(), exit_code};
    command_cache_[key] = entry;
}

std::string ShellCommandExecutor::getFromCache(const std::string& key, int& exit_code) {
    std::lock_guard<std::mutex> lock(cache_mutex_);
    auto it = command_cache_.find(key);
    if (it != command_cache_.end() && isCacheValid(it->second)) {
        exit_code = it->second.exit_code;
        return it->second.output;
    }
    return "";
}

std::string ShellCommandExecutor::executeShellCommand(const std::string& command, bool background,
                                                      const std::string& current_directory) {
    // 检查是否是交互式命令
    if (isInteractiveCommand(command)) {
        return executeInteractiveCommand(command, background, current_directory);
    }

    // 检查缓存（仅对非后台命令且不包含特殊字符的简单命令）
    if (!background && command.find_first_of("$`\\|&><;") == std::string::npos) {
        std::string cache_key = getCacheKey(command, current_directory);
        int cached_exit_code = 0;
        std::string cached_result = getFromCache(cache_key, cached_exit_code);
        if (!cached_result.empty()) {
            return cached_result;
        }
    }

    // 使用 /bin/sh -c 来执行命令，支持所有 shell 特性
    // 这是最通用的方式，可以执行所有 Linux 命令和参数

    // 构建完整的 shell 命令
    // 格式: cd "directory" && command [&]
    std::string full_cmd = "cd \"";
    full_cmd += escapeDirectory(current_directory);
    full_cmd += "\" && ";

    // 对于简单命令，直接执行
    // 对于复杂命令（包含引号、变量等），需要适当转义
    std::string cmd_to_execute = command;

    // 检查命令是否包含需要转义的字符
    bool needs_escaping =
        (command.find('"') != std::string::npos || command.find('$') != std::string::npos ||
         command.find('`') != std::string::npos || command.find('\\') != std::string::npos);

    if (needs_escaping) {
        // 需要转义特殊字符
        cmd_to_execute = escapeCommand(command);
        // 但保留 $ 用于环境变量展开（在 shell 中处理）
        // 重新处理 $，不转义它
        size_t pos = 0;
        while ((pos = cmd_to_execute.find("\\$", pos)) != std::string::npos) {
            cmd_to_execute.replace(pos, 2, "$");
            pos++;
        }
        full_cmd += "\"";
        full_cmd += cmd_to_execute;
        full_cmd += "\"";
    } else {
        // 简单命令，直接使用
        full_cmd += command;
    }

    if (background) {
        full_cmd += " &";
    }

    // 优先级：zsh > bash > sh
    std::vector<std::string> preferred_shells = {"/bin/zsh",      "/usr/bin/zsh", "/bin/bash",
                                                 "/usr/bin/bash", "/bin/sh",      "/usr/bin/sh"};
    std::string shell_path = "/bin/sh"; // 默认值

    for (const auto& shell : preferred_shells) {
        if (access(shell.c_str(), X_OK) == 0) {
            shell_path = shell;
            break;
        }
    }

    std::string shell_cmd = shell_path + " -c \"";
    std::string escaped_full_cmd = escapeCommand(full_cmd);
    // 恢复 $ 用于环境变量
    size_t pos = 0;
    while ((pos = escaped_full_cmd.find("\\$", pos)) != std::string::npos) {
        escaped_full_cmd.replace(pos, 2, "$");
        pos++;
    }
    shell_cmd += escaped_full_cmd;
    shell_cmd += "\"";

    // 执行命令并捕获输出（包括标准错误）
    std::string exec_cmd = shell_cmd + " 2>&1"; // 2>&1 将标准错误重定向到标准输出
    FILE* pipe = popen(exec_cmd.c_str(), "r");
    if (!pipe) {
        std::string error_msg = "Error: Failed to execute command '" + command + "'\n";
        error_msg += "Possible causes:\n";
        error_msg += "  - Command not found in PATH\n";
        error_msg += "  - Permission denied\n";
        error_msg += "  - System shell not available\n";
        return error_msg;
    }

    // 使用更大的缓冲区提高读取效率
    const size_t BUFFER_SIZE = 16384; // 16KB buffer
    std::vector<char> buffer(BUFFER_SIZE);
    std::string result;

    // 预分配字符串以提高性能
    result.reserve(4096);

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    int status = pclose(pipe);

    // 如果命令失败且没有输出，显示错误信息
    if (status != 0 && result.empty()) {
        result = "Command failed with exit code " + std::to_string(status);
    }

    // 移除末尾的换行符（如果存在）
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    // 缓存结果（仅对非后台命令且不包含特殊字符的简单命令）
    if (!background && command.find_first_of("$`\\|&><;") == std::string::npos) {
        std::string cache_key = getCacheKey(command, current_directory);
        addToCache(cache_key, result, status);
    }

    return result;
}

} // namespace terminal
} // namespace features
} // namespace pnana
