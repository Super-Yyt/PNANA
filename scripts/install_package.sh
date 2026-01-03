#!/bin/bash

# pnana 安装脚本
# 用于安装通用 Linux (.tar.gz) 格式的 pnana 软件包并创建相关配置文件

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 默认配置
INSTALL_DIR="/usr/local"
CONFIG_DIR="$HOME/.config/pnana"
PLUGINS_DIR="$CONFIG_DIR/plugins"
THEMES_DIR="$CONFIG_DIR/themes"
DESKTOP_FILE_DIR="$HOME/.local/share/applications"
DESKTOP_FILE="pnana.desktop"
UNINSTALL_SCRIPT="$INSTALL_DIR/bin/pnana-uninstall"

# 显示帮助信息
show_help() {
    echo -e "${BLUE}pnana 安装脚本${NC}"
    echo ""
    echo "用法: $0 [选项] <tar.gz 文件路径>"
    echo ""
    echo "选项:"
    echo "  -h, --help              显示此帮助信息"
    echo "  -d, --dir DIR           指定安装目录 (默认: $INSTALL_DIR)"
    echo "  -c, --config DIR        指定配置目录 (默认: $CONFIG_DIR)"
    echo "  --no-desktop            不创建桌面快捷方式"
    echo "  --no-plugins            不安装默认插件"
    echo "  --no-themes             不安装默认主题"
    echo ""
    echo "示例:"
    echo "  $0 pnana-1.0.0-linux-x86_64.tar.gz"
    echo "  $0 -d /opt pnana-1.0.0-linux-x86_64.tar.gz"
    echo "  $0 --no-desktop pnana-1.0.0-linux-x86_64.tar.gz"
}

# 解析命令行参数
parse_args() {
    CREATE_DESKTOP=true
    INSTALL_PLUGINS=true
    INSTALL_THEMES=true
    
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_help
                exit 0
                ;;
            -d|--dir)
                INSTALL_DIR="$2"
                shift 2
                ;;
            -c|--config)
                CONFIG_DIR="$2"
                PLUGINS_DIR="$CONFIG_DIR/plugins"
                THEMES_DIR="$CONFIG_DIR/themes"
                shift 2
                ;;
            --no-desktop)
                CREATE_DESKTOP=false
                shift
                ;;
            --no-plugins)
                INSTALL_PLUGINS=false
                shift
                ;;
            --no-themes)
                INSTALL_THEMES=false
                shift
                ;;
            -*)
                echo -e "${RED}错误: 未知选项 $1${NC}" >&2
                show_help
                exit 1
                ;;
            *)
                PACKAGE_FILE="$1"
                shift
                ;;
        esac
    done
    
    if [[ -z "$PACKAGE_FILE" ]]; then
        echo -e "${RED}错误: 请指定要安装的 tar.gz 文件${NC}" >&2
        show_help
        exit 1
    fi
    
    if [[ ! -f "$PACKAGE_FILE" ]]; then
        echo -e "${RED}错误: 文件 $PACKAGE_FILE 不存在${NC}" >&2
        exit 1
    fi
}

