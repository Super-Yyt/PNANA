#include "core/input/region_handlers/git_panel_handler.h"
#include "core/editor.h"
#include "input/key_action.h"
#include <ftxui/component/event.hpp>

using namespace ftxui;

namespace pnana {
namespace core {
namespace input {

GitPanelHandler::GitPanelHandler() = default;

bool GitPanelHandler::handleInput(ftxui::Event event, Editor* editor) {
    if (!isGitPanelActive(editor)) {
        return false;
    }

    // 将事件传递给GitPanel处理
    bool handled = editor->getGitPanel().onKeyPress(event);

    // 如果事件被处理，强制UI更新
    if (handled) {
        // 注意：这里我们不能直接访问private成员，但可以通过其他方式
        // 或者我们需要添加一个公共方法来强制更新
    }

    return handled;
}

bool GitPanelHandler::handleNavigation(ftxui::Event event, Editor* editor) {
    if (!isGitPanelActive(editor)) {
        return false;
    }

    // Git面板的导航：Esc关闭面板，Tab切换区域
    if (event == Event::Escape) {
        editor->toggleGitPanel();
        return true;
    }

    if (event == Event::Tab) {
        // 从Git面板切换到代码区域
        editor->getRegionManager().setRegion(EditorRegion::CODE_AREA);
        editor->setStatusMessage("Switched to Code Area | Ctrl+G: Git Panel");
        return true;
    }

    return false;
}

std::vector<pnana::input::KeyAction> GitPanelHandler::getSupportedActions() const {
    return {}; // Git面板主要使用键盘导航，不需要特殊的快捷键
}

bool GitPanelHandler::isGitPanelActive(Editor* editor) const {
    return editor->isGitPanelVisible() &&
           editor->getRegionManager().getCurrentRegion() == EditorRegion::GIT_PANEL;
}

} // namespace input
} // namespace core
} // namespace pnana
