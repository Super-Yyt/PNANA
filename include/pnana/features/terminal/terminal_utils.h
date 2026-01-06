#ifndef PNANA_FEATURES_TERMINAL_UTILS_H
#define PNANA_FEATURES_TERMINAL_UTILS_H

#include <chrono>
#include <mutex>
#include <string>
#include <unordered_map>

namespace pnana {
namespace features {
namespace terminal {

// Git 分支缓存条目
struct GitBranchCacheEntry {
    std::string branch;
    std::chrono::steady_clock::time_point timestamp;
    std::string directory;
};

// 终端工具函数
class TerminalUtils {
  public:
    // 获取用户名
    static std::string getUsername();

    // 获取主机名
    static std::string getHostname();

    // 获取当前时间（格式: HH:MM:SS）
    static std::string getCurrentTime();

    // 获取 Git 分支（从指定目录向上查找）
    static std::string getGitBranch(const std::string& directory);

    // 简化路径（将 HOME 替换为 ~）
    static std::string simplifyPath(const std::string& path);

    // 截断路径（如果太长，只显示最后一部分）
    static std::string truncatePath(const std::string& path, size_t max_length = 30);

    // 清除 Git 分支缓存
    static void clearGitBranchCache();

  private:
    // 检查缓存是否有效
    static bool isGitBranchCacheValid(const GitBranchCacheEntry& entry,
                                      const std::string& directory);

    // 获取缓存的 Git 分支
    static std::string getCachedGitBranch(const std::string& directory);

    // 设置缓存的 Git 分支
    static void setCachedGitBranch(const std::string& directory, const std::string& branch);

    // Git 分支缓存（静态成员）
    static std::unordered_map<std::string, GitBranchCacheEntry> git_branch_cache_;
    static std::mutex git_cache_mutex_;
    static const std::chrono::seconds GIT_CACHE_TTL; // Git 缓存生存时间
};

} // namespace terminal
} // namespace features
} // namespace pnana

#endif // PNANA_FEATURES_TERMINAL_UTILS_H
