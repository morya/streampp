非常好！运行时检测确实是正确的选择，这样能确保程序在不同硬件环境下都能稳定工作。基于你的确认，我现在生成全部五个部分的提示词。

---

## 📋 第一部分：基础方案描述词

```
# 屏幕捕获与SRT/RTMP推流工具 - Qt/C++ 基础方案描述

## 项目概述
开发一个Windows平台下的高性能屏幕捕获与推流工具，基于Qt 6.10 + FFmpeg 6.x LTS技术栈。程序能够捕获屏幕指定区域，支持摄像头画面叠加（画中画），通过SRT协议（优先）或RTMP协议实时推送到网络服务，同时支持本地录像功能。

## 核心功能需求
1. **屏幕捕获**：基于Qt 6.10的QScreenCapture类，支持60 FPS稳定捕获
2. **摄像头捕获**：基于QCamera，支持多摄像头运行时动态检测和切换
3. **画面叠加**：支持摄像头画面叠加到屏幕捕获画面（画中画），位置大小可配置
4. **区域选择**：通过+/-按钮快速切换捕获区域大小（1、2、4对应1/4屏、1/2屏、全屏）
5. **实时预览**：使用红色虚线框标识当前捕获区域，窗口始终置顶
6. **编码器自动检测**：运行时检测可用硬件编码器（NVENC/AMF/QSV），自动fallback到libx264
7. **视频编码**：H.264编码，支持启动时设置码率，GOP可配置（默认2秒）
8. **网络推流**：优先支持SRT低延迟协议，同时兼容RTMP协议
9. **本地录像**：支持MP4格式本地存储，可独立于推流控制开关
10. **配置持久化**：在用户目录保存程序配置信息

## 目标用户
需要将屏幕内容（可叠加摄像头）实时推送到网络服务的专业用户、直播场景、远程协作、在线教育等

## 交付形态
基于CMake构建的Qt项目，通过MinGW编译，最终打包为单一的exe安装程序（InnoSetup），用户无需安装任何依赖即可使用
```

---

## 🏗️ 第二部分：基础架构选型指定

```
# 基础架构选型与版本指定

## 核心框架与语言
- **编程语言**：C++17 标准
- **Qt 版本**：Qt 6.10.0（最新稳定版）
- **编译器**：MinGW 13.2.0（或最新版本，与Qt 6.10兼容）
- **构建系统**：CMake 3.25+

## FFmpeg 集成（动态链接）
- **FFmpeg 版本**：6.x LTS（如6.1.2，稳定版）
- **集成方式**：动态链接（.dll文件），运行时加载
- **需包含组件**：
  - libavcodec (编码解码)
  - libavformat (封装格式)
  - libavutil (工具函数)
  - libavdevice (设备输入，可选)
  - libswscale (图像缩放转换)
- **需编译启用协议**：
  - libsrt (SRT协议支持)
  - librtmp (RTMP协议支持)

## 编码器支持策略
- **硬件编码器检测**：运行时调用FFmpeg API枚举可用编码器
- **目标编码器**：
  - NVIDIA NVENC (h264_nvenc)
  - AMD AMF (h264_amf)
  - Intel QSV (h264_qsv)
  - 备用软件编码器：libx264
- **检测机制**：程序启动时/编码器设置变更时重新检测

## 摄像头支持
- **Qt模块**：Qt Multimedia
- **检测机制**：QCameraInfo::availableCameras() 运行时枚举
- **多摄像头**：支持动态切换，UI实时刷新可用设备列表

## 项目目录结构
```
screen-streamer/
├── CMakeLists.txt                 # 主CMake配置
├── cmake/                         # CMake辅助模块
│   ├── FindFFmpeg.cmake           # FFmpeg查找脚本
│   └── Qt6Config.cmake             # Qt6配置
├── src/
│   ├── main.cpp                    # 程序入口
│   ├── core/                       # 核心功能模块
│   │   ├── capture/                 # 捕获模块
│   │   │   ├── ScreenCapture.cpp/h
│   │   │   ├── CameraCapture.cpp/h
│   │   │   └── VideoMixer.cpp/h     # 画面叠加
│   │   ├── encoder/                  # 编码模块
│   │   │   ├── EncoderManager.cpp/h  # 编码器管理/检测
│   │   │   └── H264Encoder.cpp/h
│   │   ├── streamer/                  # 推流模块
│   │   │   ├── SRTStreamer.cpp/h
│   │   │   └── RTMPStreamer.cpp/h
│   │   ├── recorder/                  # 录像模块
│   │   │   └── LocalRecorder.cpp/h
│   │   └── config/                    # 配置管理
│   │       └── ConfigManager.cpp/h
│   ├── ui/                          # 界面模块
│   │   ├── MainToolbar.cpp/h        # 主工具栏
│   │   ├── RegionOverlay.cpp/h      # 区域覆盖层
│   │   └── CameraSelector.cpp/h     # 摄像头选择控件
│   └── utils/                       # 工具类
│       ├── FFmpegUtils.cpp/h        # FFmpeg辅助函数
│       └── FpsCounter.cpp/h         # 帧率统计
├── resources/                       # 资源文件
│   ├── icons/                       # 图标
│   └── styles/                      # QSS样式表
├── third_party/                     # 第三方库（如需要）
├── tests/                           # 单元测试
├── deploy/                          # 部署脚本
│   ├── InnoSetup/                   # InnoSetup安装包脚本
│   │   └── installer.iss
│   └── windeployqt.ps1              # Qt部署脚本
└── README.md
```

## 依赖库清单
| 组件 | 作用 | 获取方式 |
|------|------|----------|
| Qt 6.10 | 基础框架 | Qt官方安装包/MaintenanceTool |
| FFmpeg 6.x | 编码推流 | 从gyan.dev或BtbN下载预编译包 |
| InnoSetup 6.x | 安装包制作 | 官网下载 |
| vcpkg/conan | 可选包管理 | 如需要 |

## 运行时检测流程
1. **启动时**：检测可用摄像头 → 更新UI下拉列表
2. **编码设置时**：枚举FFmpeg编码器 → 过滤出h264硬件编码器 → 生成可用列表
3. **推流启动前**：验证所选编码器是否可用，不可用则自动fallback
```

