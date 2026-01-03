# pnana 脚本集合

本目录包含用于构建、安装和管理 pnana 编辑器的各种实用脚本。这些脚本旨在简化开发流程、依赖管理和用户安装体验。

## 脚本概览

| 脚本名称 | 功能描述 | 主要用途 |
|---------|---------|---------|
| [`install_dependencies.sh`](#install_dependenciessh) | 安装构建 pnana 所需的所有依赖 | 开发环境设置 |
| [`install_package.sh`](#install_packagesh) | 安装预编译的 pnana 软件包 | 用户安装 |
| [`generate_release_notes.sh`](#generate_release_notessh) | 生成版本发布说明 | 版本发布 |

---

## install_dependencies.sh

### 功能描述
自动安装构建 pnana 所需的所有依赖库和工具，支持多种 Linux 发行版和 macOS。该脚本会自动检测操作系统并选择适当的安装方法，大大简化了开发环境的搭建过程。

### 支持的系统
- **Ubuntu/Debian** (18.04+)
- **Fedora/RHEL/CentOS** (7+)
- **macOS** (10.14+)

### 安装的依赖
- **构建工具**: cmake, g++, build-essential
- **核心库**: FTXUI (终端 UI 库)
- **JSON 处理**: nlohmann/json
- **语法高亮**: Tree-sitter 及多种语言解析器
- **LSP 支持**: jsonrpccxx (作为子模块包含)
- **可选依赖**: FFmpeg (用于媒体文件预览)

### 使用方法

#### 基本用法
```bash
# 克隆仓库后进入项目目录
cd /path/to/pnana

# 运行依赖安装脚本
./scripts/install_dependencies.sh
```

#### 高级选项
```bash
# 显示帮助信息
./scripts/install_dependencies.sh --help

# 只安装特定组件
./scripts/install_dependencies.sh --components basic,lsp

# 跳过某些组件的安装
./scripts/install_dependencies.sh --skip ffmpeg
```

#### 输出示例
```
[INFO] 检测到操作系统: Ubuntu 20.04
[INFO] 安装构建工具...
[SUCCESS] 构建工具安装完成
[INFO] 安装 FTXUI...
[SUCCESS] FTXUI 安装完成
[INFO] 安装 LSP 依赖 (nlohmann/json)...
[SUCCESS] LSP 依赖安装完成
[INFO] 安装 Tree-sitter...
[SUCCESS] Tree-sitter 安装完成
[INFO] 安装 Tree-sitter 语言库...
[SUCCESS] Tree-sitter 语言库安装完成
[SUCCESS] 所有依赖安装完成！
```

### 故障排除

#### 常见问题
1. **权限错误**
   ```bash
   # 确保脚本有执行权限
   chmod +x scripts/install_dependencies.sh
   
   # 如果需要，使用 sudo 运行
   sudo ./scripts/install_dependencies.sh
   ```

2. **包管理器错误**
   ```bash
   # 更新包管理器缓存
   sudo apt update  # Ubuntu/Debian
   sudo dnf update  # Fedora
   ```

3. **某些包无法安装**
   ```bash
   # 脚本会尝试从源码编译安装
   # 如果失败，请查看错误信息并手动安装
   ```

#### 手动安装依赖
如果自动安装失败，可以参考以下手动安装方法：

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install -y cmake build-essential g++ libftxui-dev nlohmann-json3-dev libtree-sitter-dev
```

**Fedora/RHEL:**
```bash
sudo dnf install -y cmake gcc-c++ ftxui-devel nlohmann_json-devel tree-sitter-devel
```

**macOS:**
```bash
brew install cmake ftxui nlohmann-json tree-sitter
```

---

## install_package.sh

### 功能描述
用于安装通用 Linux (.tar.gz) 格式的 pnana 软件包并创建相关配置文件。该脚本会自动处理二进制文件安装、配置文件创建、插件和主题安装，以及桌面快捷方式设置，提供完整的用户安装体验。

### 主要特性
- 自动解压和安装二进制文件
- 创建用户配置目录结构
- 安装默认插件和主题
- 创建桌面快捷方式
- 生成卸载脚本
- 支持自定义安装路径和配置目录
- 提供详细的安装日志和错误处理

### 命令行选项

| 选项 | 描述 | 默认值 |
|------|------|--------|
| `-h, --help` | 显示帮助信息 | - |
| `-d, --dir DIR` | 指定安装目录 | /usr/local |
| `-c, --config DIR` | 指定配置目录 | ~/.config/pnana |
| `--no-desktop` | 不创建桌面快捷方式 | 创建 |
| `--no-plugins` | 不安装默认插件 | 安装 |
| `--no-themes` | 不安装默认主题 | 安装 |
| `--dry-run` | 仅显示将要执行的操作，不实际执行 | 实际执行 |

### 使用方法

#### 基本安装
```bash
# 下载预编译包
wget https://github.com/your-username/pnana/releases/download/v1.0.0/pnana-1.0.0-linux-x86_64.tar.gz

# 安装到默认位置
./scripts/install_package.sh pnana-1.0.0-linux-x86_64.tar.gz
```

#### 自定义安装
```bash
# 安装到 /opt 目录
./scripts/install_package.sh -d /opt pnana-1.0.0-linux-x86_64.tar.gz

# 使用自定义配置目录
./scripts/install_package.sh -c ~/.pnana pnana-1.0.0-linux-x86_64.tar.gz

# 组合多个选项
./scripts/install_package.sh -d /opt -c ~/.pnana --no-desktop pnana-1.0.0-linux-x86_64.tar.gz
```

#### 预览安装操作
```bash
# 查看将要执行的操作，不实际安装
./scripts/install_package.sh --dry-run pnana-1.0.0-linux-x86_64.tar.gz
```

#### 输出示例
```
[INFO] 检查依赖...
[SUCCESS] 依赖检查完成
[INFO] 提取包信息...
[SUCCESS] 包信息提取完成
  包名: pnana-1.0.0-linux-x86_64
  版本: pnana version 1.0.0
  架构: x86_64
[INFO] 安装二进制文件...
[SUCCESS] 二进制文件已安装到 /usr/local/bin/pnana
[INFO] 创建配置目录...
[SUCCESS] 配置目录已创建: ~/.config/pnana
[INFO] 安装配置文件...
[SUCCESS] 配置文件已安装
[INFO] 安装插件...
[SUCCESS] 插件已安装
[INFO] 创建桌面快捷方式...
[SUCCESS] 桌面快捷方式已创建
[INFO] 生成卸载脚本...
[SUCCESS] 卸载脚本已创建: /usr/local/bin/pnana-uninstall
[SUCCESS] pnana 安装完成！
```

### 安装后的文件结构
```
/usr/local/bin/pnana                 # 二进制文件
~/.config/pnana/config.json         # 配置文件
~/.config/pnana/plugins/            # 插件目录
~/.config/pnana/themes/             # 主题目录
~/.local/share/applications/pnana.desktop  # 桌面快捷方式
/usr/local/bin/pnana-uninstall       # 卸载脚本
```

### 卸载

#### 基本卸载
```bash
# 使用生成的卸载脚本
sudo /usr/local/bin/pnana-uninstall
```

#### 部分卸载
```bash
# 仅删除配置文件
sudo /usr/local/bin/pnana-uninstall --config-only

# 仅删除桌面快捷方式
sudo /usr/local/bin/pnana-uninstall --desktop-only

# 仅删除二进制文件
sudo /usr/local/bin/pnana-uninstall --binary-only
```

#### 卸载脚本选项
```bash
# 查看卸载脚本帮助
sudo /usr/local/bin/pnana-uninstall --help

# 预览将要删除的文件
sudo /usr/local/bin/pnana-uninstall --dry-run
```

### 故障排除

#### 常见问题
1. **权限错误**
   ```bash
   # 确保脚本有执行权限
   chmod +x scripts/install_package.sh
   
   # 使用 sudo 运行（如果需要写入系统目录）
   sudo ./scripts/install_package.sh pnana-1.0.0-linux-x86_64.tar.gz
   ```

2. **PATH 环境变量问题**
   ```bash
   # 检查安装目录是否在 PATH 中
   echo $PATH | grep /usr/local/bin
   
   # 如果不在，添加到 ~/.bashrc 或 ~/.zshrc
   echo 'export PATH="$PATH:/usr/local/bin"' >> ~/.bashrc
   source ~/.bashrc
   ```

3. **桌面快捷方式不工作**
   ```bash
   # 检查桌面文件是否正确安装
   ls -la ~/.local/share/applications/pnana.desktop
   
   # 更新桌面数据库
   update-desktop-database ~/.local/share/applications/
   ```

4. **配置文件问题**
   ```bash
   # 重置配置文件
   rm -rf ~/.config/pnana
   pnana  # 启动时会重新创建默认配置
   ```

#### 高级使用场景

**仅安装二进制文件**
```bash
./scripts/install_package.sh --no-plugins --no-themes --no-desktop pnana-1.0.0-linux-x86_64.tar.gz
```

**安装到用户目录**
```bash
./scripts/install_package.sh -d ~/.local pnana-1.0.0-linux-x86_64.tar.gz

# 确保 ~/.local/bin 在 PATH 中
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

**多版本安装**
```bash
# 安装特定版本到不同目录
./scripts/install_package.sh -d /opt/pnana-1.0.0 -c ~/.config/pnana-1.0.0 pnana-1.0.0-linux-x86_64.tar.gz

# 创建启动脚本
echo '#!/bin/bash' > ~/bin/pnana-1.0.0
echo 'export PATH="/opt/pnana-1.0.0/bin:$PATH"' >> ~/bin/pnana-1.0.0
echo 'pnana "$@"' >> ~/bin/pnana-1.0.0
chmod +x ~/bin/pnana-1.0.0
```

详细使用说明请参考 [INSTALL_PACKAGE_README.md](INSTALL_PACKAGE_README.md)

---

## generate_release_notes.sh

### 功能描述
基于模板生成版本发布说明，用于创建 GitHub Release 或其他发布渠道的版本说明文档。该脚本会自动填充版本号、日期和其他相关信息，生成一个可以进一步编辑的 Markdown 文件。

### 使用方法

#### 基本用法
```bash
# 生成当前版本的发布说明
./scripts/generate_release_notes.sh 1.0.0

# 指定上一个版本（用于比较变更）
./scripts/generate_release_notes.sh 1.0.0 0.9.0
```

#### 在 GitHub Actions 中使用
```yaml
- name: Generate Release Notes
  run: |
    chmod +x scripts/generate_release_notes.sh
    ./scripts/generate_release_notes.sh ${{ github.ref_name }}
```

#### 输出示例
```
已生成发布说明: release_notes_1.0.0.md
请编辑此文件，添加实际的版本更新内容，然后用于 GitHub Release
```

#### 生成的文件结构
脚本会创建一个临时 Markdown 文件，包含以下部分：
- 版本信息和发布日期
- 下载链接
- 安装说明
- 变更日志占位符
- 已知问题占位符

#### 自定义模板
可以通过修改 `RELEASE.md` 文件来自定义发布说明模板。脚本会替换以下占位符：
- `{VERSION}`: 当前版本号
- `{DATE}`: 当前日期
- `{USERNAME}`: GitHub 用户名
- `{PREV_VERSION}`: 上一个版本号
- `{PREV_RELEASE_URL}`: 上一个版本的发布页面 URL

---

## 脚本开发指南

### 添加新脚本
1. 确保脚本具有可执行权限：
   ```bash
   chmod +x scripts/your_script.sh
   ```
2. 在脚本开头添加标准注释块，说明脚本用途和基本用法
3. 实现基本的错误处理和日志输出
4. 添加命令行参数解析和帮助信息
5. 更新本 README.md 文件，添加新脚本的说明

### 脚本规范
- 使用 `set -e` 确保脚本在遇到错误时立即退出
- 实现基本的命令行参数解析
- 提供帮助信息 (`--help` 参数)
- 使用颜色输出增强用户体验
- 添加适当的错误处理和用户友好的错误消息
- 支持预览模式 (`--dry-run`)
- 提供详细的日志输出

### 代码示例
```bash
#!/bin/bash

# 脚本描述
# 用法: ./scripts/your_script.sh [选项] <参数>

set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 默认配置
DRY_RUN=false

# 显示帮助信息
show_help() {
    echo -e "${BLUE}脚本名称${NC}"
    echo ""
    echo "用法: $0 [选项] <参数>"
    echo ""
    echo "选项:"
    echo "  -h, --help              显示此帮助信息"
    echo "  --dry-run               仅显示将要执行的操作，不实际执行"
    echo ""
}

# 解析命令行参数
parse_args() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_help
                exit 0
                ;;
            --dry-run)
                DRY_RUN=true
                shift
                ;;
            *)
                PARAM="$1"
                shift
                ;;
        esac
    done
}

# 执行操作
execute() {
    local cmd="$1"
    if [[ "$DRY_RUN" == true ]]; then
        echo -e "${YELLOW}[DRY-RUN]${NC} $cmd"
    else
        echo -e "${BLUE}[EXEC]${NC} $cmd"
        eval "$cmd"
    fi
}

# 主函数
main() {
    parse_args "$@"
    
    # 脚本逻辑
    execute "echo 'Hello World'"
}

# 执行主函数
main "$@"
```

### 测试
在提交新脚本或修改现有脚本之前，请确保：
1. 脚本在不同系统上可以正常运行
2. 脚本具有适当的错误处理
3. 帮助信息准确且有用
4. 脚本遵循项目的编码规范
5. 添加了适当的日志输出

### 文档
- 为新脚本添加详细的文档
- 包含使用示例和常见问题解答
- 更新本 README.md 文件
- 考虑创建单独的文档文件（如 INSTALL_PACKAGE_README.md）

---

## 贡献

如果您想为脚本集合做出贡献：
1. Fork 项目仓库
2. 创建功能分支
3. 添加或修改脚本
4. 更新相关文档
5. 确保脚本有适当的测试
6. 提交 Pull Request

### 贡献指南
- 遵循现有的代码风格和约定
- 添加适当的注释和文档
- 确保脚本在不同系统上的兼容性
- 测试您的更改
- 提供清晰的提交信息

---

## 许可证

这些脚本遵循与 pnana 项目相同的许可证。