# 检查依赖
check_dependencies() {
    echo -e "${BLUE}检查依赖...${NC}"
    
    local missing_deps=()
    
    # 检查基本命令
    for cmd in tar mkdir; do
        if ! command -v $cmd &> /dev/null; then
            missing_deps+=($cmd)
        fi
    done
    
    # 检查可选依赖
    if [[ "$CREATE_DESKTOP" == true ]]; then
        if ! command -v desktop-file-validate &> /dev/null; then
            echo -e "${YELLOW}警告: desktop-file-validate 未找到，将跳过桌面文件验证${NC}"
        fi
    fi
    
    if [[ ${#missing_deps[@]} -gt 0 ]]; then
        echo -e "${RED}错误: 缺少依赖: ${missing_deps[*]}${NC}" >&2
        exit 1
    fi
    
    echo -e "${GREEN}依赖检查完成${NC}"
}

# 提取包信息
extract_package_info() {
    echo -e "${BLUE}提取包信息...${NC}"
    
    # 获取包名（不含扩展名）
    PACKAGE_NAME=$(basename "$PACKAGE_FILE" .tar.gz)
    
    # 创建临时目录
    TEMP_DIR=$(mktemp -d)
    
    # 解压包到临时目录
    echo -e "${BLUE}解压包...${NC}"
    tar -xzf "$PACKAGE_FILE" -C "$TEMP_DIR"
    
    # 查找解压后的目录
    EXTRACTED_DIR=$(find "$TEMP_DIR" -maxdepth 1 -type d | head -n 2 | tail -n 1)
    
    if [[ ! -d "$EXTRACTED_DIR" ]]; then
        echo -e "${RED}错误: 无法找到解压后的目录${NC}" >&2
        rm -rf "$TEMP_DIR"
        exit 1
    fi
    
    # 查找二进制文件
    BINARY_PATH=$(find "$EXTRACTED_DIR" -name "pnana" -type f -executable | head -n 1)
    
    if [[ -z "$BINARY_PATH" ]]; then
        echo -e "${RED}错误: 在包中找不到 pnana 二进制文件${NC}" >&2
        rm -rf "$TEMP_DIR"
        exit 1
    fi
    
    # 检查二进制文件的架构
    BINARY_ARCH=$(file "$BINARY_PATH" | grep -o -E 'x86-64|x86_64|i386|i686|arm64|aarch64|arm' | head -n 1)
    
    if [[ -z "$BINARY_ARCH" ]]; then
        BINARY_ARCH="unknown"
    fi
    
    # 获取版本信息
    VERSION_INFO=""
    if [[ -x "$BINARY_PATH" ]]; then
        VERSION_INFO=$("$BINARY_PATH" --version 2>&1 | head -n 1 || echo "")
    fi
    
    echo -e "${GREEN}包信息提取完成${NC}"
    echo "  包名: $PACKAGE_NAME"
    echo "  版本: $VERSION_INFO"
    echo "  架构: $BINARY_ARCH"
}

# 安装二进制文件
install_binary() {
    echo -e "${BLUE}安装二进制文件...${NC}"
    
    # 创建安装目录
    sudo mkdir -p "$INSTALL_DIR/bin"
    
    # 复制二进制文件
    sudo cp "$BINARY_PATH" "$INSTALL_DIR/bin/pnana"
    
    # 设置可执行权限
    sudo chmod +x "$INSTALL_DIR/bin/pnana"
    
    echo -e "${GREEN}二进制文件已安装到 $INSTALL_DIR/bin/pnana${NC}"
}

# 创建配置目录
create_config_dirs() {
    echo -e "${BLUE}创建配置目录...${NC}"
    
    # 创建主配置目录
    mkdir -p "$CONFIG_DIR"
    
    # 创建插件目录
    if [[ "$INSTALL_PLUGINS" == true ]]; then
        mkdir -p "$PLUGINS_DIR"
    fi
    
    # 创建主题目录
    if [[ "$INSTALL_THEMES" == true ]]; then
        mkdir -p "$THEMES_DIR"
    fi
    
    echo -e "${GREEN}配置目录已创建${NC}"
}

# 安装配置文件
install_config() {
    echo -e "${BLUE}安装配置文件...${NC}"
    
    # 查找包中的配置文件
    CONFIG_SOURCE=$(find "$EXTRACTED_DIR" -name "*.json" -path "*/config/*" | head -n 1)
    
    if [[ -n "$CONFIG_SOURCE" ]]; then
        # 如果找到配置文件，复制到配置目录
        cp "$CONFIG_SOURCE" "$CONFIG_DIR/config.json"
        echo -e "${GREEN}默认配置文件已安装到 $CONFIG_DIR/config.json${NC}"
    else
        # 如果没有找到配置文件，创建一个基本的配置文件
        cat > "$CONFIG_DIR/config.json" << EOF
{
  "editor": {
    "theme": "monokai",
    "font_size": 12,
    "tab_size": 4,
    "insert_spaces": true,
    "word_wrap": false,
    "auto_indent": true
  },
  "display": {
    "show_line_numbers": true,
    "relative_line_numbers": false,
    "highlight_current_line": true,
    "show_whitespace": false
  },
  "files": {
    "encoding": "UTF-8",
    "line_ending": "LF",
    "trim_trailing_whitespace": true,
    "insert_final_newline": true,
    "auto_save": false,
    "auto_save_interval": 60
  },
  "search": {
    "case_sensitive": false,
    "whole_word": false,
    "regex": false,
    "wrap_around": true
  },
  "themes": {
    "current": "monokai"
  }
}
EOF
        echo -e "${GREEN}基本配置文件已创建到 $CONFIG_DIR/config.json${NC}"
    fi
}

# 安装插件
install_plugins() {
    if [[ "$INSTALL_PLUGINS" != true ]]; then
        return
    fi
    
    echo -e "${BLUE}安装插件...${NC}"
    
    # 查找包中的插件目录
    PLUGINS_SOURCE=$(find "$EXTRACTED_DIR" -name "plugins" -type d | head -n 1)
    
    if [[ -n "$PLUGINS_SOURCE" ]]; then
        # 如果找到插件目录，复制到配置目录
        cp -r "$PLUGINS_SOURCE"/* "$PLUGINS_DIR/"
        echo -e "${GREEN}插件已安装到 $PLUGINS_DIR${NC}"
    else
        # 如果没有找到插件目录，创建一个基本的插件示例
        mkdir -p "$PLUGINS_DIR/example"
        cat > "$PLUGINS_DIR/example/init.lua" << EOF
-- 示例插件
-- 这是一个简单的 pnana 插件示例

function pnana.init()
    -- 插件初始化代码
    pnana.log("示例插件已加载")
end

function pnana.on_file_open(filename)
    -- 文件打开时触发
    pnana.log("打开文件: " .. filename)
end

function pnana.on_key_pressed(key)
    -- 按键按下时触发
    -- 返回 true 表示已处理该按键，false 表示继续处理
    return false
end

function pnana.cleanup()
    -- 插件清理代码
    pnana.log("示例插件已卸载")
end
EOF
        echo -e "${GREEN}示例插件已创建到 $PLUGINS_DIR/example/init.lua${NC}"
    fi
}

# 安装主题
install_themes() {
    if [[ "$INSTALL_THEMES" != true ]]; then
        return
    fi
    
    echo -e "${BLUE}安装主题...${NC}"
    
    # 查找包中的主题目录
    THEMES_SOURCE=$(find "$EXTRACTED_DIR" -name "themes" -type d | head -n 1)
    
    if [[ -n "$THEMES_SOURCE" ]]; then
        # 如果找到主题目录，复制到配置目录
        cp -r "$THEMES_SOURCE"/* "$THEMES_DIR/"
        echo -e "${GREEN}主题已安装到 $THEMES_DIR${NC}"
    else
        # 如果没有找到主题目录，创建一个基本的主题示例
        mkdir -p "$THEMES_DIR"
        cat > "$THEMES_DIR/custom.json" << EOF
{
  "name": "自定义主题",
  "background": [30, 30, 30],
  "foreground": [255, 255, 255],
  "current_line": [50, 50, 50],
  "selection": [60, 60, 60],
  "line_number": [150, 150, 150],
  "line_number_current": [255, 255, 255],
  "statusbar_bg": [40, 40, 40],
  "statusbar_fg": [255, 255, 255],
  "menubar_bg": [25, 25, 25],
  "menubar_fg": [255, 255, 255],
  "helpbar_bg": [40, 40, 40],
  "helpbar_fg": [150, 150, 150],
  "helpbar_key": [100, 200, 100],
  "keyword": [255, 100, 100],
  "string": [100, 255, 100],
  "comment": [150, 150, 150],
  "number": [255, 200, 100],
  "function": [100, 150, 255],
  "type": [200, 100, 255],
  "operator_color": [255, 100, 100],
  "error": [255, 50, 50],
  "warning": [255, 200, 50],
  "info": [50, 150, 255],
  "success": [50, 255, 50]
}
EOF
        echo -e "${GREEN}示例主题已创建到 $THEMES_DIR/custom.json${NC}"
    fi
}

# 创建桌面快捷方式
create_desktop_entry() {
    if [[ "$CREATE_DESKTOP" != true ]]; then
        return
    fi
    
    echo -e "${BLUE}创建桌面快捷方式...${NC}"
    
    # 创建应用程序目录
    mkdir -p "$DESKTOP_FILE_DIR"
    
    # 创建桌面文件
    cat > "$DESKTOP_FILE_DIR/$DESKTOP_FILE" << EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=pnana
Comment=现代化终端文本编辑器
Comment[zh_CN]=现代化终端文本编辑器
Exec=$INSTALL_DIR/bin/pnana %F
Icon=pnana
Terminal=true
Categories=TextEditor;Development;Utility;
MimeType=text/plain;text/x-chdr;text/x-csrc;text/x-c++hdr;text/x-c++src;text/x-java;text/x-perl;text/x-python;application/x-shellscript;text/x-c;text/x-c++;
StartupNotify=true
EOF
    
    # 验证桌面文件
    if command -v desktop-file-validate &> /dev/null; then
        if desktop-file-validate "$DESKTOP_FILE_DIR/$DESKTOP_FILE"; then
            echo -e "${GREEN}桌面快捷方式已创建并验证${NC}"
        else
            echo -e "${YELLOW}警告: 桌面文件验证失败，但已创建${NC}"
        fi
    else
        echo -e "${GREEN}桌面快捷方式已创建${NC}"
    fi
    
    # 更新桌面数据库
    if command -v update-desktop-database &> /dev/null; then
        update-desktop-database "$DESKTOP_FILE_DIR" 2>/dev/null || true
    fi
}

# 创建卸载脚本
create_uninstall_script() {
    echo -e "${BLUE}创建卸载脚本...${NC}"
    
    # 创建卸载脚本
    sudo tee "$UNINSTALL_SCRIPT" > /dev/null << EOF
#!/bin/bash

# pnana 卸载脚本
# 此脚本由 pnana 安装程序自动生成

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 安装信息
INSTALL_DIR="$INSTALL_DIR"
CONFIG_DIR="$CONFIG_DIR"
DESKTOP_FILE_DIR="$DESKTOP_FILE_DIR"
DESKTOP_FILE="$DESKTOP_FILE"

# 显示帮助信息
show_help() {
    echo -e "\${BLUE}pnana 卸载脚本\${NC}"
    echo ""
    echo "用法: \$0 [选项]"
    echo ""
    echo "选项:"
    echo "  -h, --help              显示此帮助信息"
    echo "  --config-only           仅删除配置文件"
    echo "  --desktop-only          仅删除桌面快捷方式"
    echo ""
}

# 解析命令行参数
CONFIG_ONLY=false
DESKTOP_ONLY=false

while [[ \$# -gt 0 ]]; do
    case \$1 in
        -h|--help)
            show_help
            exit 0
            ;;
        --config-only)
            CONFIG_ONLY=true
            shift
            ;;
        --desktop-only)
            DESKTOP_ONLY=true
            shift
            ;;
        -*)
            echo -e "\${RED}错误: 未知选项 \$1\${NC}" >&2
            show_help
            exit 1
            ;;
        *)
            shift
            ;;
    esac
done

# 卸载二进制文件
if [[ "\$DESKTOP_ONLY" != true ]]; then
    echo -e "\${BLUE}卸载二进制文件...\${NC}"
    
    if [[ -f "\$INSTALL_DIR/bin/pnana" ]]; then
        sudo rm -f "\$INSTALL_DIR/bin/pnana"
        echo -e "\${GREEN}二进制文件已删除\${NC}"
    else
        echo -e "\${YELLOW}警告: 二进制文件不存在\${NC}"
    fi
fi

# 删除配置文件
if [[ "\$DESKTOP_ONLY" != true ]]; then
    echo -e "\${BLUE}删除配置文件...\${NC}"
    
    if [[ -d "\$CONFIG_DIR" ]]; then
        read -p "是否删除配置目录 \$CONFIG_DIR? [y/N] " -n 1 -r
        echo
        if [[ \$REPLY =~ ^[Yy]\$ ]]; then
            rm -rf "\$CONFIG_DIR"
            echo -e "\${GREEN}配置目录已删除\${NC}"
        else
            echo -e "\${YELLOW}配置目录保留\${NC}"
        fi
    else
        echo -e "\${YELLOW}警告: 配置目录不存在\${NC}"
    fi
fi

# 删除桌面快捷方式
if [[ "\$CONFIG_ONLY" != true ]]; then
    echo -e "\${BLUE}删除桌面快捷方式...\${NC}"
    
    if [[ -f "\$DESKTOP_FILE_DIR/\$DESKTOP_FILE" ]]; then
        rm -f "\$DESKTOP_FILE_DIR/\$DESKTOP_FILE"
        echo -e "\${GREEN}桌面快捷方式已删除\${NC}"
        
        # 更新桌面数据库
        if command -v update-desktop-database &> /dev/null; then
            update-desktop-database "\$DESKTOP_FILE_DIR" 2>/dev/null || true
        fi
    else
        echo -e "\${YELLOW}警告: 桌面快捷方式不存在\${NC}"
    fi
fi

# 删除卸载脚本本身
echo -e "\${BLUE}删除卸载脚本...\${NC}"
sudo rm -f "\$0"

echo -e "\${GREEN}pnana 卸载完成\${NC}"
EOF
    
    # 设置可执行权限
    sudo chmod +x "$UNINSTALL_SCRIPT"
    
    echo -e "${GREEN}卸载脚本已创建到 $UNINSTALL_SCRIPT${NC}"
}

# 清理临时文件
cleanup() {
    if [[ -n "$TEMP_DIR" && -d "$TEMP_DIR" ]]; then
        rm -rf "$TEMP_DIR"
    fi
}

# 显示安装完成信息
show_completion_info() {
    echo ""
    echo -e "${GREEN}=====================================${NC}"
    echo -e "${GREEN}pnana 安装完成!${NC}"
    echo -e "${GREEN}=====================================${NC}"
    echo ""
    echo -e "${BLUE}安装信息:${NC}"
    echo "  二进制文件: $INSTALL_DIR/bin/pnana"
    echo "  配置目录: $CONFIG_DIR"
    echo "  插件目录: $PLUGINS_DIR"
    echo "  主题目录: $THEMES_DIR"
    echo ""
    
    if [[ "$CREATE_DESKTOP" == true ]]; then
        echo -e "${BLUE}桌面快捷方式:${NC}"
        echo "  文件位置: $DESKTOP_FILE_DIR/$DESKTOP_FILE"
        echo ""
    fi
    
    echo -e "${BLUE}使用方法:${NC}"
    echo "  启动编辑器: $INSTALL_DIR/bin/pnana"
    echo "  打开文件: $INSTALL_DIR/bin/pnana <文件名>"
    echo ""
    
    echo -e "${BLUE}卸载方法:${NC}"
    echo "  运行卸载脚本: $UNINSTALL_SCRIPT"
    echo ""
    
    echo -e "${YELLOW}提示:${NC}"
    echo "  - 确保 $INSTALL_DIR/bin 在您的 PATH 环境变量中"
    echo "  - 配置文件位于 $CONFIG_DIR/config.json"
    echo "  - 您可以添加更多插件到 $PLUGINS_DIR"
    echo "  - 您可以添加更多主题到 $THEMES_DIR"
    echo ""
}

# 主函数
main() {
    # 解析命令行参数
    parse_args "$@"
    
    # 设置清理陷阱
    trap cleanup EXIT
    
    # 检查依赖
    check_dependencies
    
    # 提取包信息
    extract_package_info
    
    # 安装二进制文件
    install_binary
    
    # 创建配置目录
    create_config_dirs
    
    # 安装配置文件
    install_config
    
    # 安装插件
    install_plugins
    
    # 安装主题
    install_themes
    
    # 创建桌面快捷方式
    create_desktop_entry
    
    # 创建卸载脚本
    create_uninstall_script
    
    # 显示安装完成信息
    show_completion_info
}

# 执行主函数
main "$@"