---

## 🎨 第三部分：GUI界面设计图稿出图指令

```
# Qt/C++ 版本 - GUI界面设计图稿出图指令

请根据以下描述生成软件界面的UI设计图（适用于Figma、Penpot或手绘风格）：

## 整体风格
简洁、专业、暗色主题（类似OBS的暗色模式），界面元素现代、清晰，适合长时间使用的生产力工具。所有控件需考虑Qt Widgets的实现可行性。

## 主工具栏设计（固定在屏幕中央顶部）

### 窗口规格
- **尺寸**：500x60 像素（水平布局，高度适中）
- **位置**：屏幕中央顶部（坐标：屏幕宽度/2 - 250, 20）
- **背景**：半透明磨砂玻璃效果（深色 #1E1E1E，透明度85%，带模糊效果）
- **边框**：1px 灰色细边框 (#3A3A3A)
- **圆角**：8px
- **始终置顶**：是
- **可拖动**：鼠标按住空白区域可拖动

### 控件布局（从左到右）

| 控件 | 类型 | 描述 |
|------|------|------|
| **区域指示器** | 标签 | 显示当前捕获区域，如"1/4屏 (960x540)"，字体白色12pt |
| **[-]** | 按钮 | 缩小捕获区域，蓝色背景悬停效果 |
| **[+]** | 按钮 | 放大捕获区域，蓝色背景悬停效果 |
| **分隔线1** | 垂直线 | 灰色 (#3A3A3A)，高度30px |
| **摄像头图标** | 按钮 | 摄像头开关，点击展开摄像头选择面板 |
| **摄像头名称** | 下拉框 | 显示当前选择的摄像头，点击列出所有可用设备 |
| **分隔线2** | 垂直线 | 灰色 (#3A3A3A)，高度30px |
| **码率** | 下拉框 | 预设：1Mbps、2Mbps、5Mbps、8Mbps、自定义 |
| **GOP设置** | 下拉框 | 预设：1秒、2秒（默认）、3秒、5秒 |
| **协议** | 切换按钮 | SRT/RTMP 二选一 |
| **地址输入框** | 输入框 | 宽度180px，占位文字："rtmp:// 或 srt://..." |
| **录像开关** | 切换按钮 | 独立控制是否本地录像（图标+文字"录像"） |
| **开始推流** | 按钮 | 绿色背景 (#0B6A4B)，点击后变为红色"停止推流" |

### 摄像头选择面板（点击摄像头图标展开）
- **位置**：工具栏下方，悬浮面板
- **尺寸**：300x150
- **内容**：
  - 摄像头列表（单选按钮）
  - "叠加画中画"复选框
  - 画中画位置预设：左上、右上、左下、右下（下拉框）
  - 画中画大小：小(25%)、中(50%)、原始(100%)

## 区域选择覆盖层设计

### 全屏覆盖窗口
- **类型**：透明QWidget，全屏，始终置顶
- **背景**：黑色半透明 (#000000, 60%透明度)，用于蒙层
- **捕获区域**：矩形窗口，内部完全透明（可见下方内容）
- **边框**：红色虚线框 (2px宽度，虚线样式：10px实线 + 5px空白)
- **区域尺寸**：
  - 模式1：1/4屏幕（宽度=屏幕宽/2，高度=屏幕高/2，居中）
  - 模式2：1/2屏幕（宽度=屏幕宽/√2，高度=屏幕高/√2，居中）
  - 模式3：全屏（铺满全屏）

### 实时状态显示
- **工具栏右侧**：推流时显示实时状态，如"▶ 60fps | 2.5Mbps | 录像中"
- **错误提示**：红色文字弹窗或工具栏下方临时提示

## 配色方案
- **主背景**：#1E1E1E (深灰)
- **文字**：#FFFFFF (白色主文字)，#CCCCCC (辅助文字)
- **强调色**：#0078D4 (蓝色，用于悬停、选中)
- **成功/开始**：#0B6A4B (绿色)
- **停止/错误**：#E81123 (红色)
- **边框**：#3A3A3A (灰色)
- **禁用状态**：#666666

## 图标资源
使用Qt内置的QStyle.StandardPixmap或Font Awesome 6的免费图标，确保在Qt中易于加载（.png或.svg）
```

