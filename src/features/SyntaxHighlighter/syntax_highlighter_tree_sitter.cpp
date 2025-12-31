#include "features/SyntaxHighlighter/syntax_highlighter_tree_sitter.h"
#include <tree_sitter/api.h>
#include <algorithm>
#include <cstring>

// Tree-sitter 语言定义（需要链接对应的语言库）
// 注意：这些函数从对应的语言库中获取
// 如果语言库未链接，需要在 CMakeLists.txt 中链接对应的库
extern "C" {
    // C/C++
    #ifdef BUILD_TREE_SITTER_CPP
    TSLanguage* tree_sitter_cpp();
    #endif
    #ifdef BUILD_TREE_SITTER_C
    TSLanguage* tree_sitter_c();
    #endif
    
    // Python
    #ifdef BUILD_TREE_SITTER_PYTHON
    TSLanguage* tree_sitter_python();
    #endif
    
    // JavaScript/TypeScript
    #ifdef BUILD_TREE_SITTER_JAVASCRIPT
    TSLanguage* tree_sitter_javascript();
    #endif
    #ifdef BUILD_TREE_SITTER_TYPESCRIPT
    TSLanguage* tree_sitter_typescript();
    #endif
    
    // 数据格式
    #ifdef BUILD_TREE_SITTER_JSON
    TSLanguage* tree_sitter_json();
    #endif
    #ifdef BUILD_TREE_SITTER_MARKDOWN
    TSLanguage* tree_sitter_markdown();
    #endif
    
    // Shell
    #ifdef BUILD_TREE_SITTER_BASH
    TSLanguage* tree_sitter_bash();
    #endif
    
    // 其他语言
    #ifdef BUILD_TREE_SITTER_RUST
    TSLanguage* tree_sitter_rust();
    #endif
    #ifdef BUILD_TREE_SITTER_GO
    TSLanguage* tree_sitter_go();
    #endif
    #ifdef BUILD_TREE_SITTER_JAVA
    TSLanguage* tree_sitter_java();
    #endif
}

using namespace ftxui;

