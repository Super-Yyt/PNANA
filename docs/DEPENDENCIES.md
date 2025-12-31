# pnana 依赖文档

本文档详细说明 pnana 项目的所有依赖项，包括必需依赖和可选依赖。

## 📋 目录

- [构建工具依赖](#构建工具依赖)
- [必需库依赖](#必需库依赖)
- [可选依赖](#可选依赖)
- [系统库依赖](#系统库依赖)
- [安装指南](#安装指南)
- [依赖版本要求](#依赖版本要求)
- [常见问题](#常见问题)

---

## 构建工具依赖

### CMake

**版本要求**: 3.10 或更高版本

**说明**: CMake 是项目的构建系统，用于配置和编译项目。

**安装方法**:

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install cmake
```

#### Fedora/RHEL
```bash
sudo dnf install cmake
```

#### macOS
```bash
brew install cmake
```

#### 验证安装
```bash
cmake --version
```

### C++ 编译器

**版本要求**: 支持 C++17 标准的编译器

**支持的编译器**:
- GCC 7.0 或更高版本
- Clang 5.0 或更高版本
- MSVC 2017 或更高版本（Windows）

**安装方法**:

#### Ubuntu/Debian (GCC)
```bash
sudo apt update
sudo apt install build-essential g++
```

#### Ubuntu/Debian (Clang)
```bash
sudo apt update
sudo apt install clang
```

#### Fedora/RHEL (GCC)
```bash
sudo dnf install gcc-c++
```

#### macOS (Clang)
```bash
xcode-select --install
```

#### 验证安装
```bash
g++ --version
# 或
clang++ --version
```

---

## 必需库依赖

### FTXUI

**版本要求**: 最新稳定版本

**说明**: FTXUI (Functional Terminal User Interface) 是一个用于创建终端用户界面的 C++ 库。pnana 使用 FTXUI 来构建所有 UI 组件。

**安装方法**:

FTXUI 通常通过 CMake 的 `find_package` 自动查找。如果系统中已安装 FTXUI，CMake 会自动使用。

#### 使用包管理器安装

##### Ubuntu/Debian
```bash
sudo apt install libftxui-dev
```

##### Fedora/RHEL
```bash
sudo dnf install ftxui-devel
```

##### macOS
```bash
brew install ftxui
```

#### 从源码安装

如果包管理器中没有 FTXUI，可以从源码安装：

```bash
git clone https://github.com/ArthurSonzogni/FTXUI.git
cd FTXUI
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install
```

**验证安装**:
```bash
pkg-config --modversion ftxui
```

---

## 可选依赖

### LSP 支持依赖

#### nlohmann/json

**版本要求**: 3.10.0 或更高版本

**说明**: nlohmann/json 是一个现代 C++ JSON 库，用于 LSP (Language Server Protocol) 支持。如果未安装，LSP 功能将被禁用，但编辑器仍可正常使用。

**安装方法**:

##### Ubuntu/Debian
```bash
sudo apt install nlohmann-json3-dev
```

##### Fedora/RHEL
```bash
sudo dnf install nlohmann_json-devel
```

##### macOS
```bash
brew install nlohmann-json
```

##### 从源码安装

如果包管理器中没有，可以从源码安装：

```bash
git clone https://github.com/nlohmann/json.git
cd json
mkdir build && cd build
cmake .. -DJSON_BuildTests=OFF
make -j$(nproc)
sudo make install
```

**验证安装**:
```bash
pkg-config --modversion nlohmann_json
```

#### jsonrpccxx

**版本要求**: 最新版本

**说明**: jsonrpccxx 是一个 C++ JSON-RPC 库，用于与 LSP 服务器通信。该库已包含在项目的 `third-party/JSON-RPC-CXX` 目录中，无需单独安装。

**注意**: 如果 `third-party/JSON-RPC-CXX` 目录不存在或缺少文件，LSP 支持将被禁用。确保在克隆项目时使用 `--recursive` 选项：

```bash
git clone --recursive https://github.com/your-repo/pnana.git
```

如果已经克隆了项目，可以运行：

```bash
git submodule update --init --recursive
```

### Tree-sitter 语法高亮

#### Tree-sitter

**版本要求**: 0.20.0 或更高版本

**说明**: Tree-sitter 是一个增量解析系统，用于提供高质量的语法高亮。如果未安装 Tree-sitter，pnana 将使用内置的原生语法高亮器，功能仍可正常使用，但语法高亮质量可能略低。

**安装方法**:

##### Ubuntu/Debian
```bash
# 安装 Tree-sitter 核心库
sudo apt install libtree-sitter-dev

# 安装语言库（可选，但推荐）
sudo apt install libtree-sitter-cpp-dev \
                 libtree-sitter-python-dev \
                 libtree-sitter-javascript-dev \
                 libtree-sitter-json-dev \
                 libtree-sitter-markdown-dev \
                 libtree-sitter-bash-dev
```

##### 从源码安装（推荐）

如果包管理器中没有 Tree-sitter，可以从源码安装：

```bash
# 1. 安装 Tree-sitter 核心库
git clone https://github.com/tree-sitter/tree-sitter.git
cd tree-sitter
make
sudo make install

# 2. 安装语言库（以 C++ 为例）
git clone https://github.com/tree-sitter/tree-sitter-cpp.git
cd tree-sitter-cpp
make
sudo make install

# 其他常用语言库：
# - tree-sitter-python: https://github.com/tree-sitter/tree-sitter-python
# - tree-sitter-javascript: https://github.com/tree-sitter/tree-sitter-javascript
# - tree-sitter-typescript: https://github.com/tree-sitter/tree-sitter-typescript
# - tree-sitter-json: https://github.com/tree-sitter/tree-sitter-json
# - tree-sitter-markdown: https://github.com/tree-sitter/tree-sitter-markdown
# - tree-sitter-bash: https://github.com/tree-sitter/tree-sitter-bash
```

**验证安装**:
```bash
pkg-config --modversion tree-sitter
```

**注意**: 安装 Tree-sitter 后，需要重新配置 CMake：
```bash
rm -rf build
mkdir build && cd build
cmake ..
```

**支持的语言**: Tree-sitter 支持 40+ 种编程语言。pnana 默认支持以下语言：
- C/C++ (cpp, c, h, hpp)
- Python (py, pyw, pyi)
- JavaScript/TypeScript (js, jsx, ts, tsx)
- JSON (json, jsonc)
- Markdown (md, markdown)
- Shell/Bash (sh, bash, zsh)
- Rust (rs)
- Go (go)
- Java (java)

如果安装了对应的语言库，pnana 会自动使用 Tree-sitter 进行语法高亮。否则会回退到内置的原生语法高亮器。

### Lua 插件系统

#### Lua

**版本要求**: Lua 5.3 或 5.4

**说明**: Lua 用于插件系统，允许用户编写 Lua 脚本来扩展编辑器功能。如果未安装 Lua，插件系统将被禁用，但编辑器核心功能仍可正常使用。

**安装方法**:

##### Ubuntu/Debian
```bash
# Lua 5.4 (推荐)
sudo apt install liblua5.4-dev

# 或 Lua 5.3
sudo apt install liblua5.3-dev
```

##### Fedora/RHEL
```bash
# Lua 5.4
sudo dnf install lua-devel

# 或指定版本
sudo dnf install lua5.4-devel
```

##### macOS
```bash
brew install lua
```

##### 从源码安装

```bash
# 下载 Lua 5.4
wget https://www.lua.org/ftp/lua-5.4.6.tar.gz
tar -xzf lua-5.4.6.tar.gz
cd lua-5.4.6
make linux  # 或 make macosx
sudo make install
```

**验证安装**:
```bash
lua -v
pkg-config --modversion lua5.4  # 或 lua5.3
```

**注意**: 安装 Lua 后，需要重新配置 CMake：

```bash
rm -rf build
mkdir build && cd build
cmake ..
```

### Go 编译器

**版本要求**: Go 1.21 或更高版本

**说明**: Go 编译器用于构建 SSH 客户端模块。如果未安装 Go，pnana 仍可正常编译和运行，但 SSH 功能将使用系统命令作为后备方案。

**安装方法**:

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install golang-go
```

#### Fedora/RHEL
```bash
sudo dnf install golang
```

#### macOS
```bash
brew install go
```

#### 从官方安装（推荐）

访问 [Go 官网](https://golang.org/dl/) 下载最新版本。

**验证安装**:
```bash
go version
```

### Go 模块依赖

如果使用 Go SSH 模块，以下 Go 包会被自动下载：

- `golang.org/x/crypto` (v0.17.0) - SSH 加密支持
- `golang.org/x/sys` (v0.15.0) - 系统调用支持

这些依赖会在编译时通过 `go mod download` 自动获取，无需手动安装。

---

## 系统库依赖

以下系统库在链接时自动链接，通常已包含在标准 C++ 运行时中：

- **pthread**: POSIX 线程库（用于多线程支持）
- **dl**: 动态链接库（用于 Go 模块的动态加载）

这些库在大多数 Linux 系统中默认可用，无需额外安装。

---

## 安装指南

### 快速检查依赖

pnana 项目包含一个构建脚本 `build.sh`，它会自动检查所有必需依赖：

```bash
./build.sh
```

如果缺少依赖，脚本会提示需要安装的包。

### 完整安装步骤

#### 1. 安装构建工具

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install cmake build-essential g++

# Fedora/RHEL
sudo dnf install cmake gcc-c++

# macOS
brew install cmake
xcode-select --install
```

#### 2. 安装 FTXUI

```bash
# Ubuntu/Debian
sudo apt install libftxui-dev

# Fedora/RHEL
sudo dnf install ftxui-devel

# macOS
brew install ftxui
```

#### 3. （可选）安装 LSP 支持依赖

```bash
# Ubuntu/Debian
sudo apt install nlohmann-json3-dev

# Fedora/RHEL
sudo dnf install nlohmann_json-devel

# macOS
brew install nlohmann-json
```

**注意**: jsonrpccxx 已包含在项目中，无需单独安装。如果使用 git 克隆项目，请确保包含子模块：

```bash
git submodule update --init --recursive
```

#### 4. （可选）安装 Tree-sitter 语法高亮

```bash
# Ubuntu/Debian
sudo apt install libtree-sitter-dev

# 或从源码安装（推荐）
git clone https://github.com/tree-sitter/tree-sitter.git
cd tree-sitter && make && sudo make install
```

#### 5. （可选）安装 Lua 插件系统

```bash
# Ubuntu/Debian
sudo apt install liblua5.4-dev

# Fedora/RHEL
sudo dnf install lua-devel

# macOS
brew install lua
```

#### 6. （可选）安装 Go

```bash
# Ubuntu/Debian
sudo apt install golang-go

# Fedora/RHEL
sudo dnf install golang

# macOS
brew install go
```

#### 7. 编译项目

```bash
./build.sh
```

---

## 依赖版本要求

| 依赖项 | 最低版本 | 推荐版本 | 必需/可选 |
|--------|----------|----------|-----------|
| CMake | 3.10 | 3.20+ | 必需 |
| C++ 编译器 | C++17 | 最新 | 必需 |
| GCC | 7.0 | 11.0+ | 必需 |
| Clang | 5.0 | 14.0+ | 必需 |
| FTXUI | 最新 | 最新 | 必需 |
| Tree-sitter | 0.20.0 | 0.20.0+ | 可选（语法高亮） |
| nlohmann/json | 3.10.0 | 3.11.0+ | 可选（LSP） |
| jsonrpccxx | 最新 | 最新 | 可选（LSP） |
| Lua | 5.3 | 5.4 | 可选（插件） |
| Go | 1.21 | 1.21+ | 可选（SSH） |

---

## 常见问题

### Q: 编译时提示找不到 FTXUI 怎么办？

**A**: 确保已安装 FTXUI 开发库。如果使用包管理器安装后仍无法找到，可能需要设置 `CMAKE_PREFIX_PATH`：

```bash
cmake .. -DCMAKE_PREFIX_PATH=/usr/local
```

### Q: 可以不安装 LSP 支持依赖吗？

**A**: 可以。pnana 可以在没有 nlohmann/json 和 jsonrpccxx 的情况下编译和运行。但 LSP 功能（代码补全、诊断等）将被禁用。编辑器核心功能（编辑、搜索、文件浏览等）仍可正常使用。

### Q: 可以不安装 Tree-sitter 吗？

**A**: 可以。pnana 可以在没有 Tree-sitter 的情况下编译和运行。编辑器会自动使用内置的原生语法高亮器，功能仍可正常使用，但语法高亮质量可能略低于 Tree-sitter。

### Q: 可以不安装 Lua 吗？

**A**: 可以。pnana 可以在没有 Lua 的情况下编译和运行。但插件系统将被禁用，无法使用 Lua 插件扩展功能。编辑器核心功能仍可正常使用。

### Q: 可以不安装 Go 吗？

**A**: 可以。pnana 可以在没有 Go 的情况下编译和运行。但 SSH 功能将使用系统命令（如 `ssh`）作为后备方案，功能可能受限。

### Q: 如何检查所有依赖是否已安装？

**A**: 运行构建脚本会自动检查：

```bash
./build.sh
```

或者手动检查：

```bash
cmake --version
g++ --version  # 或 clang++ --version
pkg-config --modversion ftxui  # 如果已安装
pkg-config --modversion nlohmann_json  # 如果已安装（LSP）
lua -v  # 如果已安装（插件系统）
go version  # 可选（SSH）
```

### Q: 如何检查 LSP 和插件系统是否已启用？

**A**: 运行 CMake 配置时，会显示功能启用状态：

```bash
cd build
cmake ..
```

查找以下输出：
- `✓ Tree-sitter found - syntax highlighting enabled` - Tree-sitter 语法高亮已启用
- `LSP support enabled` - LSP 功能已启用
- `✓ Lua found - plugin system enabled` - 插件系统已启用
- `Go SSH module will be built and linked` - Go SSH 模块已启用

### Q: jsonrpccxx 找不到怎么办？

**A**: jsonrpccxx 是项目的子模块，位于 `third-party/JSON-RPC-CXX`。如果找不到：

1. 确保使用 `--recursive` 克隆项目：
   ```bash
   git clone --recursive <repository-url>
   ```

2. 如果已克隆，更新子模块：
   ```bash
   git submodule update --init --recursive
   ```

3. 如果子模块目录存在但 CMake 仍找不到，检查 `third-party/JSON-RPC-CXX/jsonrpccxx/client.hpp` 是否存在。

### Q: Windows 上可以编译吗？

**A**: 理论上可以，但需要：
- Visual Studio 2017 或更高版本（支持 C++17）
- CMake 3.10+
- FTXUI（需要从源码编译或使用 vcpkg）
- nlohmann/json（可通过 vcpkg 安装）
- Lua（需要从源码编译或使用 vcpkg）

Windows 支持仍在测试中，建议使用 WSL2 或 Linux/macOS 环境。

### Q: 依赖冲突怎么办？

**A**: 如果遇到依赖版本冲突：

1. 确保使用推荐的版本
2. 清理构建目录：`rm -rf build`
3. 重新配置：`cmake ..`
4. 如果问题持续，考虑使用虚拟环境或容器

### Q: 如何更新依赖？

**A**: 

**系统包管理器安装的依赖**:
```bash
# Ubuntu/Debian
sudo apt update && sudo apt upgrade

# Fedora/RHEL
sudo dnf update

# macOS
brew update && brew upgrade
```

**从源码安装的依赖**:
需要重新从源码编译安装。

**Go 模块依赖**:
在项目根目录运行：
```bash
go mod tidy
go mod download
```

**jsonrpccxx (子模块)**:
如果子模块未正确初始化：
```bash
git submodule update --init --recursive
```

---

## 依赖关系图

```
pnana
├── 构建工具
│   ├── CMake (>= 3.10)
│   └── C++ 编译器 (C++17)
├── 必需库
│   └── FTXUI
└── 可选依赖
    ├── 语法高亮
    │   ├── Tree-sitter (>= 0.20.0) [推荐]
    │   │   ├── tree-sitter-cpp
    │   │   ├── tree-sitter-python
    │   │   ├── tree-sitter-javascript
    │   │   ├── tree-sitter-json
    │   │   └── ... (其他语言库)
    │   └── 原生语法高亮器 [内置，兼容模式]
    ├── LSP 支持
    │   ├── nlohmann/json (>= 3.10.0)
    │   └── jsonrpccxx (子模块)
    ├── 插件系统
    │   └── Lua (5.3 或 5.4)
    ├── SSH 模块
    │   ├── Go (>= 1.21)
    │   │   ├── golang.org/x/crypto
    │   │   └── golang.org/x/sys
    │   └── 系统库
    │       ├── pthread
    │       └── dl
    └── 系统库
        ├── pthread
        └── dl
```

---

## 更新日志

- **v1.1.0**: 更新依赖文档
  - 添加 Tree-sitter 语法高亮依赖
  - 说明 Tree-sitter 和原生语法高亮器的兼容模式
  - 更新安装指南和依赖关系图

- **v1.0.0**: 更新依赖文档
  - 添加 LSP 支持依赖（nlohmann/json 和 jsonrpccxx）
  - 添加 Lua 插件系统依赖
  - 更新安装指南和常见问题
  - 更新依赖关系图

- **v0.0.3**: 初始依赖文档
  - 添加 CMake 和 C++ 编译器要求
  - 添加 FTXUI 库依赖
  - 添加可选的 Go 依赖说明

---

**注意**: 本文档基于当前版本的依赖要求。如有更新，请参考最新代码和 CMakeLists.txt 文件。

