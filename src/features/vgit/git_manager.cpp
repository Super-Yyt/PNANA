#include "vgit/git_manager.h"
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>

namespace fs = std::filesystem;

namespace pnana {
namespace vgit {

GitManager::GitManager(const std::string& repo_path) : repo_path_(repo_path) {
    // Find repository root
    repo_root_ = getRepositoryRoot();
}

bool GitManager::isGitRepository() const {
    std::string cmd = "git -C \"" + repo_path_ + "\" rev-parse --git-dir 2>/dev/null";
    std::string result = executeGitCommand(cmd);
    return !result.empty() && result.find("fatal") == std::string::npos;
}

bool GitManager::initRepository() {
    std::string cmd = "git -C \"" + repo_path_ + "\" init";
    std::string result = executeGitCommand(cmd);
    if (!result.empty()) {
        last_error_ = "Failed to initialize git repository";
        return false;
    }
    repo_root_ = repo_path_;
    return true;
}

std::string GitManager::getRepositoryRoot() const {
    if (!isGitRepository()) {
        return "";
    }

    std::string cmd = "git -C \"" + repo_path_ + "\" rev-parse --show-toplevel 2>/dev/null";
    std::string result = executeGitCommand(cmd);
    if (!result.empty()) {
        // Remove trailing newline
        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }
        return result;
    }
    return "";
}

std::vector<GitFile> GitManager::getStatus() {
    if (!isGitRepository()) {
        return {};
    }

    refreshStatus();
    return current_status_;
}

bool GitManager::refreshStatus() {
    if (!isGitRepository()) {
        last_error_ = "Not a git repository";
        return false;
    }

    clearError();

    // Get porcelain status output
    std::string cmd = "git -C \"" + repo_root_ + "\" status --porcelain=v1";
    auto lines = executeGitCommandLines(cmd);

    current_status_.clear();

    for (const auto& line : lines) {
        if (line.length() >= 3) {
            parseStatusLine(line, current_status_);
        }
    }

    return true;
}

bool GitManager::stageFile(const std::string& path) {
    if (!isGitRepository()) {
        last_error_ = "Not a git repository";
        return false;
    }

    std::string escaped_path = escapePath(path);
    std::string cmd = "git -C \"" + repo_root_ + "\" add \"" + escaped_path + "\"";
    std::string result = executeGitCommand(cmd);

    if (!result.empty()) {
        last_error_ = "Failed to stage file: " + result;
        return false;
    }

    refreshStatus();
    return true;
}

bool GitManager::unstageFile(const std::string& path) {
    if (!isGitRepository()) {
        last_error_ = "Not a git repository";
        return false;
    }

    std::string escaped_path = escapePath(path);
    std::string cmd = "git -C \"" + repo_root_ + "\" reset HEAD \"" + escaped_path + "\"";
    std::string result = executeGitCommand(cmd);

    if (!result.empty()) {
        last_error_ = "Failed to unstage file: " + result;
        return false;
    }

    refreshStatus();
    return true;
}

bool GitManager::stageAll() {
    if (!isGitRepository()) {
        last_error_ = "Not a git repository";
        return false;
    }

    std::string cmd = "git -C \"" + repo_root_ + "\" add .";
    std::string result = executeGitCommand(cmd);

    if (!result.empty()) {
        last_error_ = "Failed to stage all files: " + result;
        return false;
    }

    refreshStatus();
    return true;
}

bool GitManager::unstageAll() {
    if (!isGitRepository()) {
        last_error_ = "Not a git repository";
        return false;
    }

    std::string cmd = "git -C \"" + repo_root_ + "\" reset HEAD";
    std::string result = executeGitCommand(cmd);

    if (!result.empty()) {
        last_error_ = "Failed to unstage all files: " + result;
        return false;
    }

    refreshStatus();
    return true;
}

bool GitManager::commit(const std::string& message) {
    if (!isGitRepository()) {
        last_error_ = "Not a git repository";
        return false;
    }

    if (message.empty()) {
        last_error_ = "Commit message cannot be empty";
        return false;
    }

    // Escape single quotes in message
    std::string escaped_message = message;
    size_t pos = 0;
    while ((pos = escaped_message.find("'", pos)) != std::string::npos) {
        escaped_message.replace(pos, 1, "'\\''");
        pos += 4;
    }

    std::string cmd = "git -C \"" + repo_root_ + "\" commit -m '" + escaped_message + "'";
    std::string result = executeGitCommand(cmd);

    if (!result.empty()) {
        last_error_ = "Failed to commit: " + result;
        return false;
    }

    refreshStatus();
    return true;
}

std::vector<GitCommit> GitManager::getRecentCommits(int count) {
    if (!isGitRepository()) {
        return {};
    }

    std::string cmd = "git -C \"" + repo_root_ + "\" log --oneline -n " + std::to_string(count) +
                      " --pretty=format:\"%H|%s|%an|%ad\" --date=short";
    auto lines = executeGitCommandLines(cmd);

    std::vector<GitCommit> commits;

    for (const auto& line : lines) {
        size_t pos1 = line.find('|');
        if (pos1 == std::string::npos)
            continue;

        size_t pos2 = line.find('|', pos1 + 1);
        if (pos2 == std::string::npos)
            continue;

        size_t pos3 = line.find('|', pos2 + 1);
        if (pos3 == std::string::npos)
            continue;

        std::string hash = line.substr(0, pos1);
        std::string message = line.substr(pos1 + 1, pos2 - pos1 - 1);
        std::string author = line.substr(pos2 + 1, pos3 - pos2 - 1);
        std::string date = line.substr(pos3 + 1);

        commits.emplace_back(hash, message, author, date);
    }

    return commits;
}

std::vector<GitBranch> GitManager::getBranches() {
    if (!isGitRepository()) {
        return {};
    }

    std::string cmd =
        "git -C \"" + repo_root_ + "\" branch -a --format=\"%(refname:short)|%(HEAD)\"";
    auto lines = executeGitCommandLines(cmd);

    std::vector<GitBranch> branches;

    for (const auto& line : lines) {
        size_t sep_pos = line.find('|');
        if (sep_pos == std::string::npos)
            continue;

        std::string name = line.substr(0, sep_pos);
        std::string head_marker = line.substr(sep_pos + 1);

        bool is_current = (head_marker == "*");
        bool is_remote = name.find("remotes/") == 0;

        // Remove remotes/ prefix for cleaner display
        if (is_remote) {
            name = name.substr(8);
        }

        branches.emplace_back(name, is_current, is_remote);
    }

    return branches;
}

bool GitManager::createBranch(const std::string& name) {
    if (!isGitRepository()) {
        last_error_ = "Not a git repository";
        return false;
    }

    if (name.empty()) {
        last_error_ = "Branch name cannot be empty";
        return false;
    }

    std::string cmd = "git -C \"" + repo_root_ + "\" checkout -b \"" + name + "\"";
    std::string result = executeGitCommand(cmd);

    if (!result.empty()) {
        last_error_ = "Failed to create branch: " + result;
        return false;
    }

    return true;
}

bool GitManager::switchBranch(const std::string& name) {
    if (!isGitRepository()) {
        last_error_ = "Not a git repository";
        return false;
    }

    std::string cmd = "git -C \"" + repo_root_ + "\" checkout \"" + name + "\"";
    std::string result = executeGitCommand(cmd);

    if (!result.empty()) {
        last_error_ = "Failed to switch branch: " + result;
        return false;
    }

    return true;
}

bool GitManager::deleteBranch(const std::string& name, bool force) {
    if (!isGitRepository()) {
        last_error_ = "Not a git repository";
        return false;
    }

    std::string cmd =
        "git -C \"" + repo_root_ + "\" branch " + (force ? "-D " : "-d ") + "\"" + name + "\"";
    std::string result = executeGitCommand(cmd);

    if (!result.empty()) {
        last_error_ = "Failed to delete branch: " + result;
        return false;
    }

    return true;
}

std::string GitManager::getCurrentBranch() {
    if (!isGitRepository()) {
        return "";
    }

    std::string cmd = "git -C \"" + repo_root_ + "\" branch --show-current";
    std::string result = executeGitCommand(cmd);

    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}

bool GitManager::push(const std::string& remote, const std::string& branch) {
    if (!isGitRepository()) {
        last_error_ = "Not a git repository";
        return false;
    }

    std::string current_branch = getCurrentBranch();
    std::string target_branch = branch.empty() ? current_branch : branch;

    std::string cmd =
        "git -C \"" + repo_root_ + "\" push \"" + remote + "\" \"" + target_branch + "\"";
    std::string result = executeGitCommand(cmd);

    if (!result.empty()) {
        last_error_ = "Failed to push: " + result;
        return false;
    }

    return true;
}

bool GitManager::pull(const std::string& remote, const std::string& branch) {
    if (!isGitRepository()) {
        last_error_ = "Not a git repository";
        return false;
    }

    std::string current_branch = getCurrentBranch();
    std::string target_branch = branch.empty() ? current_branch : branch;

    std::string cmd =
        "git -C \"" + repo_root_ + "\" pull \"" + remote + "\" \"" + target_branch + "\"";
    std::string result = executeGitCommand(cmd);

    if (!result.empty()) {
        last_error_ = "Failed to pull: " + result;
        return false;
    }

    refreshStatus();
    return true;
}

bool GitManager::fetch(const std::string& remote) {
    if (!isGitRepository()) {
        last_error_ = "Not a git repository";
        return false;
    }

    std::string cmd = "git -C \"" + repo_root_ + "\" fetch \"" + remote + "\"";
    std::string result = executeGitCommand(cmd);

    if (!result.empty()) {
        last_error_ = "Failed to fetch: " + result;
        return false;
    }

    return true;
}

std::vector<std::string> GitManager::getRemotes() {
    if (!isGitRepository()) {
        return {};
    }

    std::string cmd = "git -C \"" + repo_root_ + "\" remote";
    auto lines = executeGitCommandLines(cmd);

    return lines;
}

// Private helper methods

std::string GitManager::executeGitCommand(const std::string& command) const {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe) {
        return "Failed to execute command";
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    // Remove trailing newline
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}

std::vector<std::string> GitManager::executeGitCommandLines(const std::string& command) const {
    std::array<char, 128> buffer;
    std::vector<std::string> lines;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe) {
        return lines;
    }

