#ifndef PNANA_FEATURES_TERMINAL_SHELL_H
#define PNANA_FEATURES_TERMINAL_SHELL_H

#include <chrono>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace pnana {
namespace features {
namespace terminal {

// 命令缓存条目
struct CommandCacheEntry {
    std::string output;
    std::chrono::steady_clock::time_point timestamp;
    int exit_code;
};

// Shell 命令执行器
class ShellCommandExecutor {
  public:
    // 执行系统命令（通过 popen）
    static std::string executeSystemCommand(const std::string& command,
                                            const std::vector<std::string>& args,
                                            const std::string& current_directory);

    // 执行 Shell 命令（支持所有 shell 特性）
    static std::string executeShellCommand(const std::string& command, bool background,
                                           const std::string& current_directory);

    // 清除命令缓存
    static void clearCache();

    // 设置缓存大小限制
    static void setCacheSize(size_t max_size);

    // 检查是否是交互式命令
    static bool isInteractiveCommand(const std::string& command);

    // 执行交互式命令
    static std::string executeInteractiveCommand(const std::string& command, bool background,
                                                 const std::string& current_directory);

    // 检查是否需要伪终端支持的命令
    static bool isPseudoTerminalCommand(const std::string& command);

    // 执行需要伪终端的命令
    static std::string executePseudoTerminalCommand(const std::string& command,
                                                    const std::string& current_directory);

  private:
    // 转义命令字符串中的特殊字符
    static std::string escapeCommand(const std::string& command);

    // 转义目录路径
    static std::string escapeDirectory(const std::string& directory);

    // 获取缓存键
    static std::string getCacheKey(const std::string& command, const std::string& directory);

    // 检查缓存是否有效
    static bool isCacheValid(const CommandCacheEntry& entry);

    // 添加到缓存
    static void addToCache(const std::string& key, const std::string& output, int exit_code);

    // 从缓存获取
    static std::string getFromCache(const std::string& key, int& exit_code);

    // 命令缓存（静态成员）
    static std::unordered_map<std::string, CommandCacheEntry> command_cache_;
    static std::mutex cache_mutex_;
    static size_t max_cache_size_;
    static const std::chrono::seconds CACHE_TTL; // 缓存生存时间
};

} // namespace terminal
} // namespace features
} // namespace pnana

#endif // PNANA_FEATURES_TERMINAL_SHELL_H
