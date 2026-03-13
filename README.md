# Screen Streamer

基于 Qt 6.10 + FFmpeg 6.x 的高性能屏幕捕获与推流工具，支持 SRT/RTMP 协议推流和本地录像。

## 功能特性

- **高性能屏幕捕获**：基于 Qt 6.10 QScreenCapture，支持 60 FPS 稳定捕获
- **摄像头画中画**：支持摄像头画面叠加到屏幕捕获画面
- **智能区域选择**：通过 +/- 按钮快速切换捕获区域（1/4屏、1/2屏、全屏）
- **硬件编码加速**：自动检测并优先使用硬件编码器（NVENC/AMF/QSV）
- **低延迟推流**：优先支持 SRT 协议，兼容 RTMP 协议
- **本地录像**：支持 MP4 格式本地存储
- **配置持久化**：自动保存用户设置

## 系统要求

- **操作系统**：Windows 10/11 (64位)
- **Qt 版本**：6.10.0 或更高
- **FFmpeg**：6.x LTS 版本
- **编译器**：MinGW 13.2.0 (与 Qt 6.10 兼容)

## 快速开始

### 1. 环境搭建

```bash
# 安装 Qt 6.10 (选择 MinGW 组件)
# 下载地址: https://www.qt.io/download

# 安装 FFmpeg 6.x LTS
# 下载地址: https://www.gyan.dev/ffmpeg/builds/

# 设置环境变量
export FFMPEG_DIR="C:\ffmpeg"  # 根据实际安装路径调整
```

### 2. 构建项目

```bash
# 克隆项目
git clone <repository-url>
cd screen-streamer

# 构建
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.10.0/mingw_64"
cmake --build . --config Release
```

### 3. 运行程序

```bash
# 在构建目录中
./ScreenStreamer
```

## 项目结构

```
screen-streamer/
├── CMakeLists.txt                 # 主CMake配置
├── cmake/
│   └── FindFFmpeg.cmake          # FFmpeg查找脚本
├── src/
│   ├── main.cpp                  # 程序入口
│   ├── core/                     # 核心功能模块
│   │   ├── capture/              # 捕获模块
│   │   ├── encoder/              # 编码模块
│   │   ├── streamer/             # 推流模块
│   │   ├── recorder/             # 录像模块
│   │   └── config/               # 配置管理
│   ├── ui/                       # 界面模块
│   └── utils/                    # 工具类
├── resources/                    # 资源文件
├── tests/                        # 单元测试
└── deploy/                       # 部署脚本
```

## 使用说明

### 主工具栏

工具栏固定在屏幕中央顶部，包含以下控件：

1. **区域控制**：显示当前捕获区域，使用 +/- 按钮调整
2. **摄像头控制**：摄像头开关和选择
3. **编码设置**：码率和 GOP 设置
4. **协议选择**：SRT/RTMP 切换
5. **推流地址**：输入推流服务器地址
6. **录像开关**：控制本地录像
7. **开始/停止**：控制推流状态

### 捕获区域

支持三种捕获模式：
- **1/4屏**：屏幕宽高的 1/2，居中显示
- **1/2屏**：屏幕宽高的 1/√2，居中显示
- **全屏**：捕获整个屏幕

### 推流设置

#### SRT 协议
```
srt://<ip>:<port>?mode=caller&latency=200000
```

#### RTMP 协议
```
rtmp://<server>/<app>/<stream_key>
```

### 硬件编码器支持

程序会自动检测并优先使用以下硬件编码器：
- **NVIDIA**：h264_nvenc
- **AMD**：h264_amf
- **Intel**：h264_qsv
- **软件备用**：libx264

## 开发指南

### 添加新功能

1. 在 `src/core/` 下创建相应的模块
2. 更新 `CMakeLists.txt` 添加源文件
3. 在 `src/main.cpp` 中集成新模块
4. 更新 UI 界面（如果需要）

### 测试

```bash
# 运行单元测试
cd build
ctest --output-on-failure
```

### 打包发布

```bash
# 部署 Qt 依赖
./deploy/windeployqt.ps1

# 使用 InnoSetup 创建安装包
# 运行 deploy/InnoSetup/installer.iss
```

## 配置说明

配置文件保存在用户目录：
- **Windows**: `%APPDATA%/ScreenStreamer/config.json`
- **Linux/macOS**: `~/.config/ScreenStreamer/config.json`

配置项包括：
- 捕获区域设置
- 摄像头选择
- 编码参数
- 推流地址
- 录像设置

## 故障排除

### 常见问题

1. **无法找到 FFmpeg 库**
   - 设置 `FFMPEG_DIR` 环境变量
   - 确保 FFmpeg 版本为 6.x

2. **摄像头无法识别**
   - 检查摄像头权限
   - 确保 Qt Multimedia 模块正确安装

3. **推流失败**
   - 检查网络连接
   - 验证推流地址格式
   - 查看防火墙设置

4. **性能问题**
   - 降低捕获分辨率
   - 使用硬件编码器
   - 调整码率设置

### 日志查看

程序日志保存在：
- **Windows**: `%APPDATA%/ScreenStreamer/logs/`
- **Linux/macOS**: `~/.local/share/ScreenStreamer/logs/`

## 许可证

本项目采用 MIT 许可证。详见 [LICENSE](LICENSE) 文件。

## 贡献指南

欢迎提交 Issue 和 Pull Request！

1. Fork 项目
2. 创建功能分支
3. 提交更改
4. 推送到分支
5. 创建 Pull Request

## 联系方式

- 项目主页：<repository-url>
- 问题反馈：<issues-url>
- 讨论区：<discussions-url>