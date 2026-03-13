# 代码格式化指南

本项目提供了自动代码格式化工具，确保代码风格一致。

## 工具说明

### 1. Bash脚本 (`format-code.sh`)
- 适用于Linux/macOS系统
- 支持格式化C++文件和CMake文件
- 提供检查模式

### 2. Python脚本 (`format_code.py`)
- 跨平台支持（Windows/Linux/macOS）
- 功能更全面
- 自动创建配置文件

### 3. 配置文件 (`.clang-format`)
- 基于LLVM风格
- 4空格缩进
- 120字符行宽限制
- 符合项目现有代码风格

## 安装依赖

### macOS
```bash
# 安装clang-format
brew install clang-format

# 安装cmake-format (可选)
pip install cmake-format
```

### Linux (Ubuntu/Debian)
```bash
# 安装clang-format
sudo apt-get install clang-format

# 安装cmake-format (可选)
pip install cmake-format
```

### Windows
```bash
# 使用Chocolatey安装
choco install llvm

# 或从LLVM官网下载安装包
# 然后安装Python和cmake-format
pip install cmake-format
```

## 使用方法

### 使用Bash脚本
```bash
# 给予执行权限
chmod +x format-code.sh

# 格式化所有C++文件
./format-code.sh --format

# 检查代码格式
./format-code.sh --check

# 格式化所有文件类型
./format-code.sh --all

# 显示帮助
./format-code.sh --help
```

### 使用Python脚本
```bash
# 格式化所有C++文件
python format_code.py --format

# 检查代码格式
python format_code.py --check

# 格式化所有文件类型
python format_code.py --all

# 创建配置文件
python format_code.py --create-config

# 显示帮助
python format_code.py --help
```

## 代码风格规范

### 缩进
- 使用4个空格，不使用Tab
- 类、函数、控制语句使用一致的缩进

### 大括号
- 使用K&R风格（开括号不换行）
```cpp
void function() {
    // 代码
}

if (condition) {
    // 代码
}
```

### 命名约定
- 类名：PascalCase（如 `ConfigManager`）
- 函数名：camelCase（如 `initialize()`）
- 变量名：camelCase（如 `m_cameraCapture`）
- 常量：UPPER_SNAKE_CASE（如 `MAX_FRAME_RATE`）

### 行宽限制
- 最大120字符
- 超过时自动换行

### 头文件包含顺序
1. 系统头文件（C/C++标准库）
2. Qt框架头文件
3. FFmpeg头文件（使用 `extern "C"` 包装）
4. 项目头文件（相对路径）

## 集成到开发流程

### 预提交钩子（Git Hook）
在 `.git/hooks/pre-commit` 中添加：
```bash
#!/bin/bash
python format_code.py --check
if [ $? -ne 0 ]; then
    echo "代码格式检查失败，请运行: python format_code.py --format"
    exit 1
fi
```

### IDE集成
#### VS Code
安装 "Clang-Format" 扩展，在设置中添加：
```json
{
    "C_Cpp.clang_format_path": "clang-format",
    "editor.formatOnSave": true
}
```

#### CLion
在设置中启用 "ClangFormat as formatter"，选择 "Use .clang-format file"。

#### Qt Creator
安装 "Beautifier" 插件，配置使用 clang-format。

## 常见问题

### 1. 命令未找到
确保已安装 clang-format，并添加到系统PATH。

### 2. 格式化后代码不匹配
检查 `.clang-format` 配置文件是否正确，或运行 `python format_code.py --create-config` 重新创建。

### 3. 某些文件未被格式化
确保文件扩展名正确（.cpp, .h, .hpp），且不在排除目录中。

### 4. Windows系统问题
在Windows上建议使用Python脚本，或安装Git Bash来运行Bash脚本。

## 手动格式化命令

如果不想使用脚本，可以直接运行：
```bash
# 格式化所有C++文件
find src -name "*.cpp" -o -name "*.h" | xargs clang-format -i

# 检查格式
find src -name "*.cpp" -o -name "*.h" | xargs clang-format --dry-run -Werror
```

## 贡献指南

提交代码前请确保：
1. 代码已通过格式化检查
2. 没有语法错误
3. 遵循项目代码风格

使用 `./format-code.sh --check` 或 `python format_code.py --check` 进行验证。