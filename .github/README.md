# GitHub Actions 工作流

本项目使用 GitHub Actions 进行自动化构建、测试、文档生成和发布。以下是各个工作流的说明：

## 工作流概览

### 1. Build and Test (`.github/workflows/build.yml`)

此工作流负责项目的构建和测试，确保代码质量。

**触发条件：**
- 推送到 `master` 或 `develop` 分支
- 创建针对 `master` 或 `develop` 分支的 Pull Request

**执行内容：**
- 在 Ubuntu 20.04 和 Ubuntu 22.04 上构建项目
- 使用 Debug 和 Release 两种构建模式
- 运行单元测试
- 代码格式检查（使用 clang-format）
- 静态代码分析（使用 cppcheck）
- 安全漏洞扫描（使用 Trivy）

### 2. Release and Package (`.github/workflows/release.yml`)

此工作流负责项目的发布和打包。

**触发条件：**
- 推送以 `v` 开头的标签（如 `v1.0.0`）
- 手动触发（workflow_dispatch）

**执行内容：**
- 在多个 Ubuntu 版本上构建项目
- 创建多种格式的安装包：
  - `.tar.gz` 压缩包（包含安装脚本）
  - `.deb` Debian 包
- 自动创建 GitHub Release
- 上传构建产物到 Release 页面

### 3. Documentation (`.github/workflows/docs.yml`)

此工作流负责项目文档的构建和部署。

**触发条件：**
- 推送到 `master` 分支（当文档文件有变更时）
- 创建针对 `master` 分支的 Pull Request（当文档文件有变更时）

**执行内容：**
- 使用 Doxygen 生成 API 文档
- 部署文档到 GitHub Pages
- 上传文档构建产物

## 如何使用

### 创建发布版本

1. 确保代码已合并到 `master` 分支
2. 创建并推送一个新标签：
   ```bash
   git tag v1.0.0
   git push origin v1.0.0
   ```
3. GitHub Actions 将自动构建、打包并创建 Release

### 手动触发发布

1. 访问 GitHub 仓库的 Actions 页面
2. 选择 "Release and Package" 工作流
3. 点击 "Run workflow" 按钮

### 查看构建状态

1. 访问 GitHub 仓库的 Actions 页面
2. 选择相应的工作流查看运行状态和日志

## 安装包说明

### .tar.gz 包安装

```bash
# 下载并解压
wget https://github.com/用户名/pnana/releases/download/v1.0.0/pnana-ubuntu22.04.tar.gz
tar -xzf pnana-ubuntu22.04.tar.gz

# 运行安装脚本
cd package
sudo ./install.sh
```

### .deb 包安装（推荐）

```bash
# 下载并安装
wget https://github.com/用户名/pnana/releases/download/v1.0.0/pnana_v1.0.0_amd64.deb
sudo dpkg -i pnana_v1.0.0_amd64.deb

# 如果有依赖问题，运行
sudo apt-get install -f
```

## 自定义工作流

如需自定义工作流，可以编辑 `.github/workflows/` 目录下的相应文件：

- `build.yml` - 构建和测试配置
- `release.yml` - 发布和打包配置
- `docs.yml` - 文档构建配置

## 发布说明管理

项目使用 `RELEASE.md` 模板来管理发布说明。在创建新版本时，GitHub Actions 会自动使用此模板生成发布说明。

### 本地生成发布说明

您可以在本地使用以下命令生成特定版本的发布说明：

```bash
# 生成版本 v1.0.0 的发布说明
./scripts/generate_release_notes.sh v1.0.0

# 生成版本 v1.0.0 的发布说明，并指定上一个版本为 v0.9.0
./scripts/generate_release_notes.sh v1.0.0 v0.9.0
```

### 自定义发布说明

1. 编辑 `RELEASE.md` 模板文件，添加通用的发布说明结构
2. 对于每个新版本，可以：
   - 使用脚本生成基础发布说明，然后手动编辑
   - 或者直接编辑生成的发布说明文件

### 发布说明内容

发布说明应包含：
- 版本号和发布日期
- 新增功能
- 改进
- 修复的问题
- 已知问题（如果有）
- 安装说明
- 使用方法
- 配置说明
- 反馈与支持链接

## 注意事项

1. 确保 `build.sh` 脚本具有可执行权限
2. 发布前请确保所有测试通过
3. 更新版本号时，请同时更新 `CMakeLists.txt` 中的版本信息
4. 如需添加新的依赖，请同时更新工作流中的依赖安装步骤

## 故障排除

如果工作流运行失败，请检查：

1. Actions 页面中的错误日志
2. 依赖是否正确安装
3. 构建脚本是否有问题
4. 版本号格式是否正确（应为 `vX.Y.Z` 格式）