#include "vgit/git_panel.h"
#include "ui/icons.h"
#include <algorithm>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <future>
#include <mutex>
#include <sstream>

using namespace ftxui;
using namespace pnana::ui::icons;

namespace pnana {
namespace vgit {

GitPanel::GitPanel(ui::Theme& theme, const std::string& repo_path)
    : theme_(theme), git_manager_(std::make_unique<GitManager>(repo_path)) {
    // 延迟加载git数据，只在面板显示时才加载
}

Component GitPanel::getComponent() {
    // 总是重新构建组件以确保状态变化被反映
    main_component_ = buildMainComponent();
    return main_component_;
}

void GitPanel::onShow() {
    // 延迟加载git数据，只在第一次显示时加载
    if (!data_loaded_) {
        refreshData();
        data_loaded_ = true;
    }
    selected_index_ = 0;
    scroll_offset_ = 0;
    clearSelection();
}

void GitPanel::onHide() {
    // Cleanup if needed
}

bool GitPanel::onKeyPress(Event event) {
    if (!visible_)
        return false;

    if (event == Event::Escape) {
        hide();
        return true;
    }

    switch (current_mode_) {
        case GitPanelMode::STATUS:
            return handleStatusModeKey(event);
        case GitPanelMode::COMMIT:
            return handleCommitModeKey(event);
        case GitPanelMode::BRANCH:
            return handleBranchModeKey(event);
        case GitPanelMode::REMOTE:
            return handleRemoteModeKey(event);
    }

    return false;
}

void GitPanel::refreshData() {
    if (data_loading_) {
        return; // 如果正在加载，忽略请求
    }

    data_loading_ = true;

    // 异步加载git数据
    auto future = std::async(std::launch::async, [this]() {
        try {
            auto files = git_manager_->getStatus();
            auto branches = git_manager_->getBranches();
            auto error = git_manager_->getLastError();
            git_manager_->clearError();

            // 在主线程中更新UI数据
            std::lock_guard<std::mutex> lock(data_mutex_);
            files_ = std::move(files);
            branches_ = std::move(branches);
            error_message_ = std::move(error);
            data_loading_ = false;
            data_loaded_ = true;
        } catch (...) {
            std::lock_guard<std::mutex> lock(data_mutex_);
            data_loading_ = false;
            error_message_ = "Failed to load git data";
        }
    });
}

void GitPanel::switchMode(GitPanelMode mode) {
    current_mode_ = mode;
    selected_index_ = 0;
    scroll_offset_ = 0;
    clearSelection();

    if (mode == GitPanelMode::COMMIT) {
        commit_message_.clear();
    } else if (mode == GitPanelMode::BRANCH) {
        branch_name_.clear();
    }
}

void GitPanel::toggleFileSelection(size_t index) {
    if (index >= files_.size())
        return;

    auto it = std::find(selected_files_.begin(), selected_files_.end(), index);
    if (it != selected_files_.end()) {
        selected_files_.erase(it);
    } else {
        selected_files_.push_back(index);
    }
}

void GitPanel::clearSelection() {
    selected_files_.clear();
}

void GitPanel::selectAll() {
    selected_files_.clear();
    for (size_t i = 0; i < files_.size(); ++i) {
        selected_files_.push_back(i);
    }
}

void GitPanel::performStageSelected() {
    bool success = true;
    for (size_t index : selected_files_) {
        if (index < files_.size()) {
            if (!git_manager_->stageFile(files_[index].path)) {
                success = false;
                break;
            }
        }
    }
    if (success) {
        refreshData();
        clearSelection();
    }
}

void GitPanel::performUnstageSelected() {
    bool success = true;
    for (size_t index : selected_files_) {
        if (index < files_.size()) {
            if (!git_manager_->unstageFile(files_[index].path)) {
                success = false;
                break;
            }
        }
    }
    if (success) {
        refreshData();
        clearSelection();
    }
}

void GitPanel::performCommit() {
    if (commit_message_.empty())
        return;

    if (git_manager_->commit(commit_message_)) {
        commit_message_.clear();
        refreshData();
        switchMode(GitPanelMode::STATUS);
    }
}

void GitPanel::performPush() {
    if (git_manager_->push()) {
        refreshData();
    }
}

void GitPanel::performPull() {
    if (git_manager_->pull()) {
        refreshData();
    }
}

void GitPanel::performCreateBranch() {
    if (branch_name_.empty())
        return;

    if (git_manager_->createBranch(branch_name_)) {
        branch_name_.clear();
        refreshData();
        switchMode(GitPanelMode::STATUS);
    }
}

void GitPanel::performSwitchBranch() {
    if (selected_index_ >= branches_.size())
        return;

    if (git_manager_->switchBranch(branches_[selected_index_].name)) {
        refreshData();
        switchMode(GitPanelMode::STATUS);
    }
}

Element GitPanel::renderHeader() {
    auto& colors = theme_.getColors();

    std::string header_text =
        std::string(pnana::ui::icons::GIT) + " Git │ " + getModeTitle(current_mode_) +
        " │ Repository: " +
        (git_manager_->getRepositoryRoot().empty() ? "." : git_manager_->getRepositoryRoot());

    return text(header_text) | color(colors.foreground) | bgcolor(colors.menubar_bg);
}

Element GitPanel::renderTabs() {
    auto& colors = theme_.getColors();

    auto makeTab = [&](const std::string& label, GitPanelMode /*mode*/, bool active) {
        if (active) {
            return text(" " + label + " ") | bgcolor(colors.selection) | color(colors.foreground) |
                   bold;
        } else {
            return text(" " + label + " ") | color(colors.menubar_fg);
        }
    };

    Elements elements = {
        makeTab("Status", GitPanelMode::STATUS, current_mode_ == GitPanelMode::STATUS),
        text(" ") | color(colors.comment),
        makeTab("Commit", GitPanelMode::COMMIT, current_mode_ == GitPanelMode::COMMIT),
        text(" ") | color(colors.comment),
        makeTab("Branch", GitPanelMode::BRANCH, current_mode_ == GitPanelMode::BRANCH),
        text(" ") | color(colors.comment),
        makeTab("Remote", GitPanelMode::REMOTE, current_mode_ == GitPanelMode::REMOTE)};
    return hbox(std::move(elements)) | center;
}

Element GitPanel::renderStatusPanel() {
    auto& colors = theme_.getColors();

    // 如果数据正在加载，显示加载指示器
    if (data_loading_) {
        return vbox({text("Loading git status...") | color(colors.comment) | center,
                     text("Please wait...") | color(colors.menubar_fg) | center}) |
               center | size(HEIGHT, EQUAL, 10);
    }

    Elements file_elements;

    // Header
    Elements header_elements = {
        text("Status") | color(colors.foreground), text(" | "),
        text(std::to_string(files_.size()) + " files") | color(colors.menubar_fg)};
    file_elements.push_back(hbox(std::move(header_elements)));
    file_elements.push_back(separator());

    // File list
    size_t start = scroll_offset_;
    size_t end = std::min(start + 20, files_.size()); // Show 20 files at a time

    for (size_t i = start; i < end; ++i) {
        bool is_selected =
            std::find(selected_files_.begin(), selected_files_.end(), i) != selected_files_.end();
        bool is_highlighted = (i == selected_index_);
        file_elements.push_back(renderFileItem(files_[i], i, is_selected || is_highlighted));
    }

    if (files_.empty()) {
        file_elements.push_back(text("No changes") | color(colors.menubar_fg) | center);
    }

    return vbox(std::move(file_elements));
}

Element GitPanel::renderCommitPanel() {
    auto& colors = theme_.getColors();

    Elements elements;

    // Header
    elements.push_back(text("Commit Changes") | color(colors.foreground));
    elements.push_back(separator());

    // Staged files summary
    size_t staged_count = 0;
    for (const auto& file : files_) {
        if (file.staged)
            staged_count++;
    }

    Elements summary_elements = {text("Staged files: ") | color(colors.menubar_fg),
                                 text(std::to_string(staged_count)) | color(colors.foreground)};
    elements.push_back(hbox(std::move(summary_elements)));

    elements.push_back(separator());

    // Commit message input
    elements.push_back(text("Commit message:") | color(colors.menubar_fg));
    elements.push_back(text(commit_message_) | color(colors.foreground) | border);

    if (staged_count == 0) {
        elements.push_back(text("No staged changes to commit") | color(Color::Red) | center);
    }

    return vbox(std::move(elements));
}

Element GitPanel::renderBranchPanel() {
    auto& colors = theme_.getColors();

    // 如果数据正在加载，显示加载指示器
    if (data_loading_) {
        return vbox({text("Loading branches...") | color(colors.comment) | center,
                     text("Please wait...") | color(colors.menubar_fg) | center}) |
               center | size(HEIGHT, EQUAL, 10);
    }

    Elements branch_elements;

    // Header
    Elements header_elements = {
        text("Branches") | color(colors.foreground), text(" | "),
        text(std::to_string(branches_.size()) + " branches") | color(colors.menubar_fg)};
    branch_elements.push_back(hbox(std::move(header_elements)));
    branch_elements.push_back(separator());

    // Current branch
    std::string current_branch = git_manager_->getCurrentBranch();
    if (!current_branch.empty()) {
        Elements current_elements = {text("Current: ") | color(colors.menubar_fg),
                                     text(current_branch) | color(colors.foreground) | bold};
        branch_elements.push_back(hbox(std::move(current_elements)));
        branch_elements.push_back(separator());
    }

    // Branch list
    size_t start = scroll_offset_;
    size_t end = std::min(start + 15, branches_.size()); // Show 15 branches at a time

    for (size_t i = start; i < end; ++i) {
        bool is_highlighted = (i == selected_index_);
        branch_elements.push_back(renderBranchItem(branches_[i], i, is_highlighted));
    }

    if (branches_.empty()) {
        branch_elements.push_back(text("No branches found") | color(colors.menubar_fg) | center);
    }

    // New branch input
    branch_elements.push_back(separator());
    branch_elements.push_back(text("Create new branch:") | color(colors.menubar_fg));
    branch_elements.push_back(text(branch_name_) | color(colors.foreground) | border);

    return vbox(std::move(branch_elements));
}

Element GitPanel::renderRemotePanel() {
    auto& colors = theme_.getColors();

    Elements elements;

    // Header
    elements.push_back(text("Remote Operations") | color(colors.foreground));
    elements.push_back(separator());

    // Current branch info
    std::string current_branch = git_manager_->getCurrentBranch();
    if (!current_branch.empty()) {
        Elements branch_elements = {text("Current branch: ") | color(colors.menubar_fg),
                                    text(current_branch) | color(colors.foreground)};
        elements.push_back(hbox(std::move(branch_elements)));
        elements.push_back(separator());
    }

    // Remote operations buttons (text-based for now)
    elements.push_back(text("Available operations:") | color(colors.menubar_fg));
    elements.push_back(text("  p - Push to remote") | color(colors.foreground));
    elements.push_back(text("  l - Pull from remote") | color(colors.foreground));
    elements.push_back(text("  f - Fetch from remote") | color(colors.foreground));

    return vbox(std::move(elements));
}

Element GitPanel::renderFileItem(const GitFile& file, size_t /*index*/, bool is_selected) {
    auto& colors = theme_.getColors();

    // Strict file browser-like styling with git-specific information
    Color item_color = colors.foreground;
    std::string status_icon = getStatusIcon(file.status);
    std::string status_text = getStatusText(file.status);

    // Git status indicator (like expand icon in file browser)
    std::string git_indicator = file.staged ? pnana::ui::icons::SAVED : pnana::ui::icons::MODIFIED;

    // File name (primary element like in file browser)
    std::string display_name = file.path;

    // Status info (like additional metadata in file browser)
    std::string metadata = status_text;

    // Build row elements following file browser exact pattern:
    // [space][git_indicator][space][status_icon][space][filename][space][metadata]
    Elements row_elements = {
        text(" "), // Leading space (like file browser)
        text(git_indicator) |
            color(file.staged ? colors.success
                              : colors.comment), // Git staged indicator (like expand icon)
        text(" "),                               // Space (like file browser)
        text(status_icon) | color(item_color),   // Status icon (like file icon)
        text(" "),                               // Space (like file browser)
        text(display_name) | color(item_color),  // File path (like file name)
        text(" "),                               // Space (like file browser)
        text(metadata) | color(colors.comment)   // Status metadata (like file info)
    };

    auto item_text = hbox(std::move(row_elements));

    // Selection highlighting exactly like file browser
    if (is_selected) {
        item_text = item_text | bgcolor(colors.selection) | bold;
    } else {
        item_text = item_text | bgcolor(colors.background);
    }

    return item_text;
}

Element GitPanel::renderBranchItem(const GitBranch& branch, size_t /*index*/, bool is_selected) {
    auto& colors = theme_.getColors();

    // Strict file browser-like styling for branches
    Color item_color = colors.foreground;

    // Branch status indicator (like expand icon in file browser)
    std::string branch_indicator;
    if (branch.is_current) {
        branch_indicator = pnana::ui::icons::CHECK_CIRCLE; // Current branch indicator
    } else if (branch.is_remote) {
        branch_indicator = pnana::ui::icons::GIT_REMOTE; // Remote branch indicator
    } else {
        branch_indicator = pnana::ui::icons::GIT_BRANCH; // Local branch indicator
    }

    // Branch icon (like file icon in file browser)
    std::string branch_icon = ""; // Git branch icon

    // Branch name (primary element like file name)
    std::string display_name = branch.name;

    // Branch metadata (like file info in file browser)
    std::string metadata = branch.is_remote ? "remote" : "local";

    // Build row elements following file browser exact pattern:
    // [space][branch_indicator][space][branch_icon][space][branch_name][space][metadata]
    Elements row_elements = {
        text(" "), // Leading space (like file browser)
        text(branch_indicator) |
            color(branch.is_current ? colors.success : colors.comment), // Branch status indicator
        text(" "),                                                      // Space (like file browser)
        text(branch_icon) | color(item_color),  // Branch icon (like file icon)
        text(" "),                              // Space (like file browser)
        text(display_name) | color(item_color), // Branch name (like file name)
        text(" "),                              // Space (like file browser)
        text(metadata) | color(colors.comment)  // Branch metadata (like file info)
    };

    auto item_text = hbox(std::move(row_elements));

    // Selection highlighting exactly like file browser
    if (is_selected) {
        item_text = item_text | bgcolor(colors.selection) | bold;
    } else {
        item_text = item_text | bgcolor(colors.background);
    }

    return item_text;
}

Element GitPanel::renderFooter() {
    auto& colors = theme_.getColors();

    Elements footer_elements;
    std::string help_text;
    switch (current_mode_) {
        case GitPanelMode::STATUS:
            help_text = "↑↓: navigate | Space: select | s: stage | u: unstage | a: select all | c: "
                        "commit | ESC: exit";
            break;
        case GitPanelMode::COMMIT:
            help_text = "Enter: commit | ESC: back to status";
            break;
        case GitPanelMode::BRANCH:
            help_text = "↑↓: navigate | Enter: switch | n: new branch | ESC: back";
            break;
        case GitPanelMode::REMOTE:
            help_text = "p: push | l: pull | f: fetch | ESC: back";
            break;
    }

    footer_elements.push_back(text(help_text) | color(colors.comment));

    // Add scroll indicator if needed
    if (files_.size() > 20) {
        size_t total_pages = (files_.size() + 19) / 20; // Ceiling division
        size_t current_page = scroll_offset_ / 20 + 1;
        std::string scroll_info =
            " [" + std::to_string(current_page) + "/" + std::to_string(total_pages) + "]";
        footer_elements.push_back(text(scroll_info) | color(colors.menubar_fg));
    }

    return hbox(std::move(footer_elements)) | border;
}

Element GitPanel::renderError() {
    if (error_message_.empty())
        return emptyElement();

    return text("Error: " + error_message_) | color(Color::Red) | border;
}

Element GitPanel::separatorLight() {
    return text(std::string(80, '-')) | color(theme_.getColors().comment);
}

// Component builders

Component GitPanel::buildMainComponent() {
    return Renderer([this] {
        if (!visible_)
            return emptyElement();

        auto& colors = theme_.getColors();

        Elements content_elements;
        content_elements.push_back(renderHeader());
        content_elements.push_back(separatorLight());
        content_elements.push_back(renderTabs());
        content_elements.push_back(separator());

        switch (current_mode_) {
            case GitPanelMode::STATUS:
                content_elements.push_back(renderStatusPanel());
                break;
            case GitPanelMode::COMMIT:
                content_elements.push_back(renderCommitPanel());
                break;
            case GitPanelMode::BRANCH:
                content_elements.push_back(renderBranchPanel());
                break;
            case GitPanelMode::REMOTE:
                content_elements.push_back(renderRemotePanel());
                break;
        }

        if (!error_message_.empty()) {
            content_elements.push_back(separatorLight());
            content_elements.push_back(renderError());
        }

        content_elements.push_back(separator());
        content_elements.push_back(renderFooter());

        Element dialog_content = vbox(std::move(content_elements));

        // Use window style like other dialogs with proper sizing
        return window(text("Git Panel"), dialog_content) | size(WIDTH, GREATER_THAN, 90) |
               size(HEIGHT, GREATER_THAN, 28) | bgcolor(colors.background) | border;
    });
}

// Key handlers

bool GitPanel::handleStatusModeKey(Event event) {
    const size_t visible_items = 20; // Number of visible items at once

    if (event == Event::ArrowUp) {
        if (selected_index_ > 0) {
            selected_index_--;

            // Auto-scroll up if needed
            if (selected_index_ < scroll_offset_) {
                scroll_offset_ = selected_index_;
            }
        }
        return true;
    }
    if (event == Event::ArrowDown) {
        if (selected_index_ < files_.size() - 1) {
            selected_index_++;

            // Auto-scroll down if needed - keep cursor visible with some buffer
            if (selected_index_ >= scroll_offset_ + visible_items - 2) {
                scroll_offset_ = selected_index_ - visible_items + 3;

                // Load more data if approaching the end
                if (selected_index_ >= files_.size() - 5) {
                    // This could trigger loading more data in the future
                    // For now, just ensure we don't go out of bounds
                }
            }
        }
        return true;
    }
    if (event == Event::Character(' ')) { // Space
        toggleFileSelection(selected_index_);
        return true;
    }
    if (event == Event::Character('a')) {
        selectAll();
        return true;
    }
    if (event == Event::Character('s')) {
        performStageSelected();
        return true;
    }
    if (event == Event::Character('u')) {
        performUnstageSelected();
        return true;
    }
    if (event == Event::Character('c')) {
        switchMode(GitPanelMode::COMMIT);
        return true;
    }
    if (event == Event::Character('b')) {
        switchMode(GitPanelMode::BRANCH);
        return true;
    }
    if (event == Event::Character('r')) {
        switchMode(GitPanelMode::REMOTE);
        return true;
    }

    return false;
}

bool GitPanel::handleCommitModeKey(Event event) {
    if (event == Event::Return) {
        performCommit();
        return true;
    }
    if (event == Event::Escape) {
        switchMode(GitPanelMode::STATUS);
        return true;
    }

    // Handle text input for commit message
    if (event.is_character()) {
        commit_message_ += event.character();
        return true;
    }
    if (event == Event::Backspace && !commit_message_.empty()) {
        commit_message_.pop_back();
        return true;
    }

    return false;
}

bool GitPanel::handleBranchModeKey(Event event) {
    const size_t visible_items = 15; // Number of visible branch items

    if (event == Event::ArrowUp) {
        if (selected_index_ > 0) {
            selected_index_--;

            // Auto-scroll up if needed
            if (selected_index_ < scroll_offset_) {
                scroll_offset_ = selected_index_;
            }
        }
        return true;
    }
    if (event == Event::ArrowDown) {
        if (selected_index_ < branches_.size() - 1) {
            selected_index_++;

            // Auto-scroll down if needed
            if (selected_index_ >= scroll_offset_ + visible_items - 2) {
                scroll_offset_ = selected_index_ - visible_items + 3;
            }
        }
        return true;
    }
    if (event == Event::Return) {
        performSwitchBranch();
        return true;
    }
    if (event == Event::Character('n')) {
        // Focus on branch name input (simplified - just switch to input mode)
        return true;
    }
    if (event == Event::Character('d')) {
        if (selected_index_ < branches_.size() && !branches_[selected_index_].is_current) {
            git_manager_->deleteBranch(branches_[selected_index_].name);
            refreshData();
        }
        return true;
    }
    if (event == Event::Escape) {
        switchMode(GitPanelMode::STATUS);
        return true;
    }

    // Handle text input for new branch name
    if (event.is_character()) {
        branch_name_ += event.character();
        return true;
    }
    if (event == Event::Backspace && !branch_name_.empty()) {
        branch_name_.pop_back();
        return true;
    }

    return false;
}

bool GitPanel::handleRemoteModeKey(Event event) {
    if (event == Event::Character('p')) {
        performPush();
        return true;
    }
    if (event == Event::Character('l')) {
        performPull();
        return true;
    }
    if (event == Event::Character('f')) {
        git_manager_->fetch();
        refreshData();
        return true;
    }
    if (event == Event::Escape) {
        switchMode(GitPanelMode::STATUS);
        return true;
    }

    return false;
}

// Utility methods

std::string GitPanel::getStatusIcon(GitFileStatus status) const {
    switch (status) {
        case GitFileStatus::MODIFIED:
            return MODIFIED;
        case GitFileStatus::ADDED:
            return SAVED;
        case GitFileStatus::DELETED:
            return CLOSE;
        case GitFileStatus::RENAMED:
            return ARROW_RIGHT;
        case GitFileStatus::COPIED:
            return COPY;
        case GitFileStatus::UNTRACKED:
            return UNSAVED;
        case GitFileStatus::IGNORED:
            return LOCK;
        default:
            return pnana::ui::icons::FILE;
    }
}

std::string GitPanel::getStatusText(GitFileStatus status) const {
    switch (status) {
        case GitFileStatus::MODIFIED:
            return "modified";
        case GitFileStatus::ADDED:
            return "added";
        case GitFileStatus::DELETED:
            return "deleted";
        case GitFileStatus::RENAMED:
            return "renamed";
        case GitFileStatus::COPIED:
            return "copied";
        case GitFileStatus::UNTRACKED:
            return "untracked";
        case GitFileStatus::IGNORED:
            return "ignored";
        default:
            return "unknown";
    }
}

std::string GitPanel::getModeTitle(GitPanelMode mode) const {
    switch (mode) {
        case GitPanelMode::STATUS:
            return "Status";
        case GitPanelMode::COMMIT:
            return "Commit";
        case GitPanelMode::BRANCH:
            return "Branch";
        case GitPanelMode::REMOTE:
            return "Remote";
        default:
            return "";
    }
}

bool GitPanel::hasStagedChanges() const {
    return std::any_of(files_.begin(), files_.end(), [](const GitFile& f) {
        return f.staged;
    });
}

bool GitPanel::hasUnstagedChanges() const {
    return std::any_of(files_.begin(), files_.end(), [](const GitFile& f) {
        return !f.staged;
    });
}

} // namespace vgit
} // namespace pnana