    std::string current_line;
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        current_line += buffer.data();

        // Check if we have a complete line
        if (!current_line.empty() && current_line.back() == '\n') {
            // Remove trailing newline
            current_line.pop_back();
            if (!current_line.empty()) {
                lines.push_back(current_line);
            }
            current_line.clear();
        }
    }

    // Handle last line if it doesn't end with newline
    if (!current_line.empty()) {
        lines.push_back(current_line);
    }

    return lines;
}

GitFileStatus GitManager::parseStatusChar(char status_char) const {
    switch (status_char) {
        case ' ':
            return GitFileStatus::UNMODIFIED;
        case 'M':
            return GitFileStatus::MODIFIED;
        case 'A':
            return GitFileStatus::ADDED;
        case 'D':
            return GitFileStatus::DELETED;
        case 'R':
            return GitFileStatus::RENAMED;
        case 'C':
            return GitFileStatus::COPIED;
        case 'U':
            return GitFileStatus::UPDATED_BUT_UNMERGED;
        case '?':
            return GitFileStatus::UNTRACKED;
        case '!':
            return GitFileStatus::IGNORED;
        default:
            return GitFileStatus::UNMODIFIED;
    }
}

void GitManager::parseStatusLine(const std::string& line, std::vector<GitFile>& files) {
    if (line.length() < 3)
        return;

    char index_status = line[0];
    char worktree_status = line[1];
    std::string path = line.substr(2);

    // Skip leading spaces in path
    size_t path_start = path.find_first_not_of(" \t");
    if (path_start != std::string::npos) {
        path = path.substr(path_start);
    }

    // Handle renamed files (format: "R  old_name -> new_name")
    std::string old_path;
    size_t arrow_pos = path.find(" -> ");
    if (arrow_pos != std::string::npos) {
        old_path = path.substr(0, arrow_pos);
        path = path.substr(arrow_pos + 4);
    }

    // Determine if file is staged
    bool staged = (index_status != ' ' && index_status != '?');

    GitFileStatus status;
    if (index_status != ' ' && worktree_status != ' ') {
        // Both staged and unstaged changes
        status =
            (worktree_status == 'M') ? GitFileStatus::MODIFIED : parseStatusChar(worktree_status);
    } else if (index_status != ' ') {
        // Only staged changes
        status = parseStatusChar(index_status);
    } else {
        // Only unstaged changes
        status = parseStatusChar(worktree_status);
    }

    if (!old_path.empty()) {
        files.emplace_back(path, old_path, status, staged);
    } else {
        files.emplace_back(path, status, staged);
    }
}

std::string GitManager::escapePath(const std::string& path) const {
    std::string escaped = path;
    // Escape quotes
    size_t pos = 0;
    while ((pos = escaped.find("\"", pos)) != std::string::npos) {
        escaped.replace(pos, 1, "\\\"");
        pos += 2;
    }
    return escaped;
}

} // namespace vgit
} // namespace pnana
