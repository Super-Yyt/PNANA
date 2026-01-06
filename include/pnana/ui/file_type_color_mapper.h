#ifndef PNANA_UI_FILE_TYPE_COLOR_MAPPER_H
#define PNANA_UI_FILE_TYPE_COLOR_MAPPER_H

#include "ui/theme.h"
#include <string>

namespace pnana {
namespace ui {

/**
 * 文件类型颜色映射器
 * 根据文件类型和扩展名返回相应的显示颜色
 */
class FileTypeColorMapper {
  public:
    explicit FileTypeColorMapper(const Theme& theme);

    /**
     * 获取文件或目录的显示颜色
     * @param filename 文件名
     * @param is_directory 是否为目录
     * @return 显示颜色
     */
    ftxui::Color getFileColor(const std::string& filename, bool is_directory) const;

    /**
     * 获取文件扩展名（小写）
     * @param filename 文件名
     * @return 文件扩展名
     */
    static std::string getFileExtension(const std::string& filename);

  private:
    const Theme& theme_;
};

} // namespace ui
} // namespace pnana

#endif // PNANA_UI_FILE_TYPE_COLOR_MAPPER_H
