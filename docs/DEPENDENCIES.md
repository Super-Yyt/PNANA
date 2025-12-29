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

#### 3. （可选）安装 Go

```bash
# Ubuntu/Debian
sudo apt install golang-go

# Fedora/RHEL
sudo dnf install golang

# macOS
brew install go
```

#### 4. 编译项目

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
| Go | 1.21 | 1.21+ | 可选 |

---

## 常见问题

### Q: 编译时提示找不到 FTXUI 怎么办？

**A**: 确保已安装 FTXUI 开发库。如果使用包管理器安装后仍无法找到，可能需要设置 `CMAKE_PREFIX_PATH`：

```bash
cmake .. -DCMAKE_PREFIX_PATH=/usr/local
```

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
go version  # 可选
```

### Q: Windows 上可以编译吗？

**A**: 理论上可以，但需要：
- Visual Studio 2017 或更高版本（支持 C++17）
- CMake 3.10+
- FTXUI（需要从源码编译或使用 vcpkg）

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
    ├── Go (>= 1.21)
    │   ├── golang.org/x/crypto
    │   └── golang.org/x/sys
    └── 系统库
        ├── pthread
        └── dl
```

---

## 更新日志

- **v0.0.3**: 初始依赖文档
  - 添加 CMake 和 C++ 编译器要求
  - 添加 FTXUI 库依赖
  - 添加可选的 Go 依赖说明

---

**注意**: 本文档基于当前版本的依赖要求。如有更新，请参考最新代码和 CMakeLists.txt 文件。

