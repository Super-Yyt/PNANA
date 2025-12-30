// LSP 集成相关实现
#include "core/editor.h"
#include "features/lsp/lsp_server_manager.h"
#include "utils/logger.h"
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <ftxui/component/event.hpp>
#include <cstring>

using namespace ftxui;

namespace fs = std::filesystem;

namespace pnana {
namespace core {

#ifdef BUILD_LSP_SUPPORT

void Editor::initializeLsp() {
    // 创建 LSP 服务器管理器
    lsp_manager_ = std::make_unique<features::LspServerManager>();
    
    // 设置诊断回调（应用到所有 LSP 客户端）
    lsp_manager_->setDiagnosticsCallback([this](
        const std::string& /* uri */,
        const std::vector<features::Diagnostic>& diagnostics) {
        // 可以在这里处理诊断信息，比如在状态栏显示错误数量
        if (!diagnostics.empty()) {
            int error_count = 0;
            int warning_count = 0;
            for (const auto& diag : diagnostics) {
                if (diag.severity == 1) error_count++;
                else if (diag.severity == 2) warning_count++;
            }
            if (error_count > 0 || warning_count > 0) {
                std::string msg = "LSP: ";
                if (error_count > 0) msg += std::to_string(error_count) + " error(s)";
                if (warning_count > 0) {
                    if (error_count > 0) msg += ", ";
                    msg += std::to_string(warning_count) + " warning(s)";
                }
                setStatusMessage(msg);
            }
        }
    });
    
    // LSP 管理器使用延迟初始化，只在需要时启动对应的服务器
        lsp_enabled_ = true;
    setStatusMessage("LSP manager initialized");
}

void Editor::shutdownLsp() {
    if (lsp_manager_ && lsp_enabled_) {
        lsp_manager_->shutdownAll();
        lsp_enabled_ = false;
    }
    completion_popup_.hide();
}

std::string Editor::detectLanguageId(const std::string& filepath) {
    // 根据文件扩展名返回语言 ID
    std::string ext = fs::path(filepath).extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == ".cpp" || ext == ".cxx" || ext == ".cc" || ext == ".hpp" || 
        ext == ".hxx" || ext == ".h" || ext == ".c") {
        return "cpp";
    } else if (ext == ".py") {
        return "python";
    } else if (ext == ".go") {
        return "go";
    } else if (ext == ".rs") {
        return "rust";
    } else if (ext == ".java") {
        return "java";
    } else if (ext == ".js" || ext == ".jsx") {
        return "javascript";
    } else if (ext == ".ts" || ext == ".tsx") {
        return "typescript";
    } else if (ext == ".html" || ext == ".htm") {
        return "html";
    } else if (ext == ".css") {
        return "css";
    } else if (ext == ".json") {
        return "json";
    } else if (ext == ".xml") {
        return "xml";
    } else if (ext == ".md") {
        return "markdown";
    } else if (ext == ".sh" || ext == ".bash") {
        return "shellscript";
    } else if (ext == ".yaml" || ext == ".yml") {
        return "yaml";
    } else if (ext == ".toml") {
        return "toml";
    }
    
