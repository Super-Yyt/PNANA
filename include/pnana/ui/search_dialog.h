#ifndef PNANA_UI_SEARCH_DIALOG_H
#define PNANA_UI_SEARCH_DIALOG_H

#include "features/search.h"
#include "ui/theme.h"
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <functional>
#include <string>

namespace pnana {
namespace ui {

class SearchDialog {
  public:
    SearchDialog(Theme& theme);

    void show(std::function<void(const std::string&, const features::SearchOptions&)> on_search,
              std::function<void(const std::string&)> on_replace,
              std::function<void(const std::string&)> on_replace_all,
              std::function<void()> on_cancel);

    void hide();

    bool isVisible() const {
        return visible_;
    }

    bool handleInput(ftxui::Event event);

    ftxui::Element render();

    // 更新搜索结果统计
    void updateResults(size_t current_match, size_t total_matches);

    // 设置当前搜索模式
    void setSearchOptions(const features::SearchOptions& options);

    // 获取当前输入
    std::string getCurrentInput() const {
        return search_input_;
    }

  private:
    Theme& theme_;
    bool visible_;
    int current_field_;
    size_t cursor_position_;

    // 搜索输入
    std::string search_input_;

    // 替换输入
    std::string replace_input_;

    // 搜索选项
    features::SearchOptions search_options_;

    // 结果统计
    size_t current_match_;
    size_t total_matches_;

    // 回调函数
    std::function<void(const std::string&, const features::SearchOptions&)> on_search_;
    std::function<void(const std::string&)> on_replace_;
    std::function<void(const std::string&)> on_replace_all_;
    std::function<void()> on_cancel_;

    // 内部方法
    void insertChar(char ch);
    void deleteChar();
    void backspace();
    void moveCursorLeft();
    void moveCursorRight();
    std::string* getCurrentField();
    void toggleOption(int option_index);
    void performSearch();
    void performReplace();
    void performReplaceAll();

    // 渲染辅助方法
    ftxui::Element renderSearchInput();
    ftxui::Element renderReplaceInput();
    ftxui::Element renderOptions();
    ftxui::Element renderResults();
    ftxui::Element renderButtons();
};

} // namespace ui
} // namespace pnana

#endif // PNANA_UI_SEARCH_DIALOG_H
