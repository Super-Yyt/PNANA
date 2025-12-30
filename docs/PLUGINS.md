# pnana 插件系统与 LSP 文档

pnana 提供了两种扩展方式：**Lua 插件系统**和 **LSP (Language Server Protocol) 支持**。

## 目录

- [LSP 支持](#lsp-支持)
  - [什么是 LSP](#什么是-lsp)
  - [支持的 LSP 服务器](#支持的-lsp-服务器)
  - [LSP 功能](#lsp-功能)
  - [配置 LSP](#配置-lsp)
- [Lua 插件系统](#lua-插件系统)
  - [快速开始](#快速开始)
  - [插件结构](#插件结构)
  - [API 参考](#api-参考)
  - [事件系统](#事件系统)
  - [命令注册](#命令注册)
  - [键位映射](#键位映射)
  - [示例插件](#示例插件)

---

## LSP 支持

### 什么是 LSP

LSP (Language Server Protocol) 是由 Microsoft 开发的开放标准协议，允许编辑器与语言服务器通信，提供代码补全、语法检查、代码导航等智能功能。

pnana 内置了完整的 LSP 客户端支持，可以自动检测并连接各种语言的 LSP 服务器。

### 支持的 LSP 服务器

pnana 支持所有符合 LSP 标准的语言服务器，包括但不限于：

- **C/C++**: clangd
- **Python**: pyright, pylsp, jedi-language-server
- **JavaScript/TypeScript**: typescript-language-server
- **Java**: eclipse.jdt.ls
- **Go**: gopls
- **Rust**: rust-analyzer
- **HTML/CSS**: vscode-html-languageserver, vscode-css-languageserver
- **JSON**: vscode-json-languageserver
- **YAML**: yaml-language-server
- **Markdown**: markdown-language-server

### LSP 功能

pnana 的 LSP 支持提供以下功能：

#### 1. 代码补全 (Code Completion)
- 智能代码补全，根据上下文提供建议
- 支持函数、变量、类、方法等符号补全
- 实时响应，输入时自动触发
- 支持使用上下键浏览补全项

#### 2. 实时诊断 (Diagnostics)
- 语法错误实时显示
- 警告信息提示
- 错误位置高亮
- 悬停显示错误详情

#### 3. 代码导航 (Code Navigation)
- **跳转到定义** (Go to Definition): 快速跳转到符号定义位置
- **查找引用** (Find References): 查找符号的所有引用
- **符号搜索** (Symbol Search): 快速查找文件中的符号

#### 4. 悬停信息 (Hover)
- 鼠标悬停显示符号信息
- 显示函数签名、类型信息、文档等

### 配置 LSP

#### 自动配置

pnana 会自动检测系统中已安装的 LSP 服务器，无需手动配置即可使用。

#### 手动配置

如果需要自定义 LSP 服务器配置，可以在配置文件中设置：

```json
{
  "lsp": {
    "servers": {
      "python": {
        "command": "pylsp",
        "args": ["--stdio"]
      },
      "cpp": {
        "command": "clangd",
        "args": []
      }
    }
  }
}
```

#### 安装 LSP 服务器

**Python (pylsp)**:
```bash
pip install python-lsp-server
```

**C/C++ (clangd)**:
```bash
# Ubuntu/Debian
sudo apt install clangd

# macOS
brew install llvm
```

**JavaScript/TypeScript**:
```bash
npm install -g typescript-language-server
```

**Go (gopls)**:
```bash
go install golang.org/x/tools/gopls@latest
```

**Rust (rust-analyzer)**:
```bash
# 通过 rustup 安装
rustup component add rust-analyzer
```

### LSP 使用技巧

1. **代码补全触发**：输入 2-3 个字符后自动触发，或手动按 `Ctrl+Space`
2. **浏览补全项**：使用上下键在补全列表中导航
3. **接受补全**：按 `Enter` 或 `Tab` 接受当前补全项
4. **跳转到定义**：将光标放在符号上，按 `F12` 或右键菜单选择
5. **查看错误**：错误会在状态栏显示，悬停查看详情

---

## Lua 插件系统

## 快速开始

### 创建插件

1. 在插件目录创建插件文件夹：
   ```bash
   mkdir -p ~/.config/pnana/plugins/my-plugin
   ```

2. 创建 `init.lua` 文件：
   ```lua
   -- 插件信息
   plugin_name = "my-plugin"
   plugin_version = "1.0.0"
   plugin_description = "我的第一个插件"
   plugin_author = "Your Name"
   
   -- 插件代码
   print("Hello from my-plugin!")
   ```

3. 重启 pnana，插件会自动加载

### 插件目录

pnana 会按以下顺序查找插件目录：

1. `~/.config/pnana/plugins`
2. `./plugins`
3. `./lua`
4. `./.pnana/plugins`

## 插件结构

```
my-plugin/
├── init.lua          # 插件入口文件（必需）
├── plugin.lua        # 插件配置文件（可选）
└── lua/              # Lua 模块目录（可选）
    └── my_module.lua
```

## API 参考

### vim.api

编辑器核心 API，提供对编辑器功能的访问。

#### 文档操作

```lua
-- 获取当前行号（从 0 开始）
local line = vim.api.get_current_line()

-- 获取总行数
local count = vim.api.get_line_count()

-- 获取光标位置
local pos = vim.api.get_cursor_pos()
-- 返回: {row = 0, col = 0}

-- 设置光标位置
vim.api.set_cursor_pos({row = 10, col = 5})

-- 获取指定行
local line = vim.api.get_line(5)  -- 获取第 6 行（索引从 0 开始）

-- 设置指定行
vim.api.set_line(5, "new content")

-- 插入文本
vim.api.insert_text(5, 10, "inserted text")

-- 删除行
vim.api.delete_line(5)
```

#### 文件操作

```lua
-- 获取当前文件路径
local filepath = vim.api.get_filepath()

-- 打开文件
vim.api.open_file("/path/to/file.txt")

-- 保存文件
vim.api.save_file()
```

#### 主题操作

```lua
-- 获取当前主题
local theme = vim.api.get_theme()

-- 设置主题
vim.api.set_theme("monokai")
```

#### 状态消息

```lua
-- 设置状态栏消息
vim.api.set_status_message("Hello from plugin!")
```

### vim.fn

工具函数集合。

```lua
-- 执行系统命令
local output = vim.fn.system("ls -la")

-- 读取文件
local lines = vim.fn.readfile("/path/to/file.txt")
-- 返回: {"line1", "line2", ...}

-- 写入文件
vim.fn.writefile("/path/to/file.txt", {"line1", "line2"})
```

## 事件系统

使用 `vim.autocmd` 或 `pnana_autocmd` 注册事件监听器。

### 可用事件

- `BufEnter` - 进入缓冲区
- `BufLeave` - 离开缓冲区
- `BufWrite` - 写入缓冲区
- `FileOpened` - 文件打开
- `FileSaved` - 文件保存
- `CursorMoved` - 光标移动
- `TextChanged` - 文本改变

### 示例

```lua
-- 监听文件打开事件
vim.autocmd("FileOpened", function(filepath)
    print("File opened: " .. filepath)
end)

-- 监听文件保存事件
vim.autocmd("FileSaved", function(filepath)
    print("File saved: " .. filepath)
end)
```

## 命令注册

使用 `vim.cmd` 或 `pnana_command` 注册自定义命令。

```lua
-- 注册命令
vim.cmd("MyCommand", function()
    vim.api.set_status_message("MyCommand executed!")
end)

-- 命令可以通过命令面板调用
```

## 键位映射

使用 `vim.keymap` 或 `pnana_keymap` 注册键位映射。

```lua
-- 在 normal 模式下映射 Ctrl+P 到自定义函数
vim.keymap("n", "<C-p>", function()
    vim.api.set_status_message("Ctrl+P pressed!")
end)

-- 模式说明：
-- "n" - normal 模式
-- "i" - insert 模式
-- "v" - visual 模式
```

## 示例插件

### 示例 1: 简单通知插件

```lua
-- init.lua
plugin_name = "hello-plugin"
plugin_version = "1.0.0"
plugin_description = "简单的问候插件"

vim.api.set_status_message("Hello Plugin loaded!")
```

### 示例 2: 自动保存插件

```lua
-- init.lua
plugin_name = "auto-save"
plugin_version = "1.0.0"
plugin_description = "自动保存文件"

-- 监听文本改变事件，延迟保存
local save_timer = nil

vim.autocmd("TextChanged", function()
    -- 取消之前的定时器
    if save_timer then
        -- 这里简化处理，实际应该使用定时器
    end
    
    -- 延迟 2 秒后保存
    -- 注意：pnana 目前不提供定时器 API，这里仅作示例
    vim.api.set_status_message("Text changed, will auto-save...")
end)
```

### 示例 3: 自定义命令插件

```lua
-- init.lua
plugin_name = "custom-commands"
plugin_version = "1.0.0"
plugin_description = "自定义命令集合"

-- 注册命令：显示当前时间
vim.cmd("ShowTime", function()
    local time = os.date("%Y-%m-%d %H:%M:%S")
    vim.api.set_status_message("Current time: " .. time)
end)

-- 注册命令：插入当前日期
vim.cmd("InsertDate", function()
    local date = os.date("%Y-%m-%d")
    local pos = vim.api.get_cursor_pos()
    vim.api.insert_text(pos.row, pos.col, date)
end)
```

### 示例 4: 键位映射插件

```lua
-- init.lua
plugin_name = "custom-keymaps"
plugin_version = "1.0.0"
plugin_description = "自定义键位映射"

-- 映射 Ctrl+S 为保存（虽然默认已有，这里作为示例）
vim.keymap("n", "<C-s>", function()
    vim.api.save_file()
    vim.api.set_status_message("File saved!")
end)

-- 映射 F5 为运行当前文件（如果是脚本）
vim.keymap("n", "<F5>", function()
    local filepath = vim.api.get_filepath()
    if filepath then
        local ext = filepath:match("%.([^%.]+)$")
        if ext == "py" then
            vim.fn.system("python " .. filepath)
        elseif ext == "sh" then
            vim.fn.system("bash " .. filepath)
        end
    end
end)
```

## 最佳实践

1. **插件命名**：使用小写字母和连字符，如 `my-awesome-plugin`
2. **版本管理**：遵循语义化版本（SemVer）
3. **错误处理**：使用 `pcall` 包装可能出错的代码
4. **性能优化**：避免在事件回调中执行耗时操作
5. **文档**：为插件编写清晰的 README

## 调试

插件错误会记录到 `pnana.log` 文件中。查看日志：

```bash
tail -f pnana.log
```

## 更多资源

- [Lua 官方文档](https://www.lua.org/manual/5.4/)
- [Neovim Lua 指南](https://github.com/nanotee/nvim-lua-guide)（参考）

