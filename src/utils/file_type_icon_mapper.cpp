#include "utils/file_type_icon_mapper.h"
#include <algorithm>

namespace pnana {
namespace utils {

FileTypeIconMapper::FileTypeIconMapper() {
    // 初始化时可以加载默认配置
}

std::string FileTypeIconMapper::getIcon(const std::string& file_type) const {
    // 首先检查自定义图标
    auto custom_it = custom_icons_.find(file_type);
    if (custom_it != custom_icons_.end()) {
        return custom_it->second;
    }

    // 检查默认图标（"default" 键）
    auto default_it = custom_icons_.find("default");
    if (default_it != custom_icons_.end()) {
        return default_it->second;
    }

    // 使用 icons.h 中的内置映射
    return ui::icons::getFileTypeIcon(file_type);
}

void FileTypeIconMapper::setCustomIcon(const std::string& file_type, const std::string& icon) {
    custom_icons_[file_type] = icon;
}

void FileTypeIconMapper::clearCustomIcons() {
    custom_icons_.clear();
}

} // namespace utils
} // namespace pnana