namespace pnana {
namespace features {

SyntaxHighlighterTreeSitter::SyntaxHighlighterTreeSitter(ui::Theme& theme)
    : theme_(theme), parser_(nullptr), current_language_(nullptr), current_file_type_("text") {
    parser_ = ts_parser_new();
    if (!parser_) {
        // 如果创建失败，parser_ 保持 nullptr
        return;
    }
    initializeLanguages();
}

SyntaxHighlighterTreeSitter::~SyntaxHighlighterTreeSitter() {
    if (parser_) {
        ts_parser_delete(parser_);
    }
}

void SyntaxHighlighterTreeSitter::initializeLanguages() {
    // 初始化语言映射
    // 注意：这些语言库需要在编译时链接（在 CMakeLists.txt 中）
    // 如果语言库未链接，对应的语言将使用原生语法高亮器（自动回退）
    
    // C/C++
    #ifdef BUILD_TREE_SITTER_CPP
    TSLanguage* cpp_lang = tree_sitter_cpp();
    if (cpp_lang) {
        language_map_["cpp"] = cpp_lang;
        language_map_["cxx"] = cpp_lang;
        language_map_["cc"] = cpp_lang;
        language_map_["c++"] = cpp_lang;
        language_map_["hpp"] = cpp_lang;
        language_map_["hxx"] = cpp_lang;
        language_map_["hh"] = cpp_lang;
    }
    #endif
    
    #ifdef BUILD_TREE_SITTER_C
    TSLanguage* c_lang = tree_sitter_c();
    if (c_lang) {
        language_map_["c"] = c_lang;
        language_map_["h"] = c_lang;
    }
    #endif
    
    // Python
    #ifdef BUILD_TREE_SITTER_PYTHON
    TSLanguage* python_lang = tree_sitter_python();
    if (python_lang) {
        language_map_["py"] = python_lang;
        language_map_["python"] = python_lang;
        language_map_["pyw"] = python_lang;
        language_map_["pyi"] = python_lang;
    }
    #endif
    
    // JavaScript
    #ifdef BUILD_TREE_SITTER_JAVASCRIPT
    TSLanguage* js_lang = tree_sitter_javascript();
    if (js_lang) {
        language_map_["js"] = js_lang;
        language_map_["javascript"] = js_lang;
        language_map_["jsx"] = js_lang;
        language_map_["mjs"] = js_lang;
    }
    #endif
    
    // TypeScript
    #ifdef BUILD_TREE_SITTER_TYPESCRIPT
    TSLanguage* ts_lang = tree_sitter_typescript();
    if (ts_lang) {
        language_map_["ts"] = ts_lang;
        language_map_["typescript"] = ts_lang;
        language_map_["tsx"] = ts_lang;
    }
    #endif
    
    // JSON
    #ifdef BUILD_TREE_SITTER_JSON
    TSLanguage* json_lang = tree_sitter_json();
    if (json_lang) {
        language_map_["json"] = json_lang;
        language_map_["jsonc"] = json_lang;
    }
    #endif
    
    // Markdown
    #ifdef BUILD_TREE_SITTER_MARKDOWN
    TSLanguage* md_lang = tree_sitter_markdown();
    if (md_lang) {
        language_map_["md"] = md_lang;
        language_map_["markdown"] = md_lang;
    }
    #endif
    
    // Shell/Bash
    #ifdef BUILD_TREE_SITTER_BASH
    TSLanguage* bash_lang = tree_sitter_bash();
    if (bash_lang) {
        language_map_["sh"] = bash_lang;
        language_map_["bash"] = bash_lang;
        language_map_["shell"] = bash_lang;
        language_map_["zsh"] = bash_lang;
    }
    #endif
    
    // Rust
    #ifdef BUILD_TREE_SITTER_RUST
    TSLanguage* rust_lang = tree_sitter_rust();
    if (rust_lang) {
        language_map_["rs"] = rust_lang;
        language_map_["rust"] = rust_lang;
    }
    #endif
    
    // Go
    #ifdef BUILD_TREE_SITTER_GO
    TSLanguage* go_lang = tree_sitter_go();
    if (go_lang) {
        language_map_["go"] = go_lang;
    }
    #endif
    
    // Java
    #ifdef BUILD_TREE_SITTER_JAVA
    TSLanguage* java_lang = tree_sitter_java();
    if (java_lang) {
        language_map_["java"] = java_lang;
    }
    #endif
}

TSLanguage* SyntaxHighlighterTreeSitter::getLanguageForFileType(const std::string& file_type) {
    auto it = language_map_.find(file_type);
    if (it != language_map_.end()) {
        return it->second;
    }
    return nullptr;
}

void SyntaxHighlighterTreeSitter::setFileType(const std::string& file_type) {
    if (current_file_type_ == file_type) {
        return;
    }
    
    current_file_type_ = file_type;
    TSLanguage* lang = getLanguageForFileType(file_type);
    
    if (lang && parser_) {
        ts_parser_set_language(parser_, lang);
        current_language_ = lang;
    } else {
        current_language_ = nullptr;
    }
}

bool SyntaxHighlighterTreeSitter::supportsFileType(const std::string& file_type) const {
    return language_map_.find(file_type) != language_map_.end();
}

void SyntaxHighlighterTreeSitter::reset() {
    if (parser_ && current_language_) {
        ts_parser_set_language(parser_, current_language_);
    }
}

ftxui::Element SyntaxHighlighterTreeSitter::highlightLine(const std::string& line) {
    if (line.empty() || !parser_ || !current_language_) {
        return text(line) | color(theme_.getColors().foreground);
    }
    
    return parseAndHighlight(line);
}

ftxui::Element SyntaxHighlighterTreeSitter::highlightLines(const std::vector<std::string>& lines) {
    if (lines.empty() || !parser_ || !current_language_) {
        Elements elements;
        for (const auto& line : lines) {
            elements.push_back(text(line) | color(theme_.getColors().foreground));
        }
        return vbox(elements);
    }
    
    // 合并所有行为一个字符串进行解析（更高效）
    std::string code;
    for (const auto& line : lines) {
        code += line + "\n";
    }
    
    return parseAndHighlight(code);
}

ftxui::Element SyntaxHighlighterTreeSitter::parseAndHighlight(const std::string& code) {
    if (code.empty() || !parser_ || !current_language_) {
        return text(code) | color(theme_.getColors().foreground);
    }
    
    // 解析代码
    TSTree* tree = ts_parser_parse_string(parser_, nullptr, code.c_str(), code.length());
    if (!tree) {
        return text(code) | color(theme_.getColors().foreground);
    }
    
    TSNode root_node = ts_tree_root_node(tree);
    
    // 遍历语法树并生成高亮元素
    Elements elements;
    size_t current_pos = 0;
    traverseTree(root_node, code, elements, current_pos);
    
    // 处理未覆盖的文本
    if (current_pos < code.length()) {
        std::string remaining = code.substr(current_pos);
        elements.push_back(text(remaining) | color(theme_.getColors().foreground));
    }
    
    ts_tree_delete(tree);
    
    return hbox(elements);
}

void SyntaxHighlighterTreeSitter::traverseTree(TSNode node, const std::string& source,
                                               std::vector<ftxui::Element>& elements,
                                               size_t& current_pos) const {
    uint32_t start_byte = ts_node_start_byte(node);
    uint32_t end_byte = ts_node_end_byte(node);
    
    // 处理节点前的文本
    if (current_pos < start_byte) {
        std::string before = source.substr(current_pos, start_byte - current_pos);
        elements.push_back(text(before) | color(theme_.getColors().foreground));
        current_pos = start_byte;
    }
    
    // 获取节点类型
    const char* node_type_cstr = ts_node_type(node);
    std::string node_type = node_type_cstr ? node_type_cstr : "";
    
    // 获取节点文本
    std::string node_text = getNodeText(node, source);
    
    // 获取颜色
    Color node_color = getColorForNodeType(node_type);
    
    // 如果是叶子节点，直接添加
    if (ts_node_child_count(node) == 0) {
        elements.push_back(text(node_text) | color(node_color));
        current_pos = end_byte;
    } else {
        // 递归处理子节点
        uint32_t child_count = ts_node_child_count(node);
        for (uint32_t i = 0; i < child_count; ++i) {
            TSNode child = ts_node_child(node, i);
            traverseTree(child, source, elements, current_pos);
        }
    }
}

std::string SyntaxHighlighterTreeSitter::getNodeText(TSNode node, const std::string& source) const {
    uint32_t start_byte = ts_node_start_byte(node);
    uint32_t end_byte = ts_node_end_byte(node);
    
    if (start_byte >= source.length()) {
        return "";
    }
    
    size_t start = static_cast<size_t>(start_byte);
    size_t end = std::min(static_cast<size_t>(end_byte), source.length());
    
    return source.substr(start, end - start);
}

ftxui::Color SyntaxHighlighterTreeSitter::getColorForNodeType(const std::string& node_type) const {
    auto& colors = theme_.getColors();
    
    // 关键字
    if (node_type.find("keyword") != std::string::npos ||
        node_type == "if" || node_type == "else" || node_type == "for" ||
        node_type == "while" || node_type == "return" || node_type == "class" ||
        node_type == "function" || node_type == "const" || node_type == "let" ||
        node_type == "var" || node_type == "import" || node_type == "export") {
        return colors.keyword;
    }
    
    // 字符串
    if (node_type.find("string") != std::string::npos ||
        node_type == "string_content" || node_type == "string_literal") {
        return colors.string;
    }
    
    // 注释
    if (node_type.find("comment") != std::string::npos) {
        return colors.comment;
    }
    
    // 数字
    if (node_type.find("number") != std::string::npos ||
        node_type == "integer" || node_type == "float") {
        return colors.number;
    }
    
    // 函数
    if (node_type.find("function") != std::string::npos ||
        node_type == "call_expression" || node_type == "method_invocation") {
        return colors.function;
    }
    
    // 类型
    if (node_type.find("type") != std::string::npos ||
        node_type == "type_identifier" || node_type == "class_declaration") {
        return colors.type;
    }
    
    // 操作符
    if (node_type.find("operator") != std::string::npos ||
        node_type == "+" || node_type == "-" || node_type == "*" ||
        node_type == "/" || node_type == "=" || node_type == "==") {
        return colors.operator_color;
    }
    
    // 预处理器（使用 keyword 颜色）
    if (node_type.find("preproc") != std::string::npos ||
        node_type == "preprocessor_directive") {
        return colors.keyword;
    }
    
    // 默认颜色
    return colors.foreground;
}

} // namespace features
} // namespace pnana