---

## 🔨 第四部分：详细方案执行步骤设计

```
# Qt/C++/FFmpeg 详细执行步骤（拆分提示词）

## 步骤1：开发环境搭建与项目初始化
```
请帮我创建一个Qt/C++屏幕推流项目的完整开发环境配置和项目初始化。要求：

### 1. 环境要求
- Qt 6.10.0 (MinGW 13.2.0)
- CMake 3.25+
- FFmpeg 6.x LTS (从gyan.dev或BtbN下载预编译包)
- InnoSetup 6.x (用于最终打包)

### 2. 创建项目基础结构
使用CMake初始化项目，提供完整的CMakeLists.txt，需包含：
- 查找Qt6组件：Core, Widgets, Multimedia, Gui
- 查找FFmpeg库 (创建FindFFmpeg.cmake脚本，支持动态链接)
- 设置C++17标准
- 包含src/、resources/等子目录

### 3. 提供FFmpeg查找脚本
创建cmake/FindFFmpeg.cmake，能够通过环境变量FFMPEG_DIR或系统路径找到：
- avcodec
- avformat
- avutil
- swscale
- avdevice (可选)

### 4. 初始化Git仓库
- 创建.gitignore (包含build/、*.user、*.exe等)
- 创建README.md，包含项目简介、环境搭建步骤

### 5. 提供启动脚本
- build.bat/sh：一键构建脚本
- run.bat/sh：运行程序
- deploy.bat/sh：调用windeployqt部署依赖
```
[/step]

## 步骤2：配置管理模块实现
```
请实现程序的配置管理模块。要求：

### 类设计：ConfigManager (单例模式)
```cpp
class ConfigManager {
public:
    static ConfigManager& instance();
    
    bool load();  // 从文件加载
    bool save();  // 保存到文件
    
    // 配置项访问
    struct CaptureConfig {
        int regionMode;        // 1,2,4 对应1/4,1/2,全屏
        bool cameraEnabled;    // 摄像头开关
        std::string cameraId;  // 当前摄像头ID
        bool pipEnabled;       // 画中画开关
        int pipPosition;       // 0:左上,1:右上,2:左下,3:右下
        int pipSize;           // 0:小,1:中,2:原始
    };
    
    struct EncoderConfig {
        int bitrate;           // kbps
        int gopSeconds;        // GOP间隔(秒)
        std::string encoderName; // 当前选择的编码器
    };
    
    struct StreamConfig {
        std::string protocol;  // "srt" 或 "rtmp"
        std::string url;       // 推流地址
        bool recordLocally;    // 本地录像开关
    };
    
    // Getter/Setter...
};
```

