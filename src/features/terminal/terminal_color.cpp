#include "features/terminal/terminal_color.h"
#include <algorithm>
#include <sstream>

namespace pnana {
namespace features {
namespace terminal {

ftxui::Element AnsiColorParser::parse(const std::string& text) {
    if (!hasAnsiCodes(text)) {
        return ftxui::text(text);
    }

    std::vector<ftxui::Element> elements;
    std::string current_text;
    ParseState state = ParseState::Normal;
    std::string escape_sequence;

    // 默认样式
    ftxui::Color fg_color = ftxui::Color::Default;
    ftxui::Color bg_color = ftxui::Color::Default;
    bool bold = false;
    bool italic = false;
    bool underline = false;
    bool blink = false;
    bool reverse = false;
    bool strikethrough = false;

    auto add_current_element = [&]() {
        if (!current_text.empty()) {
            ftxui::Element elem = ftxui::text(current_text);

            // 应用样式
            if (bold)
                elem = elem | ftxui::bold;
            // Note: italic not supported in current FTXUI version
            // if (italic) elem = elem | ftxui::italic;
            if (underline)
                elem = elem | ftxui::underlined;
            if (blink)
                elem = elem | ftxui::blink;
            if (reverse)
                elem = elem | ftxui::inverted;
            if (strikethrough)
                elem = elem | ftxui::strikethrough;

            // 应用颜色
            if (fg_color != ftxui::Color::Default) {
                elem = elem | ftxui::color(fg_color);
            }
            if (bg_color != ftxui::Color::Default) {
                elem = elem | ftxui::bgcolor(bg_color);
            }

            elements.push_back(elem);
            current_text.clear();
        }
    };

    for (size_t i = 0; i < text.length(); ++i) {
        char c = text[i];

        switch (state) {
            case ParseState::Normal:
                if (c == '\x1b') {         // ESC
                    add_current_element(); // 添加之前的文本
                    state = ParseState::Escape;
                    escape_sequence = "\x1b";
                } else {
                    current_text += c;
                }
                break;

            case ParseState::Escape:
                escape_sequence += c;
                if (c == '[') {
                    state = ParseState::CSI;
                } else if (c == ']') {
                    state = ParseState::OSC;
                } else {
                    // 不是CSI或OSC序列，回到正常状态
                    current_text += escape_sequence;
                    state = ParseState::Normal;
                }
                break;

            case ParseState::CSI:
                escape_sequence += c;
                if (c >= '@' && c <= '~') { // CSI结束符
                    // 解析CSI序列
                    std::string params_str =
                        escape_sequence.substr(2, escape_sequence.length() - 3);
                    auto params = parseCsiParams(params_str);

                    if (!params.empty()) {
                        int command = params.back();
                        params.pop_back();

                        switch (command) {
                            case 0: // 重置所有样式
                                fg_color = ftxui::Color::Default;
                                bg_color = ftxui::Color::Default;
                                bold = italic = underline = blink = reverse = strikethrough = false;
                                break;
                            case 1: // 粗体
                                bold = true;
                                break;
                            case 3: // 斜体
                                italic = true;
                                break;
                            case 4: // 下划线
                                underline = true;
                                break;
                            case 5: // 闪烁
                            case 6: // 快速闪烁
                                blink = true;
                                break;
                            case 7: // 反转
                                reverse = true;
                                break;
                            case 9: // 删除线
                                strikethrough = true;
                                break;
                            case 21: // 关闭粗体
                                bold = false;
                                break;
                            case 22: // 关闭粗体/亮色
                                bold = false;
                                break;
                            case 23: // 关闭斜体
                                italic = false;
                                break;
                            case 24: // 关闭下划线
                                underline = false;
                                break;
                            case 25: // 关闭闪烁
                                blink = false;
                                break;
                            case 27: // 关闭反转
                                reverse = false;
                                break;
                            case 29: // 关闭删除线
                                strikethrough = false;
                                break;
                            case 39: // 默认前景色
                                fg_color = ftxui::Color::Default;
                                break;
                            case 49: // 默认背景色
                                bg_color = ftxui::Color::Default;
                                break;
                        }

                        // 前景色 (30-37: 标准色, 38: 256色或RGB, 90-97: 亮色)
                        if (command >= 30 && command <= 37) {
                            fg_color = ansiColorToFtxui(command - 30);
                        } else if (command >= 90 && command <= 97) {
                            fg_color = ansiColorToFtxui(command - 82); // 90-97 -> 8-15 (亮色)
                        } else if (command == 38 && params.size() >= 2) {
                            if (params[0] == 5 && params.size() >= 1) { // 256色
                                fg_color = ansi256ColorToFtxui(params[1]);
                            } else if (params[0] == 2 && params.size() >= 4) { // RGB
                                fg_color = rgbColorToFtxui(params[1], params[2], params[3]);
                            }
                        }

                        // 背景色 (40-47: 标准色, 48: 256色或RGB, 100-107: 亮色)
                        if (command >= 40 && command <= 47) {
                            bg_color = ansiColorToFtxui(command - 40);
                        } else if (command >= 100 && command <= 107) {
                            bg_color = ansiColorToFtxui(command - 92); // 100-107 -> 8-15 (亮色)
                        } else if (command == 48 && params.size() >= 2) {
                            if (params[0] == 5 && params.size() >= 1) { // 256色
                                bg_color = ansi256ColorToFtxui(params[1]);
                            } else if (params[0] == 2 && params.size() >= 4) { // RGB
                                bg_color = rgbColorToFtxui(params[1], params[2], params[3]);
                            }
                        }
                    }

                    state = ParseState::Normal;
                    escape_sequence.clear();
                }
                break;

            case ParseState::OSC:
                escape_sequence += c;
                if (c == '\x07' || (c == '\\' && i > 0 && text[i - 1] == '\x1b')) { // OSC结束符
                    // OSC序列通常用于窗口标题等，我们忽略它们
                    state = ParseState::Normal;
                    escape_sequence.clear();
                }
                break;
        }
    }

    // 添加最后的文本
    add_current_element();

    if (elements.size() == 1) {
        return elements[0];
    } else {
        return ftxui::hbox(std::move(elements));
    }
}

bool AnsiColorParser::hasAnsiCodes(const std::string& text) {
    return text.find('\x1b') != std::string::npos;
}

std::string AnsiColorParser::stripAnsiCodes(const std::string& text) {
    std::string result;
    ParseState state = ParseState::Normal;

    for (char c : text) {
        switch (state) {
            case ParseState::Normal:
                if (c == '\x1b') {
                    state = ParseState::Escape;
                } else {
                    result += c;
                }
                break;

            case ParseState::Escape:
                if (c == '[') {
                    state = ParseState::CSI;
                } else if (c == ']') {
                    state = ParseState::OSC;
                } else {
                    result += '\x1b';
                    result += c;
                    state = ParseState::Normal;
                }
                break;

            case ParseState::CSI:
                if (c >= '@' && c <= '~') {
                    state = ParseState::Normal;
                }
                break;

            case ParseState::OSC:
                if (c == '\x07' || c == '\\') {
                    state = ParseState::Normal;
                }
                break;
        }
    }

    return result;
}

ftxui::Color AnsiColorParser::ansiColorToFtxui(int ansi_code) {
    static const std::vector<ftxui::Color> ansi_colors = {
        ftxui::Color::Black,        // 0
        ftxui::Color::Red,          // 1
        ftxui::Color::Green,        // 2
        ftxui::Color::Yellow,       // 3
        ftxui::Color::Blue,         // 4
        ftxui::Color::Magenta,      // 5
        ftxui::Color::Cyan,         // 6
        ftxui::Color::White,        // 7
        ftxui::Color::GrayDark,     // 8 (亮黑)
        ftxui::Color::RedLight,     // 9 (亮红)
        ftxui::Color::GreenLight,   // 10 (亮绿)
        ftxui::Color::YellowLight,  // 11 (亮黄)
        ftxui::Color::BlueLight,    // 12 (亮蓝)
        ftxui::Color::MagentaLight, // 13 (亮品红)
        ftxui::Color::CyanLight,    // 14 (亮青)
        ftxui::Color::White         // 15 (亮白)
    };

    if (ansi_code >= 0 && ansi_code < static_cast<int>(ansi_colors.size())) {
        return ansi_colors[ansi_code];
    }
    return ftxui::Color::Default;
}

ftxui::Color AnsiColorParser::ansi256ColorToFtxui(int color_code) {
    // 简化的256色到RGB的映射
    if (color_code < 16) {
        return ansiColorToFtxui(color_code);
    } else if (color_code < 232) {
        // 6x6x6 颜色立方体 (16-231)
        int cube_code = color_code - 16;
        int r = (cube_code / 36) * 51;
        int g = ((cube_code / 6) % 6) * 51;
        int b = (cube_code % 6) * 51;
        return rgbColorToFtxui(r, g, b);
    } else {
        // 灰度 (232-255)
        int gray = (color_code - 232) * 10 + 8;
        return rgbColorToFtxui(gray, gray, gray);
    }
}

ftxui::Color AnsiColorParser::rgbColorToFtxui(int r, int g, int b) {
    return ftxui::Color::RGB(r, g, b);
}

std::vector<int> AnsiColorParser::parseCsiParams(const std::string& params_str) {
    std::vector<int> params;
    std::istringstream iss(params_str);
    std::string param;

    while (std::getline(iss, param, ';')) {
        if (!param.empty()) {
            try {
                params.push_back(std::stoi(param));
            } catch (...) {
                // 忽略无效参数
            }
        } else {
            params.push_back(0); // 空参数默认为0
        }
    }

    return params;
}

} // namespace terminal
} // namespace features
} // namespace pnana
