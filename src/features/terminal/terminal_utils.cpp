#include "features/terminal/terminal_utils.h"
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <pwd.h>
#include <sstream>
#include <sys/utsname.h>
#include <unistd.h>

namespace fs = std::filesystem;

namespace pnana {
namespace features {
namespace terminal {

// 静态成员初始化
std::unordered_map<std::string, GitBranchCacheEntry> TerminalUtils::git_branch_cache_;
std::mutex TerminalUtils::git_cache_mutex_;
const std::chrono::seconds TerminalUtils::GIT_CACHE_TTL(5); // 5秒缓存

std::string TerminalUtils::getUsername() {
    struct passwd* pw = getpwuid(getuid());
    if (pw) {
        return pw->pw_name;
    }
    const char* user = getenv("USER");
    return user ? user : "user";
}

std::string TerminalUtils::getHostname() {
    struct utsname info;
    if (uname(&info) == 0) {
        return info.nodename;
    }
    return "localhost";
}

std::string TerminalUtils::getCurrentTime() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << tm.tm_hour << ":" << std::setfill('0')
        << std::setw(2) << tm.tm_min << ":" << std::setfill('0') << std::setw(2) << tm.tm_sec;

    return oss.str();
}

void TerminalUtils::clearGitBranchCache() {
    std::lock_guard<std::mutex> lock(git_cache_mutex_);
    git_branch_cache_.clear();
}

bool TerminalUtils::isGitBranchCacheValid(const GitBranchCacheEntry& entry,
                                          const std::string& directory) {
    auto now = std::chrono::steady_clock::now();
    return (now - entry.timestamp) < GIT_CACHE_TTL && entry.directory == directory;
}

std::string TerminalUtils::getCachedGitBranch(const std::string& directory) {
    std::lock_guard<std::mutex> lock(git_cache_mutex_);
    auto it = git_branch_cache_.find(directory);
    if (it != git_branch_cache_.end() && isGitBranchCacheValid(it->second, directory)) {
        return it->second.branch;
    }
    return "";
}

void TerminalUtils::setCachedGitBranch(const std::string& directory, const std::string& branch) {
    std::lock_guard<std::mutex> lock(git_cache_mutex_);
    GitBranchCacheEntry entry{branch, std::chrono::steady_clock::now(), directory};
    git_branch_cache_[directory] = entry;
}

std::string TerminalUtils::getGitBranch(const std::string& directory) {
    // 先检查缓存
    std::string cached_branch = getCachedGitBranch(directory);
    if (!cached_branch.empty() || cached_branch == "") {
        return cached_branch;
    }

    fs::path git_dir = fs::path(directory) / ".git";
    fs::path check_dir = directory;

    // 检查当前目录或父目录是否有 .git
    for (int i = 0; i < 10; ++i) { // 最多向上查找10级
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
                            // 缓存结果
                            setCachedGitBranch(directory, branch);
                            return branch;
                        }
                    }
                }
            }
            break;
        }

        // 检查父目录
        if (check_dir == check_dir.parent_path()) {
            break; // 已到达根目录
        }
        check_dir = check_dir.parent_path();
    }

    // 缓存空结果
    setCachedGitBranch(directory, "");
    return "";
}

std::string TerminalUtils::simplifyPath(const std::string& path) {
    const char* home = getenv("HOME");
    if (home && path.find(home) == 0) {
        return "~" + path.substr(strlen(home));
    }
    return path;
}

std::string TerminalUtils::truncatePath(const std::string& path, size_t max_length) {
    if (path.length() <= max_length) {
        return path;
    }

    size_t last_slash = path.find_last_of('/');
    if (last_slash != std::string::npos && last_slash < path.length() - 1) {
        return "..." + path.substr(last_slash);
    }
    return path;
}

} // namespace terminal
} // namespace features
} // namespace pnana
