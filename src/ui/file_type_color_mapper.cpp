#include "ui/file_type_color_mapper.h"
#include <algorithm>

namespace pnana {
namespace ui {

FileTypeColorMapper::FileTypeColorMapper(const Theme& theme) : theme_(theme) {}

ftxui::Color FileTypeColorMapper::getFileColor(const std::string& filename,
                                               bool is_directory) const {
    auto& colors = theme_.getColors();

    if (is_directory) {
        if (filename == "..") {
            return colors.comment; // 上级目录使用灰色
        }
        return colors.function; // 目录使用蓝色
    }

    std::string ext = getFileExtension(filename);
    std::string name_lower = filename;
    std::transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower);

    // C/C++ 系列 - 蓝色
    if (ext == "c" || ext == "cpp" || ext == "cc" || ext == "cxx" || ext == "c++" || ext == "h" ||
        ext == "hpp" || ext == "hxx" || ext == "hh" || ext == "m" || ext == "mm") { // Objective-C
        return colors.keyword;
    }

    // Python 系列 - 黄色
    if (ext == "py" || ext == "pyw" || ext == "pyi" || ext == "pyc" || ext == "pyx" ||
        ext == "pxd") { // Cython
        return colors.warning;
    }

    // JavaScript/TypeScript - 橙色
    if (ext == "js" || ext == "jsx" || ext == "mjs" || ext == "cjs" || ext == "ts" ||
        ext == "tsx" || ext == "d.ts") {
        return colors.number;
    }

    // JVM 语言 - 绿色
    if (ext == "java" || ext == "class" || ext == "jar" || ext == "war" || ext == "kt" ||
        ext == "kts" || ext == "scala" || ext == "clj" || ext == "cljs" || ext == "groovy") {
        return colors.string;
    }

    // 系统编程语言 - 深绿色
    if (ext == "go" || ext == "rs" || ext == "zig" || ext == "nim" || ext == "d" || ext == "v" ||
        ext == "pony") {
        return colors.success;
    }

    // 函数式编程 - 青色
    if (ext == "hs" || ext == "lhs" || ext == "ml" || ext == "fs" || ext == "fsx" || ext == "elm" ||
        ext == "purs" || ext == "idr" || ext == "agda" || ext == "ex" || ext == "exs" ||
        ext == "dart") {
        return colors.info;
    }

    // Web 技术 - 浅蓝色
    if (ext == "html" || ext == "htm" || ext == "xhtml" || ext == "vue" || ext == "svelte" ||
        ext == "astro") {
        return ftxui::Color::Cyan;
    }

    // 样式表 - 粉色
    if (ext == "css" || ext == "scss" || ext == "sass" || ext == "less" || ext == "styl" ||
        ext == "pcss") {
        return colors.type;
    }

    // 数据格式 - 紫色
    if (ext == "json" || ext == "xml" || ext == "yml" || ext == "yaml" || ext == "toml" ||
        ext == "ini" || ext == "cfg" || ext == "graphql") {
        return ftxui::Color::Magenta;
    }

    // 脚本语言 - 绿色
    if (ext == "sh" || ext == "bash" || ext == "zsh" || ext == "fish" || ext == "ps1" ||
        ext == "bat" || ext == "cmd" || ext == "rb" || ext == "rbw" || ext == "lua" ||
        ext == "php" || ext == "pl" || ext == "pm" || ext == "tcl") {
        return ftxui::Color::Green;
    }

    // .NET 语言 - 深蓝色
    if (ext == "cs" || ext == "fs" || ext == "fsx" || ext == "vb" || ext == "xaml" ||
        ext == "csproj" || ext == "fsproj") {
        return colors.operator_color;
    }

    // Swift/Kotlin - 青绿色
    if (ext == "swift" || ext == "m" || ext == "mm") {
        return ftxui::Color::Cyan;
    }

    // 文档和标记语言 - 白色
    if (ext == "md" || ext == "markdown" || ext == "txt" || ext == "rst" || ext == "adoc" ||
        ext == "tex" || ext == "latex" || ext == "org") {
        return colors.foreground;
    }

    // 数据库 - 深蓝色
    if (ext == "sql" || ext == "db" || ext == "sqlite" || ext == "sqlite3" || ext == "mdb" ||
        ext == "accdb") {
        return colors.operator_color;
    }