### 配置文件格式
- JSON格式，保存在用户目录：%APPDATA%/ScreenStreamer/config.json
- 需包含所有配置项的默认值
- 使用Qt的QJsonDocument解析

### 功能要求
1. 程序启动时自动加载配置
2. 配置变更时自动保存（或提供save()接口）
3. 支持配置变更信号，UI可以连接更新
4. 异常处理：配置文件损坏时重置为默认值
```

## 步骤3：工具栏UI实现 (Qt Widgets)
```
请使用Qt Widgets实现主工具栏界面。要求：

### 类设计：MainToolbar (继承自QWidget)
```cpp
class MainToolbar : public QWidget {
    Q_OBJECT
public:
    explicit MainToolbar(QWidget* parent = nullptr);
    
signals:
    void regionModeChanged(int mode);              // 1,2,4
    void cameraToggled(bool enabled);
    void cameraSelected(const QString& id);
    void pipSettingsChanged(bool enabled, int position, int size);
    void bitrateChanged(int bitrate);
    void gopChanged(int seconds);
    void protocolChanged(const QString& protocol); // "srt"或"rtmp"
    void urlChanged(const QString& url);
    void recordToggled(bool enabled);
    void startStreamClicked();
    void stopStreamClicked();
    
public slots:
    void updateCameraList(const QStringList& cameras);
    void updateStreamingStatus(bool isStreaming, int fps, int bitrate);
    void showError(const QString& error);
```

### 实现要求
1. **无边框窗口拖动**：重写mousePressEvent/mouseMoveEvent
2. **半透明效果**：使用setAttribute(Qt::WA_TranslucentBackground) + QGraphicsBlurEffect或QPainter绘制
3. **控件具体实现**：
   - QLabel显示区域模式
   - QPushButton (+/-) 带图标
   - QComboBox显示摄像头列表（动态更新）
   - QComboBox码率预设（1,2,5,8,自定义输入）
   - QComboBox GOP预设（1,2,3,5秒）
   - QButtonGroup或QPushButton切换SRT/RTMP
   - QLineEdit推流地址
   - QCheckBox或QPushButton录像开关
   - QPushButton开始/停止（文本/颜色切换）
4. **样式表**：提供完整的QSS样式文件，实现暗色主题
5. **信号连接**：所有控件变更发出对应信号
6. **与ConfigManager绑定**：初始化时读取配置，变更时更新配置
```

## 步骤4：区域选择覆盖层实现
```
请实现屏幕区域选择的覆盖层窗口。要求：

### 类设计：RegionOverlay (继承自QWidget)
```cpp
class RegionOverlay : public QWidget {
    Q_OBJECT
public:
    explicit RegionOverlay(QWidget* parent = nullptr);
    
    // 设置捕获模式 (1,2,4)
    void setRegionMode(int mode);
    
    // 获取当前捕获区域(屏幕坐标)
    QRect getCaptureRect() const;
    
    // 设置/获取屏幕尺寸
    void setScreenSize(const QSize& size);
    
signals:
    void regionChanged(const QRect& newRect);  // 区域变更信号
```

