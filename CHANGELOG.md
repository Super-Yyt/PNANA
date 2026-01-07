# Changelog

This document records all important changes to the pnana project.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Added GitHub Actions workflows for automated building, testing, and releasing
- Added RELEASE.md template for generating standardized release notes

### Improved
- Optimized release process to support multiple package formats
- Added code quality checks and security scanning

## [0.0.4] - 2025-01-07

### Added

- 集成lsp系统，允许用户进行代码提示、代码诊断、代码格式化
- 增加光标样式配置修改功能
- 初步完成插件系统，允许用户使用过程中进行插件的启用与关闭
- 集成ffmpeg完成图片转ASCII码并进行预览

### Improved
- 增加更多语言的语法高亮（tree-sitter/内置的语法高亮器）
- 提供更多的内置主题进行切换
- 优化在线在线终端的主题样式与功能，解析命令效果优化
- 优化UI效果，提高界面UI与icon的统一度
- 优化文件编辑、浏览效果
- 优化go编写的ssh模块（未测试）

### Fixed

- 改进构建配置，允许用户手动进行feature功能的配置，减少必要依赖
- 修复配置系统，允许用户进行更新配置
- 完善编译构建相关文档，提高用户入手速度

详细上手文档参考 [QUICKSTART](QUICKSTART.md)


## [1.0.0] - 2023-XX-XX

### Added
- Modern terminal text editor based on FTXUI
- 28 built-in themes
- Three-column layout design
- Intuitive shortcut key system
- Multi-file support
- Syntax highlighting
- LSP support
- Lua plugin system
- SSH remote file editing
- Built-in terminal
- Image preview functionality
- Binary file viewer
- Search and replace functionality
- Split view
- Command palette
- File browser
- Encoding conversion support

### Improved
- Optimized memory usage
- Improved large file loading speed
- Enhanced user interface responsiveness

### Fixed
- Fixed display issues in certain terminals
- Fixed syntax highlighting problems for specific file formats

## [0.9.0] - 2023-XX-XX

### Added
- Initial version features
- Basic text editing functionality
- Simple theme system

---

## Version Notes

- **Major version**: Incompatible API changes
- **Minor version**: Backward-compatible functionality additions
- **Patch version**: Backward-compatible bug fixes