    // 配置文件 - 灰色
    if (ext == "conf" || ext == "config" || ext == "properties" || ext == "env" ||
        ext == "dotenv" || ext == ".env") {
        return colors.comment;
    }

    // 日志文件 - 浅灰色
    if (ext == "log" || ext == "out" || ext == "err") {
        return ftxui::Color::GrayLight;
    }

    // 图像文件 - 紫色
    if (ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "gif" || ext == "bmp" ||
        ext == "svg" || ext == "ico" || ext == "webp" || ext == "tiff" || ext == "tif" ||
        ext == "raw" || ext == "psd" || ext == "ai" || ext == "eps") {
        return ftxui::Color::Magenta;
    }

    // 音频文件 - 橙红色
    if (ext == "mp3" || ext == "wav" || ext == "flac" || ext == "aac" || ext == "ogg" ||
        ext == "m4a" || ext == "wma") {
        return ftxui::Color::Red;
    }

    // 视频文件 - 深红色
    if (ext == "mp4" || ext == "avi" || ext == "mkv" || ext == "mov" || ext == "wmv" ||
        ext == "flv" || ext == "webm" || ext == "m4v") {
        return ftxui::Color::Red3;
    }

    // 压缩文件 - 红色
    if (ext == "zip" || ext == "tar" || ext == "gz" || ext == "bz2" || ext == "xz" || ext == "7z" ||
        ext == "rar" || ext == "tgz" || ext == "tbz2" || ext == "txz") {
        return colors.error;
    }

    // PDF 和文档 - 棕色
    if (ext == "pdf" || ext == "doc" || ext == "docx" || ext == "ppt" || ext == "pptx" ||
        ext == "xls" || ext == "xlsx" || ext == "odt" || ext == "ods" || ext == "odp") {
        return ftxui::Color::Yellow3;
    }

    // 二进制和可执行文件 - 深灰色
    if (ext == "exe" || ext == "dll" || ext == "so" || ext == "dylib" || ext == "bin" ||
        ext == "o" || ext == "a") {
        return ftxui::Color::GrayDark;
    }

    // 字体文件 - 深紫色
    if (ext == "ttf" || ext == "otf" || ext == "woff" || ext == "woff2") {
        return ftxui::Color::Magenta3;
    }

    // 特殊文件 - 根据文件名判断
    if (name_lower == "makefile" || name_lower == "makefile.am" || name_lower == "makefile.in" ||
        name_lower == "gnumakefile" || name_lower == "cmakefile") {
        return colors.operator_color;
    }
    if (name_lower == "readme" || name_lower == "readme.md" || name_lower == "readme.txt" ||
        name_lower == "readme.rst" || name_lower == "read.me") {
        return colors.foreground;
    }
    if (name_lower == "license" || name_lower == "license.md" || name_lower == "license.txt" ||
        name_lower == "licence" || name_lower == "licence.md" || name_lower == "licence.txt" ||
        name_lower == "copying") {
        return colors.warning;
    }
    if (name_lower == "changelog" || name_lower == "changelog.md" ||
        name_lower == "changelog.txt" || name_lower == "history" || name_lower == "history.md") {
        return colors.info;
    }
    if (name_lower == "authors" || name_lower == "contributors" || name_lower == "maintainers") {
        return colors.string;
    }
    if (name_lower == "dockerfile" || name_lower == "docker-compose.yml" ||
        name_lower == "docker-compose.yaml") {
        return ftxui::Color::Cyan;
    }
    if (name_lower == ".gitignore" || name_lower == ".gitattributes" ||
        name_lower == ".gitmodules" || name_lower == ".gitkeep") {
        return colors.comment;
    }
    if (name_lower == "package.json" || name_lower == "cargo.toml" || name_lower == "go.mod" ||
        name_lower == "requirements.txt") {
        return colors.keyword;
    }

    // 默认颜色 - 灰色
    return colors.comment;
}

std::string FileTypeColorMapper::getFileExtension(const std::string& filename) {
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos == std::string::npos || dot_pos == 0) {
        return "";
    }

    std::string ext = filename.substr(dot_pos + 1);
    // 转换为小写
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

} // namespace ui
} // namespace pnana