### 功能要求
1. **全屏覆盖**：窗口覆盖所有屏幕，无边框，置顶
2. **透明度**：setAttribute(Qt::WA_TranslucentBackground)
3. **绘制逻辑**：重写paintEvent
   - 黑色半透明背景 (#80000000) 填充整个屏幕
   - 在捕获区域绘制一个"洞"：使用QPainter::CompositionMode_Clear清除区域
   - 在洞的边缘绘制红色虚线矩形框
4. **区域计算**：
   - 模式1(1/4屏)：width=屏幕宽/2, height=屏幕高/2, 居中
   - 模式2(1/2屏)：width=屏幕宽/√2, height=屏幕高/√2, 居中  
   - 模式3(全屏)：覆盖全屏
5. **更新机制**：setRegionMode时重新计算并update()
6. **鼠标穿透**：需要设置窗口不捕获鼠标事件，让鼠标可以穿透操作其他程序
   - setAttribute(Qt::WA_TransparentForMouseEvents)

### 与工具栏联动
- 工具栏的+/-按钮发出信号 → 调用setRegionMode → emit regionChanged → 捕获模块更新
```

## 步骤5：屏幕捕获模块实现
```
请实现基于Qt 6.10 QScreenCapture的屏幕捕获模块。要求：

### 类设计：ScreenCapture (继承自QObject)
```cpp
class ScreenCapture : public QObject {
    Q_OBJECT
public:
    explicit ScreenCapture(QObject* parent = nullptr);
    ~ScreenCapture();
    
    bool start(const QRect& captureRect);  // 开始捕获指定区域
    void stop();
    
    bool isCapturing() const;
    
signals:
    void frameCaptured(const QVideoFrame& frame);  // 原始帧信号
    void errorOccurred(const QString& error);
    
private slots:
    void onNewVideoFrame(const QVideoFrame& frame);
    
private:
    QScreenCapture* m_screenCapture;
    QMediaCaptureSession* m_captureSession;
    QVideoSink* m_videoSink;
    QRect m_captureRect;
};
```

### 实现要求
1. **Qt Multimedia组件使用**：
   - QScreenCapture：屏幕捕获核心类 (Qt 6.5+)
   - QMediaCaptureSession：管理捕获会话
   - QVideoSink：接收视频帧
2. **区域裁剪**：
   - QScreenCapture默认捕获整个屏幕
   - 通过QVideoFrame转换为QImage后，使用copy(rect)裁剪
   - 或使用QVideoFrame的map()获取数据手动裁剪
3. **帧率控制**：暂不需要限制，由定时器控制帧读取
4. **错误处理**：捕获失败时emit errorOccurred
5. **性能优化**：避免不必要的拷贝，考虑使用QVideoFrame的直接处理
```

## 步骤6：摄像头捕获模块实现
```
请实现基于QCamera的摄像头捕获模块，支持多摄像头动态检测和切换。要求：

### 类设计：CameraCapture (继承自QObject)
```cpp
class CameraCapture : public QObject {
    Q_OBJECT
public:
    explicit CameraCapture(QObject* parent = nullptr);
    ~CameraCapture();
    
    // 枚举可用摄像头
    static QList<QCameraDevice> availableCameras();
    
    bool start(const QString& cameraId);  // 指定摄像头ID启动
    void stop();
    
    bool isCapturing() const;
    QString currentCameraId() const;
    
signals:
    void frameCaptured(const QVideoFrame& frame);  // 摄像头帧
    void cameraListChanged();  // 摄像头热插拔时触发
    void errorOccurred(const QString& error);
    
private slots:
    void onNewVideoFrame(const QVideoFrame& frame);
    
private:
    QCamera* m_camera;
    QMediaCaptureSession* m_captureSession;
    QVideoSink* m_videoSink;
    QScopedPointer<QVideoSink> m_sink;
};
```

### 功能要求
1. **设备枚举**：QCameraDevice::availableCameras() 静态方法
2. **设备切换**：stop() → 设置新摄像头 → start()
3. **热插拔检测**：连接QCameraDevice的添加/移除信号，emit cameraListChanged
4. **帧接收**：同屏幕捕获，使用QVideoSink接收帧
5. **错误处理**：摄像头占用、断开连接等异常处理
```

## 步骤7：视频混合器 (画中画) 实现
```
请实现视频混合器模块，将屏幕捕获和摄像头画面叠加为单路视频。要求：

### 类设计：VideoMixer (继承自QObject)
```cpp
class VideoMixer : public QObject {
    Q_OBJECT
public:
    explicit VideoMixer(QObject* parent = nullptr);
    
    // 设置输入源
    void setScreenSource(ScreenCapture* screenCapture);
    void setCameraSource(CameraCapture* cameraCapture);
    
    // 画中画设置
    void enablePip(bool enable);
    void setPipPosition(int position);  // 0:左上,1:右上,2:左下,3:右下
    void setPipSize(int size);          // 0:小(25%),1:中(50%),2:原始(100%)
    
    // 输出分辨率设置 (固定1920x1080)
    void setOutputResolution(int width, int height);
    
    // 启动/停止混合输出
    void start();
    void stop();
    
signals:
    void mixedFrameReady(const QVideoFrame& frame);  // 混合后的帧
    
private slots:
    void onScreenFrame(const QVideoFrame& frame);
    void onCameraFrame(const QVideoFrame& frame);
    
private:
    QVideoFrame mixFrames(const QVideoFrame& screen, const QVideoFrame& camera);
    QImage videoFrameToImage(const QVideoFrame& frame);
```

### 实现要求
1. **格式转换**：
   - 将QVideoFrame转换为QImage进行合成
   - 确保格式统一为QImage::Format_RGB32或ARGB32
2. **分辨率缩放**：
   - 屏幕帧缩放至1920x1080（如有需要）
   - 摄像头帧根据画中画大小缩放
3. **画中画叠加**：
   - 根据position计算摄像头位置
   - 使用QPainter绘制到屏幕图像上
4. **性能优化**：
   - 避免每帧都重新分配内存
   - 考虑使用QVideoFrame的直接操作而非QImage转换
5. **线程安全**：使用QMutex保护共享资源
```

## 步骤8：编码器管理器与硬件编码器检测
```
请实现编码器管理器，支持运行时检测可用编码器并动态切换。要求：

### 类设计：EncoderManager (封装FFmpeg)
```cpp
class EncoderManager : public QObject {
    Q_OBJECT
public:
    explicit EncoderManager(QObject* parent = nullptr);
    ~EncoderManager();
    
    // 静态方法：检测可用硬件编码器
    static QList<QString> detectAvailableEncoders();
    
    // 初始化编码器
    bool initialize(int width, int height, 
                    int bitrate, int gopSeconds,
                    const QString& encoderName);  // 可选编码器名称
    
    // 编码一帧
    bool encodeFrame(const QVideoFrame& frame, 
                     std::vector<uint8_t>& output);
    
    // 刷新编码器（结束推流时调用）
    bool flush();
    
    // 获取当前编码器信息
    QString currentEncoder() const;
    
signals:
    void encoderChanged(const QString& newEncoder);
    void errorOccurred(const QString& error);
```

### FFmpeg集成要求
1. **动态链接**：使用LoadLibrary/dlopen方式加载FFmpeg库，避免编译时依赖
2. **编码器枚举**：
   - avcodec_find_encoder_by_name尝试所有已知硬件编码器
   - 返回可用列表：h264_nvenc, h264_amf, h264_qsv, libx264
3. **编码器初始化参数**：
   - 宽高: 1920x1080
   - 像素格式: AV_PIX_FMT_YUV420P
   - 码率控制: CBR
   - GOP: 根据gopSeconds设置gop_size = fps * gopSeconds
   - 低延迟优化: 
     - AV_CODEC_FLAG_LOW_DELAY
     - 设置tune为zerolatency (如编码器支持)
4. **帧编码流程**：
   - 将QVideoFrame转换为AVFrame
   - 调用avcodec_send_frame
   - 循环avcodec_receive_packet获取编码包
5. **错误处理**：编码失败时自动尝试下一个可用编码器
```

## 步骤9：SRT/RTMP推流模块实现
```
请实现推流模块，支持SRT和RTMP协议。要求：

### 类设计：Streamer (封装FFmpeg)
```cpp
class Streamer : public QObject {
    Q_OBJECT
public:
    explicit Streamer(QObject* parent = nullptr);
    ~Streamer();
    
    // 设置推流参数
    void setProtocol(const QString& protocol);  // "srt" 或 "rtmp"
    void setUrl(const QString& url);
    void setEncoderManager(EncoderManager* encoder);
    
    // 推流控制
    bool start();
    void stop();
    bool isStreaming() const;
    
    // 发送编码后的包
    bool sendPacket(const AVPacket* packet);
    
signals:
    void streamingStatusChanged(bool isStreaming);
    void bitrateUpdate(int currentBitrate);  // 实时码率
    void errorOccurred(const QString& error);
```

### FFmpeg推流要求
1. **协议特定参数**：
   - **SRT**: `srt://ip:port?mode=caller&latency=200000` (延迟200ms)
   - **RTMP**: `rtmp://server/live/streamKey`
2. **封装格式**：
   - SRT通常使用mpegts封装: `avformat_alloc_output_context2(&m_fmtCtx, nullptr, "mpegts", url)`
   - RTMP使用flv封装: `avformat_alloc_output_context2(&m_fmtCtx, nullptr, "flv", url)`
3. **IO错误处理**：网络中断时尝试重连（最多3次）
4. **实时码率统计**：统计每秒发送的字节数，emit bitrateUpdate
5. **资源释放**：确保av_write_trailer()和avio_closep()被调用
```

## 步骤10：本地录像模块实现
```
请实现本地录像模块，支持MP4格式存储。要求：

### 类设计：LocalRecorder
```cpp
class LocalRecorder : public QObject {
    Q_OBJECT
public:
    explicit LocalRecorder(QObject* parent = nullptr);
    ~LocalRecorder();
    
    // 开始录像
    bool startRecording(const QString& filePath, 
                        int width, int height, 
                        int bitrate, int gopSeconds);
    
    // 停止录像
    void stopRecording();
    
    // 写入一帧
    bool writeFrame(const QVideoFrame& frame);
    
    bool isRecording() const;
    
signals:
    void recordingStarted(const QString& filePath);
    void recordingStopped();
    void errorOccurred(const QString& error);
```

### 实现要求
1. **录像独立于推流**：可与推流同时进行，也可单独进行
2. **文件命名**：自动生成文件名，格式：ScreenStream_YYYYMMDD_HHMMSS.mp4
3. **MP4封装**：使用FFmpeg的mp4 muxer
4. **编码复用**：可与推流共享同一个编码器，或独立编码器
5. **文件保存路径**：默认保存在用户视频目录，可配置
```

## 步骤11：主程序集成与信号流
```
请实现主程序逻辑，集成所有模块。要求：

### 类设计：MainApplication
```cpp
class MainApplication : public QObject {
    Q_OBJECT
public:
    explicit MainApplication(QObject* parent = nullptr);
    ~MainApplication();
    
    bool initialize();  // 初始化所有组件
    void shutdown();    // 安全退出
    
private slots:
    // UI信号处理
    void onRegionModeChanged(int mode);
    void onCameraToggled(bool enabled);
    void onCameraSelected(const QString& id);
    void onPipSettingsChanged(bool enabled, int pos, int size);
    void onStartStream();
    void onStopStream();
    void onRecordToggled(bool enabled);
    
    // 模块间信号转发
    void onScreenFrame(const QVideoFrame& frame);
    void onCameraFrame(const QVideoFrame& frame);
    void onMixedFrameReady(const QVideoFrame& frame);
    
private:
    // UI组件
    QScopedPointer<MainToolbar> m_toolbar;
    QScopedPointer<RegionOverlay> m_overlay;
    
    // 核心模块
    QScopedPointer<ScreenCapture> m_screenCapture;
    QScopedPointer<CameraCapture> m_cameraCapture;
    QScopedPointer<VideoMixer> m_mixer;
    QScopedPointer<EncoderManager> m_encoder;
    QScopedPointer<Streamer> m_streamer;
    QScopedPointer<LocalRecorder> m_recorder;
    
    // 配置
    ConfigManager& m_config;
    
    // 状态
    bool m_isStreaming;
    bool m_isRecording;
};
```

### 实现要求
1. **初始化顺序**：
   - 加载配置
   - 创建UI组件
   - 检测可用摄像头（更新UI列表）
   - 初始化捕获模块（待命状态）
2. **信号连接**：
   - 工具栏信号 → 相应模块操作
   - 捕获模块帧信号 → 混合器
   - 混合器帧信号 → 编码器
   - 编码器包信号 → 推流器/录像器
3. **启动推流流程**：
   - 停止之前的捕获
   - 根据区域模式设置捕获区域
   - 启动屏幕捕获
   - 如开启摄像头，启动摄像头捕获
   - 启动混合器
   - 初始化编码器（自动检测/fallback）
   - 启动推流器/录像器
4. **停止推流流程**：反向顺序停止，释放资源
5. **错误处理**：任何模块错误时，停止所有操作，UI显示错误
```

## 步骤12：打包与安装程序制作
```
请提供将程序打包成exe安装包的完整方案。要求：

### 1. 使用windeployqt部署Qt依赖
创建deploy.bat脚本：
```batch
@echo off
set QT_PATH=C:\Qt\6.10.0\mingw_64\bin
set PATH=%QT_PATH%;%PATH%
windeployqt --qmldir .\src --release .\build\release\ScreenStreamer.exe
```

### 2. FFmpeg动态库
确保以下dll文件复制到exe同目录：
- avcodec-61.dll
- avformat-61.dll  
- avutil-59.dll
- swscale-8.dll
- avdevice-61.dll (可选)
- 如有需要：srt.dll, libcrypto等依赖

### 3. 使用CMake构建Release版本
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=C:/Qt/6.10.0/mingw_64
cmake --build . --config Release
```

### 4. InnoSetup安装包脚本
创建deploy/installer.iss：
```iss
[Setup]
AppName=Screen Streamer
AppVersion=1.0.0
DefaultDirName={pf}\ScreenStreamer
DefaultGroupName=Screen Streamer
UninstallDisplayIcon={app}\ScreenStreamer.exe
Compression=lzma2
SolidCompression=yes
OutputDir=.\Output
OutputBaseFilename=ScreenStreamer_Setup

[Files]
Source: "..\build\release\ScreenStreamer.exe"; DestDir: "{app}"
Source: "..\build\release\*.dll"; DestDir: "{app}"
Source: "..\build\release\platforms\*"; DestDir: "{app}\platforms"
Source: "..\build\release\imageformats\*"; DestDir: "{app}\imageformats"
Source: "..\build\release\styles\*"; DestDir: "{app}\styles"
Source: "..\build\release\multimedia\*"; DestDir: "{app}\multimedia"
; 复制FFmpeg DLLs
Source: "C:\ffmpeg\bin\*.dll"; DestDir: "{app}"

[Icons]
Name: "{group}\Screen Streamer"; Filename: "{app}\ScreenStreamer.exe"
Name: "{group}\Uninstall Screen Streamer"; Filename: "{uninstallexe}"
```

### 5. 安装包测试
- 在干净的Windows环境测试安装
- 验证所有依赖已正确打包
- 验证程序能正常运行
```

## 步骤13：测试方案设计
```
请提供程序的测试方案。要求：

### 1. 单元测试
- 使用Qt Test框架
- 测试ConfigManager的读写功能
- 测试RegionOverlay的区域计算逻辑
- 测试VideoMixer的画中画位置计算

### 2. 硬件编码器检测测试
- 在不同硬件上验证检测结果
  - NVIDIA显卡：应检测到h264_nvenc
  - AMD显卡：应检测到h264_amf  
  - Intel集成显卡：应检测到h264_qsv
  - 无硬件加速环境：应回退到libx264

### 3. 性能测试
- 使用高分辨率屏幕（2K/4K）测试60 FPS稳定性
- 测量捕获→编码→推流全链路延迟
- 测试摄像头叠加时的CPU/GPU占用

### 4. 异常场景测试
- 摄像头热插拔
- 网络断开后重连
- 编码器初始化失败fallback
- 磁盘空间不足（本地录像）

### 5. 兼容性测试
- Windows 10/11 不同版本
- 单显示器/多显示器环境
- 不同分辨率缩放比例（100%/125%/150%）

### 6. 性能优化建议
- 如果CPU占用过高：检查QImage转换是否频繁
- 如果延迟过高：检查GOP设置和编码器低延迟参数
- 如果内存泄漏：使用VLD或Dr.Memory检测
```

---

## ✅ 第五部分：以上步骤的确认

以上全部十三个步骤的提示词已经生成完成，涵盖了从环境搭建、模块实现、集成测试到打包分发的完整开发流程。

**使用建议：**
1. 你可以将每个步骤的提示词分别输入到vibe coding工具中
2. 建议按顺序从步骤1开始逐个生成和实现
3. 每个步骤生成后，可能需要根据实际代码输出调整后续步骤的实现细节
4. 步骤8-10（编码器、推流、录像）的FFmpeg实现较为复杂，可能需要迭代调试

如果你需要我针对某个具体步骤进一步细化，或者调整某些技术细节，随时告诉我！