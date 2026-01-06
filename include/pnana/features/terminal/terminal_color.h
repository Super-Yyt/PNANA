#ifndef PNANA_FEATURES_TERMINAL_COLOR_H
#define PNANA_FEATURES_TERMINAL_COLOR_H

#include <ftxui/dom/elements.hpp>
#include <string>
#include <vector>

namespace pnana {
namespace features {
namespace terminal {

// ANSI颜色解析器
class AnsiColorParser {
  public:
    // 解析ANSI颜色码并返回格式化的FTXUI元素
    static ftxui::Element parse(const std::string& text);

    // 检查文本是否包含ANSI颜色码
    static bool hasAnsiCodes(const std::string& text);

    // 移除ANSI颜色码，返回纯文本
    static std::string stripAnsiCodes(const std::string& text);

  private:
    // ANSI转义序列状态
    enum class ParseState { Normal, Escape, CSI, OSC };

    // 颜色映射
    static ftxui::Color ansiColorToFtxui(int ansi_code);
    static ftxui::Color ansi256ColorToFtxui(int color_code);
    static ftxui::Color rgbColorToFtxui(int r, int g, int b);

    // 解析CSI序列参数
    static std::vector<int> parseCsiParams(const std::string& params);
};

} // namespace terminal
} // namespace features
} // namespace pnana

#endif // PNANA_FEATURES_TERMINAL_COLOR_H
