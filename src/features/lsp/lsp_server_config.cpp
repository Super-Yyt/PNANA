#include "features/lsp/lsp_server_config.h"
#include <algorithm>

namespace pnana {
namespace features {

LspServerConfigManager::LspServerConfigManager() {
    initializeDefaultConfigs();
}

std::vector<LspServerConfig> LspServerConfigManager::getDefaultConfigs() {
    std::vector<LspServerConfig> configs;
    
    // C/C++ - clangd
    configs.emplace_back(
        "clangd",
        "clangd",
        "cpp",
        std::set<std::string>{".cpp", ".cxx", ".cc", ".hpp", ".hxx", ".h", ".c", ".c++", ".h++"}
    );
    
    // Python - pylsp (Python Language Server Protocol)
    configs.emplace_back(
        "pylsp",
        "pylsp",
        "python",
        std::set<std::string>{".py", ".pyw", ".pyi"}
    );
    
    // Go - gopls
    configs.emplace_back(
        "gopls",
        "gopls",
        "go",
        std::set<std::string>{".go"}
    );
    
    // Rust - rust-analyzer
    configs.emplace_back(
        "rust-analyzer",
        "rust-analyzer",
        "rust",
        std::set<std::string>{".rs"}
    );
    
    // Java - jdtls (Eclipse JDT Language Server)
    configs.emplace_back(
        "jdtls",
        "jdtls",
        "java",
        std::set<std::string>{".java"}
    );
    
    // JavaScript/TypeScript - typescript-language-server
    configs.emplace_back(
        "typescript-language-server",
        "typescript-language-server",
        "typescript",
        std::set<std::string>{".ts", ".tsx", ".mts", ".cts"}
    );
    
    // JavaScript - typescript-language-server (也支持 JS)
    configs.emplace_back(
        "typescript-language-server-js",
        "typescript-language-server",
        "javascript",
        std::set<std::string>{".js", ".jsx", ".mjs", ".cjs"}
    );
    
    // HTML - html-language-server
    configs.emplace_back(
        "html-language-server",
        "html-languageserver",
        "html",
        std::set<std::string>{".html", ".htm"}
    );
    
    // CSS - css-language-server
    configs.emplace_back(
        "css-language-server",
        "css-languageserver",
        "css",
        std::set<std::string>{".css", ".scss", ".less", ".sass"}
    );
    
    // JSON - json-language-server
    configs.emplace_back(
        "json-language-server",
        "json-languageserver",
        "json",
        std::set<std::string>{".json", ".jsonc"}
    );
    
    // YAML - yaml-language-server
    configs.emplace_back(
        "yaml-language-server",
        "yaml-language-server",
        "yaml",
        std::set<std::string>{".yaml", ".yml"}
    );
    
    // Markdown - marksman 或 markdown-language-server
    configs.emplace_back(
        "marksman",
        "marksman",
        "markdown",
        std::set<std::string>{".md", ".markdown"}
    );
    
    // Shell - bash-language-server
    configs.emplace_back(
        "bash-language-server",
        "bash-language-server",
        "shellscript",
        std::set<std::string>{".sh", ".bash", ".zsh"}
    );
    
    return configs;
}

void LspServerConfigManager::initializeDefaultConfigs() {
    configs_ = getDefaultConfigs();
}

const LspServerConfig* LspServerConfigManager::findConfigByExtension(
    const std::string& ext) const {
    // 转换为小写
    std::string lower_ext = ext;
    std::transform(lower_ext.begin(), lower_ext.end(), lower_ext.begin(), ::tolower);
    
    for (const auto& config : configs_) {
        if (config.matchesExtension(lower_ext)) {
            return &config;
        }
    }
    
    return nullptr;
}

const LspServerConfig* LspServerConfigManager::findConfigByLanguageId(
    const std::string& language_id) const {
    for (const auto& config : configs_) {
        if (config.language_id == language_id) {
            return &config;
        }
    }
    
    return nullptr;
}

void LspServerConfigManager::addConfig(const LspServerConfig& config) {
    configs_.push_back(config);
}

} // namespace features
} // namespace pnana

