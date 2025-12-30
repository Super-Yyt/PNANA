#include "ui/completion_popup.h"
#include "ui/icons.h"
#include <ftxui/dom/elements.hpp>
#include <algorithm>
#include <sstream>
#include <cmath>

using namespace ftxui;

namespace pnana {
namespace ui {

CompletionPopup::CompletionPopup()
    : visible_(false), selected_index_(0), max_items_(15),  // 增加最大显示项数
      cursor_row_(0), cursor_col_(0), screen_width_(80), screen_height_(24),
      popup_x_(0), popup_y_(0), popup_width_(70), popup_height_(17) {  // 增加默认宽度和高度
}

void CompletionPopup::show(const std::vector<features::CompletionItem>& items,
                           int cursor_row, int cursor_col,
                           int screen_width, int screen_height) {
    items_ = items;
    cursor_row_ = cursor_row;
    cursor_col_ = cursor_col;
    screen_width_ = screen_width;
    screen_height_ = screen_height;
    selected_index_ = 0;
    visible_ = !items_.empty();
    
    if (visible_) {
        calculatePopupPosition();
    }
}

void CompletionPopup::updateCursorPosition(int row, int col, int screen_width, int screen_height) {
    cursor_row_ = row;
    cursor_col_ = col;
    screen_width_ = screen_width;
    screen_height_ = screen_height;
    
    if (visible_) {
        calculatePopupPosition();
    }
}

void CompletionPopup::hide() {
    visible_ = false;
    items_.clear();
    selected_index_ = 0;
}

void CompletionPopup::selectNext() {
    if (items_.empty()) return;
    selected_index_ = (selected_index_ + 1) % items_.size();
    // 确保选中的项在可见范围内（自动滚动）
    // getDisplayStart() 和 getDisplayEnd() 会在render时自动处理滚动
}

void CompletionPopup::selectPrevious() {
    if (items_.empty()) return;
    if (selected_index_ == 0) {
        selected_index_ = items_.size() - 1;
    } else {
        selected_index_--;
    }
    // 确保选中的项在可见范围内（自动滚动）
    // getDisplayStart() 和 getDisplayEnd() 会在render时自动处理滚动
}

const features::CompletionItem* CompletionPopup::getSelectedItem() const {
    if (!visible_ || items_.empty() || selected_index_ >= items_.size()) {
        return nullptr;
    }
    return &items_[selected_index_];
}

void CompletionPopup::calculatePopupPosition() {
    // 计算弹窗宽度（根据内容自适应，但不超过屏幕宽度）
    popup_width_ = 70;  // 增加默认宽度，参考neovim
    for (const auto& item : items_) {
        size_t item_width = item.label.length();
        if (!item.detail.empty()) {
            item_width += item.detail.length() + 3;  // +3 for " - "
        }
        if (item_width > static_cast<size_t>(popup_width_)) {
            popup_width_ = std::min(static_cast<int>(item_width) + 15, screen_width_ - 4);  // 增加边距
        }
    }
    
    // 计算弹窗高度（根据显示项数）
    size_t display_count = std::min(items_.size(), max_items_);
    popup_height_ = static_cast<int>(display_count);  // 移除标题栏和分隔线
    
    // 计算弹窗位置：在光标下方
    // X位置：光标列位置，但如果会超出屏幕则左移
    popup_x_ = cursor_col_;
    if (popup_x_ + popup_width_ > screen_width_ - 2) {
        popup_x_ = screen_width_ - popup_width_ - 2;
        if (popup_x_ < 0) {
            popup_x_ = 0;
            popup_width_ = screen_width_ - 2;
        }
    }
    
    // Y位置：光标行下方，但如果空间不够则显示在上方
    // 假设每行高度为1，状态栏等占用约6行
    int available_height = screen_height_ - 6;  // 减去状态栏等
    int cursor_screen_y = cursor_row_;  // 简化：假设行号对应屏幕行
    
    if (cursor_screen_y + popup_height_ + 1 < available_height) {
        // 显示在下方
        popup_y_ = cursor_screen_y + 1;
    } else if (cursor_screen_y > popup_height_ + 1) {
        // 显示在上方
        popup_y_ = cursor_screen_y - popup_height_ - 1;
    } else {
        // 空间不够，显示在下方（即使会超出屏幕）
        popup_y_ = cursor_screen_y + 1;
    }
}

std::string CompletionPopup::getKindIcon(const std::string& kind) const {
    // LSP CompletionItemKind 枚举值
    // 1=Text, 2=Method, 3=Function, 4=Constructor, 5=Field, 6=Variable,
    // 7=Class, 8=Interface, 9=Module, 10=Property, 11=Unit, 12=Value,
    // 13=Enum, 14=Keyword, 15=Snippet, 16=Color, 17=File, 18=Reference,
    // 19=Folder, 20=EnumMember, 21=Constant, 22=Struct, 23=Event,
    // 24=Operator, 25=TypeParameter
    
    if (kind.empty()) return " ";
    
    int kind_num = 0;
    try {
        kind_num = std::stoi(kind);
    } catch (...) {
        return " ";
    }
    
    switch (kind_num) {
        case 1: return " ";  // Text
        case 2: return icons::CODE;  // Method
        case 3: return icons::CODE;  // Function
        case 4: return icons::CODE;  // Constructor
        case 5: return " ";  // Field
        case 6: return " ";  // Variable
        case 7: return icons::CODE;  // Class
        case 8: return icons::CODE;  // Interface
        case 9: return icons::FOLDER;  // Module
        case 10: return " ";  // Property
        case 11: return " ";  // Unit
        case 12: return " ";  // Value
        case 13: return icons::CODE;  // Enum
        case 14: return icons::CODE;  // Keyword
        case 15: return icons::CODE;  // Snippet
        case 16: return " ";  // Color
        case 17: return icons::FILE;  // File
        case 18: return " ";  // Reference
        case 19: return icons::FOLDER;  // Folder
        case 20: return icons::CODE;  // EnumMember
        case 21: return " ";  // Constant
        case 22: return icons::CODE;  // Struct
        case 23: return " ";  // Event
        case 24: return " ";  // Operator
        case 25: return icons::CODE;  // TypeParameter
        default: return " ";
    }
}

Color CompletionPopup::getKindColor(const std::string& kind) const {
    if (kind.empty()) return Color::Default;
    
    int kind_num = 0;
    try {
        kind_num = std::stoi(kind);
    } catch (...) {
        return Color::Default;
    }
    
    switch (kind_num) {
        case 2: case 3: case 4:  // Method, Function, Constructor
            return Color::Cyan;
        case 7: case 8: case 22:  // Class, Interface, Struct
            return Color::Blue;
        case 14:  // Keyword
            return Color::Magenta;
        case 17:  // File
            return Color::Yellow;
        case 19:  // Folder
            return Color::Blue;
        default:
            return Color::Default;
    }
}

Element CompletionPopup::renderItem(const features::CompletionItem& item, 
                                   bool is_selected, 
                                   const ui::Theme& theme) const {
    const auto& colors = theme.getColors();
    
    Elements item_elements;
    
    // 图标（参考neovim，使用更简洁的样式）
    std::string icon = getKindIcon(item.kind);
    Color icon_color = getKindColor(item.kind);
    item_elements.push_back(
        text(icon.empty() ? " " : icon) | color(icon_color) | size(WIDTH, EQUAL, 2)
    );
    item_elements.push_back(text(" "));  // 图标和文本之间的间距
    
    // 标签（主要文本）
    std::string label = item.label;
    size_t max_label_width = popup_width_ - 25;  // 预留空间给图标、detail等
    if (label.length() > max_label_width) {
        label = label.substr(0, max_label_width - 3) + "...";
    }
    
    Element label_elem = text(label);
    if (is_selected) {
        // 选中项：使用主题的当前行背景色，参考neovim
        label_elem = label_elem | color(colors.foreground) | bold;
    } else {
        label_elem = label_elem | color(colors.foreground);
    }
    item_elements.push_back(label_elem);
    
    // 详细信息（detail）- 只在选中时显示，参考neovim
    if (!item.detail.empty() && is_selected) {
        std::string detail = item.detail;
        size_t max_detail_width = popup_width_ - label.length() - 30;
        if (detail.length() > max_detail_width) {
            detail = detail.substr(0, max_detail_width - 3) + "...";
        }
        item_elements.push_back(text(" "));
        item_elements.push_back(
            text(detail) | color(colors.comment) | dim
        );
    }
    
    // 填充剩余空间
    item_elements.push_back(filler());
    
    // 构建行元素
    Element line = hbox(item_elements);
    
    // 选中状态样式（参考neovim：使用当前行背景色）
    if (is_selected) {
        line = line | bgcolor(colors.current_line) | color(colors.foreground);
    } else {
        line = line | bgcolor(colors.background);
    }
    
    return line;
}

Element CompletionPopup::render(const ui::Theme& theme) const {
    if (!visible_ || items_.empty()) {
        return text("");
    }
    
    const auto& colors = theme.getColors();
    
    // 计算显示范围
    size_t display_start = getDisplayStart();
    size_t display_end = getDisplayEnd();
    
    Elements lines;
    
    // 参考neovim：不显示标题栏，直接显示补全项，更简洁
    // 只在底部显示选中项信息（可选）
    
    // 显示补全项
    for (size_t i = display_start; i < display_end && i < items_.size(); ++i) {
        const auto& item = items_[i];
        bool is_selected = (i == selected_index_);
        lines.push_back(renderItem(item, is_selected, theme));
    }
    
    // 限制最大高度
    size_t max_height = max_items_;
    if (lines.size() > max_height) {
        lines.resize(max_height);
    }
    
    // 构建弹窗（参考neovim：使用更简洁的边框样式）
    Element popup = vbox(lines) | borderRounded | bgcolor(colors.background);
    
    // 设置固定尺寸以避免抖动
    popup = popup | size(WIDTH, EQUAL, popup_width_) 
                  | size(HEIGHT, EQUAL, popup_height_);
    
    return popup;
}

std::string CompletionPopup::applySelected() const {
    const auto* item = getSelectedItem();
    if (!item) {
        return "";
    }
    
    // 优先使用 insertText，否则使用 label
    return item->insertText.empty() ? item->label : item->insertText;
}

size_t CompletionPopup::getDisplayStart() const {
    if (items_.size() <= max_items_) {
        return 0;
    }
    
    // 确保选中的项在可见范围内
    if (selected_index_ < max_items_ / 2) {
        return 0;
    } else if (selected_index_ + max_items_ / 2 >= items_.size()) {
        return items_.size() - max_items_;
    } else {
        return selected_index_ - max_items_ / 2;
    }
}

size_t CompletionPopup::getDisplayEnd() const {
    size_t start = getDisplayStart();
    return std::min(start + max_items_, items_.size());
}

} // namespace ui
} // namespace pnana
