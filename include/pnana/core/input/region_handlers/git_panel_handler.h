#ifndef PNANA_CORE_INPUT_REGION_HANDLERS_GIT_PANEL_HANDLER_H
#define PNANA_CORE_INPUT_REGION_HANDLERS_GIT_PANEL_HANDLER_H

#include "core/input/base_region_handler.h"

namespace pnana {
namespace core {
namespace input {

class GitPanelHandler : public BaseRegionHandler {
  public:
    GitPanelHandler();
    ~GitPanelHandler() override = default;

    bool handleInput(ftxui::Event event, Editor* editor) override;
    bool handleNavigation(ftxui::Event event, Editor* editor) override;

    // 获取支持的快捷键列表
    std::vector<pnana::input::KeyAction> getSupportedActions() const override;

    // 获取区域类型
    EditorRegion getRegionType() const override {
        return EditorRegion::GIT_PANEL;
    }

  private:
    bool isGitPanelActive(Editor* editor) const;
};

} // namespace input
} // namespace core
} // namespace pnana

#endif // PNANA_CORE_INPUT_REGION_HANDLERS_GIT_PANEL_HANDLER_H
