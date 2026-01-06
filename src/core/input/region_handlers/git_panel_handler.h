#ifndef PNANA_CORE_INPUT_REGION_HANDLERS_GIT_PANEL_HANDLER_H
#define PNANA_CORE_INPUT_REGION_HANDLERS_GIT_PANEL_HANDLER_H

#include "core/input/region_handlers/base_handler.h"

namespace pnana {
namespace core {
namespace input {

class GitPanelHandler : public BaseHandler {
  public:
    GitPanelHandler();
    ~GitPanelHandler() override = default;

    bool handleInput(ftxui::Event event, Editor* editor) override;
    bool handleNavigation(ftxui::Event event, Editor* editor) override;

  private:
    bool isGitPanelActive(Editor* editor) const;
};

} // namespace input
} // namespace core
} // namespace pnana

#endif // PNANA_CORE_INPUT_REGION_HANDLERS_GIT_PANEL_HANDLER_H