    return "plaintext";
}

std::string Editor::filepathToUri(const std::string& filepath) {
    LOG("filepathToUri() called with: " + filepath);
    LOG("Filepath length: " + std::to_string(filepath.length()));
    
    // 转换为 file:// URI
    // 注意：对于包含非 ASCII 字符（如中文）的路径，需要正确进行 UTF-8 URL 编码
    std::string uri = "file://";
    
    try {
        // 使用 try-catch 保护，避免路径处理失败导致卡住
        std::string path;
        LOG("Calling fs::absolute()...");
        try {
            path = fs::absolute(filepath).string();
            LOG("fs::absolute() succeeded, path: " + path);
        } catch (const std::exception& e) {
            LOG_ERROR("fs::absolute() failed: " + std::string(e.what()));
            // 如果 absolute 失败（可能因为路径包含特殊字符），使用原始路径
            path = filepath;
            LOG("Using original path: " + path);
        } catch (...) {
            LOG_ERROR("fs::absolute() failed: Unknown exception");
            path = filepath;
            LOG("Using original path: " + path);
        }
    
    // 替换反斜杠为正斜杠（Windows）
    std::replace(path.begin(), path.end(), '\\', '/');
        LOG("Path after replacing backslashes: " + path);
        
        // URL 编码（正确处理 UTF-8 多字节字符）
        // 对于 ASCII 字符，直接使用；对于多字节 UTF-8 字符，需要按字节编码
        LOG("Starting URL encoding, path length: " + std::to_string(path.length()));
        size_t encoded_bytes = 0;
        for (size_t i = 0; i < path.length(); ) {
            unsigned char c = static_cast<unsigned char>(path[i]);
            
            // ASCII 字符（0x00-0x7F）且是安全字符
            if (c < 0x80 && (std::isalnum(c) || c == '/' || c == '-' || c == '_' || c == '.' || c == ':')) {
                uri += static_cast<char>(c);
                i++;
            } else {
                // 需要编码的字符（包括 UTF-8 多字节字符）
                // 对于 UTF-8，每个字节都需要编码
                if (c < 0x80) {
                    // 单字节非 ASCII 字符
                    char hex[4];
                    snprintf(hex, sizeof(hex), "%%%02X", c);
                    uri += hex;
                    i++;
                } else {
                    // UTF-8 多字节字符：按字节编码
                    // UTF-8 字符的第一个字节：110xxxxx (2字节), 1110xxxx (3字节), 11110xxx (4字节)
                    int bytes = 0;
                    if ((c & 0xE0) == 0xC0) bytes = 2;      // 2字节字符
                    else if ((c & 0xF0) == 0xE0) bytes = 3;  // 3字节字符
                    else if ((c & 0xF8) == 0xF0) bytes = 4; // 4字节字符
                    else bytes = 1;  // 无效的 UTF-8，按单字节处理
                    
                    // 编码所有字节
                    for (int j = 0; j < bytes && (i + j) < path.length(); j++) {
                        unsigned char byte = static_cast<unsigned char>(path[i + j]);
                        char hex[4];
                        snprintf(hex, sizeof(hex), "%%%02X", byte);
                        uri += hex;
                    }
                    i += bytes;
                }
            }
            encoded_bytes++;
            if (encoded_bytes % 100 == 0) {
                LOG("URL encoding progress: " + std::to_string(encoded_bytes) + " bytes processed");
            }
        }
        LOG("URL encoding completed, URI length: " + std::to_string(uri.length()));
        LOG("Final URI: " + uri);
    } catch (const std::exception& e) {
        LOG_ERROR("filepathToUri() exception in main try block: " + std::string(e.what()));
        // 如果路径处理失败，使用简单的编码方式
        std::string path = filepath;
        std::replace(path.begin(), path.end(), '\\', '/');
        LOG("Using fallback encoding for path: " + path);
        for (unsigned char c : path) {
        if (std::isalnum(c) || c == '/' || c == '-' || c == '_' || c == '.' || c == ':') {
                uri += static_cast<char>(c);
        } else {
            char hex[4];
                snprintf(hex, sizeof(hex), "%%%02X", c);
            uri += hex;
            }
        }
        LOG("Fallback URI: " + uri);
    } catch (...) {
        LOG_ERROR("filepathToUri() unknown exception");
    }
    
    LOG("filepathToUri() returning: " + uri);
    return uri;
}

void Editor::updateLspDocument() {
    LOG("updateLspDocument() called");
    
    if (!lsp_enabled_ || !lsp_manager_) {
        LOG("LSP not enabled or manager not available");
        return;
    }
    
    Document* doc = getCurrentDocument();
    if (!doc) {
        LOG("No current document");
        return;
    }
    
    std::string filepath = doc->getFilePath();
    if (filepath.empty()) {
        LOG("File path is empty");
        return;
    }
    
    LOG("File path: " + filepath);
    
    try {
        LOG("Converting filepath to URI...");
    std::string uri = filepathToUri(filepath);
        LOG("URI: " + uri);
        
        LOG("Detecting language ID...");
    std::string language_id = detectLanguageId(filepath);
        LOG("Language ID: " + language_id);
        
        // 获取或创建对应的 LSP 客户端
        LOG("Getting LSP client for file...");
        features::LspClient* client = lsp_manager_->getClientForFile(filepath);
        if (!client) {
            LOG("No LSP client available for this file type");
            return;
        }
        LOG("LSP client obtained");
        
        // 延迟初始化：如果客户端还未初始化，现在初始化
        // 注意：初始化可能较慢，但不应该阻塞，因为 LSP 通信是异步的
        std::string root_path = fs::current_path().string();
        LOG("Root path: " + root_path);
        LOG("Checking if client is connected...");
        if (!client->isConnected()) {
            LOG("Client not connected, initializing...");
            try {
                // 初始化 LSP 客户端（非阻塞，但可能需要一些时间）
                // 如果初始化失败，静默返回，不影响文件打开
                if (!client->initialize(root_path)) {
                    LOG_WARNING("LSP client initialization failed (server may not be installed)");
                    return;
                }
                LOG("LSP client initialized successfully");
            } catch (const std::exception& e) {
                LOG_WARNING("LSP client initialization exception: " + std::string(e.what()) + " (server may not be installed)");
                // 初始化失败，静默处理，不影响文件打开
                return;
            } catch (...) {
                LOG_WARNING("LSP client initialization unknown exception (server may not be installed)");
                // 其他异常，静默处理
                return;
            }
        } else {
            LOG("Client already connected");
        }
    
    // 获取文档内容
        LOG("Getting document content...");
    std::string content;
        size_t line_count = doc->lineCount();
        LOG("Document line count: " + std::to_string(line_count));
        
        // 限制读取的行数，避免大文件卡住（最多读取前1000行）
        size_t max_lines = std::min(line_count, static_cast<size_t>(1000));
        LOG("Reading first " + std::to_string(max_lines) + " lines for LSP");
        
        for (size_t i = 0; i < max_lines; ++i) {
        content += doc->getLine(i);
            if (i < max_lines - 1) {
            content += "\n";
        }
    }
        
        // 如果文件很大，添加提示
        if (line_count > max_lines) {
            LOG_WARNING("File has " + std::to_string(line_count) + " lines, only first " + 
                       std::to_string(max_lines) + " lines sent to LSP");
        }
        
        LOG("Document content length: " + std::to_string(content.length()));
    
    // 检查是否已经打开过
        LOG("Checking if file already opened in LSP...");
    if (file_language_map_.find(uri) == file_language_map_.end()) {
        // 首次打开，发送 didOpen
            LOG("Sending didOpen notification...");
            client->didOpen(uri, language_id, content);
        file_language_map_[uri] = language_id;
            LOG("didOpen notification sent");
    } else {
        // 已打开，发送 didChange
            LOG("Sending didChange notification...");
        static int version = 1;
        version++;
            client->didChange(uri, content, version);
            LOG("didChange notification sent, version: " + std::to_string(version));
        }
        LOG("updateLspDocument() completed successfully");
    } catch (const std::exception& e) {
        LOG_WARNING("updateLspDocument() exception: " + std::string(e.what()) + " (LSP feature disabled for this file)");
        // 任何异常都不应该影响文件打开，静默处理
        return;
    } catch (...) {
        LOG_WARNING("updateLspDocument() unknown exception (LSP feature disabled for this file)");
        // 其他异常，静默处理
        return;
    }
}

void Editor::triggerCompletion() {
    if (!lsp_enabled_ || !lsp_manager_) {
        return;
    }
    
    Document* doc = getCurrentDocument();
    if (!doc) {
        return;
    }
    
    std::string filepath = doc->getFilePath();
    if (filepath.empty()) {
        // 如果文件未保存，使用临时路径
        filepath = "/tmp/pnana_unsaved_" + std::to_string(reinterpret_cast<uintptr_t>(doc));
    }
    
    // 获取或创建对应的 LSP 客户端
    features::LspClient* client = lsp_manager_->getClientForFile(filepath);
    if (!client || !client->isConnected()) {
        completion_popup_.hide();
        return;
    }
    
    std::string uri = filepathToUri(filepath);
    
    // 确保文档已打开
    updateLspDocument();
    
    // 获取当前位置
    features::LspPosition pos(static_cast<int>(cursor_row_), static_cast<int>(cursor_col_));
    
    // 获取当前行的光标位置之前的文本，用于过滤和排序
    const std::string& line = doc->getLine(cursor_row_);
    std::string prefix = "";
    if (cursor_col_ > 0 && cursor_col_ <= line.length()) {
        // 从光标位置向前查找单词边界
        size_t start = cursor_col_;
        while (start > 0 && 
               (std::isalnum(line[start - 1]) || 
                line[start - 1] == '_' || 
                line[start - 1] == '.' ||
                line[start - 1] == ':' ||
                line[start - 1] == '-' ||
                line[start - 1] == '>')) {
            start--;
        }
        if (start < cursor_col_) {
            prefix = line.substr(start, cursor_col_ - start);
        }
    }
    
    // 请求补全
    auto items = client->completion(uri, pos);
    
    if (!items.empty()) {
        // 根据前缀过滤和排序（类似 VSCode/Neovim）
        std::vector<features::CompletionItem> filtered_items;
        std::vector<features::CompletionItem> exact_matches;
        std::vector<features::CompletionItem> prefix_matches;
        std::vector<features::CompletionItem> other_items;
        
        std::string lower_prefix = prefix;
        std::transform(lower_prefix.begin(), lower_prefix.end(), lower_prefix.begin(), ::tolower);
        
        for (const auto& item : items) {
            std::string label = item.label;
            std::string lower_label = label;
            std::transform(lower_label.begin(), lower_label.end(), lower_label.begin(), ::tolower);
            
            if (prefix.empty()) {
                // 没有前缀，显示所有项
                filtered_items.push_back(item);
            } else if (label == prefix) {
                // 精确匹配
                exact_matches.push_back(item);
            } else if (lower_label.find(lower_prefix) == 0) {
                // 前缀匹配（不区分大小写）
                prefix_matches.push_back(item);
            } else if (lower_label.find(lower_prefix) != std::string::npos) {
                // 包含匹配（不区分大小写）
                other_items.push_back(item);
            }
        }
        
        // 合并结果：精确匹配 > 前缀匹配 > 包含匹配
        filtered_items.clear();
        filtered_items.insert(filtered_items.end(), exact_matches.begin(), exact_matches.end());
        filtered_items.insert(filtered_items.end(), prefix_matches.begin(), prefix_matches.end());
        filtered_items.insert(filtered_items.end(), other_items.begin(), other_items.end());
        
        // 如果过滤后为空，显示所有项
        if (filtered_items.empty()) {
            filtered_items = items;
        }
        
        // 限制显示数量，提高性能
        if (filtered_items.size() > 50) {
            filtered_items.resize(50);
        }
        
        // 显示补全弹窗，传入屏幕尺寸用于位置计算
        completion_popup_.show(filtered_items, cursor_row_, cursor_col_,
                              screen_.dimx(), screen_.dimy());
    } else {
        completion_popup_.hide();
    }
}

void Editor::handleCompletionInput(ftxui::Event event) {
    if (!completion_popup_.isVisible()) {
        return;
    }
    
    if (event == Event::ArrowDown) {
        completion_popup_.selectNext();
    } else if (event == Event::ArrowUp) {
        completion_popup_.selectPrevious();
    } else if (event == Event::Return || event == Event::Tab) {
        applyCompletion();
    } else if (event == Event::Escape) {
        completion_popup_.hide();
    }
}

void Editor::applyCompletion() {
    if (!completion_popup_.isVisible()) {
        return;
    }
    
    std::string text = completion_popup_.applySelected();
    if (text.empty()) {
        return;
    }
    
    // 获取当前行的光标位置之前的文本
    Document* doc = getCurrentDocument();
    if (!doc) return;
    
    const std::string& line = doc->getLine(cursor_row_);
    if (cursor_col_ > line.length()) {
        cursor_col_ = line.length();
    }
    
    std::string before_cursor = line.substr(0, cursor_col_);
    
    // 找到要替换的前缀（从最后一个单词开始）
    size_t prefix_start = before_cursor.length();
    while (prefix_start > 0 && 
           (std::isalnum(before_cursor[prefix_start - 1]) || 
            before_cursor[prefix_start - 1] == '_' ||
            before_cursor[prefix_start - 1] == '.')) {
        prefix_start--;
    }
    
    std::string prefix = before_cursor.substr(prefix_start);
    
    // 删除前缀
    size_t prefix_len = prefix.length();
    for (size_t i = 0; i < prefix_len; ++i) {
        if (cursor_col_ > 0) {
            cursor_col_--;
            doc->deleteChar(cursor_row_, cursor_col_);
        }
    }
    
    // 插入补全文本
    doc->insertText(cursor_row_, cursor_col_, text);
    cursor_col_ += text.length();
    
    // 隐藏补全弹窗
    completion_popup_.hide();
    
    // 更新 LSP 文档
    updateLspDocument();
}

ftxui::Element Editor::renderCompletionPopup() {
    if (!completion_popup_.isVisible()) {
        return ftxui::text("");
    }
    
    // 更新光标位置（用于重新计算弹窗位置）
    completion_popup_.updateCursorPosition(
        cursor_row_, cursor_col_,
        screen_.dimx(), screen_.dimy()
    );
    
    // 渲染补全弹窗（固定尺寸以避免抖动）
    return completion_popup_.render(theme_);
}

#endif // BUILD_LSP_SUPPORT

} // namespace core
} // namespace pnana

