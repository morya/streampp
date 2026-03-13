#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QDir>
#include <QMessageBox>

#include "core/config/ConfigManager.h"
#include "ui/MainToolbar.h"
#include "core/capture/ScreenCapture.h"
#include "core/capture/CameraCapture.h"
#include "core/capture/VideoMixer.h"
#include "core/encoder/EncoderManager.h"
#include "core/streamer/Streamer.h"
#include "core/recorder/LocalRecorder.h"

class MainApplication : public QObject {
    Q_OBJECT
public:
    explicit MainApplication(QObject* parent = nullptr) : QObject(parent) {}
    
    bool initialize() {
        // 加载配置
        if (!ConfigManager::instance().load()) {
            qWarning() << "Failed to load config, using defaults";
        }
        
        // 创建UI组件
        m_toolbar = new MainToolbar();
        
        // 检测可用摄像头
        auto cameras = CameraCapture::availableCameras();
        QStringList cameraNames;
        for (const auto& camera : cameras) {
            cameraNames << camera.description();
        }
        m_toolbar->updateCameraList(cameraNames);
        
        // 初始化核心模块
        m_screenCapture = new ScreenCapture(this);
        m_cameraCapture = new CameraCapture(this);
        m_mixer = new VideoMixer(this);
        m_encoder = new EncoderManager(this);
        m_streamer = new Streamer(this);
        m_recorder = new LocalRecorder(this);
        
        // 连接信号
        setupConnections();
        
        // 应用配置
        applyConfig();
        
        return true;
    }
    
    void shutdown() {
        // 停止所有活动
        if (m_streamer->isStreaming()) {
            m_streamer->stop();
        }
        if (m_recorder->isRecording()) {
            m_recorder->stopRecording();
        }
        m_screenCapture->stop();
        m_cameraCapture->stop();
        
        // 保存配置
        ConfigManager::instance().save();
    }
    
private slots:
    void onRegionModeChanged(int mode) {
        // 更新配置
        auto config = ConfigManager::instance().captureConfig();
        config.regionMode = mode;
        ConfigManager::instance().setCaptureConfig(config);
        
        // TODO: 更新捕获区域
    }
    
    void onCameraToggled(bool enabled) {
        auto config = ConfigManager::instance().captureConfig();
        config.cameraEnabled = enabled;
        ConfigManager::instance().setCaptureConfig(config);
        
        if (enabled && !config.cameraId.empty()) {
            m_cameraCapture->start(QString::fromStdString(config.cameraId));
        } else {
            m_cameraCapture->stop();
        }
    }
    
    void onCameraSelected(const QString& id) {
        auto config = ConfigManager::instance().captureConfig();
        config.cameraId = id.toStdString();
        ConfigManager::instance().setCaptureConfig(config);
        
        if (config.cameraEnabled) {
            m_cameraCapture->stop();
            m_cameraCapture->start(id);
        }
    }
    
    void onStartStream() {
        // 获取配置
        auto captureConfig = ConfigManager::instance().captureConfig();
        auto encoderConfig = ConfigManager::instance().encoderConfig();
        auto streamConfig = ConfigManager::instance().streamConfig();
        
        // 初始化编码器
        if (!m_encoder->initialize(1920, 1080, 
                                   encoderConfig.bitrate, 
                                   encoderConfig.gopSeconds,
                                   QString::fromStdString(encoderConfig.encoderName))) {
            QMessageBox::critical(nullptr, "Error", "Failed to initialize encoder");
            return;
        }
        
        // 设置推流器
        m_streamer->setProtocol(QString::fromStdString(streamConfig.protocol));
        m_streamer->setUrl(QString::fromStdString(streamConfig.url));
        m_streamer->setEncoderManager(m_encoder);
        
        // 启动推流
        if (!m_streamer->start()) {
            QMessageBox::critical(nullptr, "Error", "Failed to start streaming");
            return;
        }
        
        // 如果需要，启动录像
        if (streamConfig.recordLocally) {
            QString fileName = QDir::homePath() + "/Videos/ScreenStream_" + 
                              QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".mp4";
            m_recorder->startRecording(fileName, 1920, 1080, 
                                      encoderConfig.bitrate, encoderConfig.gopSeconds);
        }
        
        m_toolbar->updateStreamingStatus(true, 60, encoderConfig.bitrate);
    }
    
    void onStopStream() {
        m_streamer->stop();
        m_recorder->stopRecording();
        m_toolbar->updateStreamingStatus(false, 0, 0);
    }
    
    void onRecordToggled(bool enabled) {
        auto config = ConfigManager::instance().streamConfig();
        config.recordLocally = enabled;
        ConfigManager::instance().setStreamConfig(config);
    }
    
private:
    void setupConnections() {
        connect(m_toolbar, &MainToolbar::regionModeChanged, this, &MainApplication::onRegionModeChanged);
        connect(m_toolbar, &MainToolbar::cameraToggled, this, &MainApplication::onCameraToggled);
        connect(m_toolbar, &MainToolbar::cameraSelected, this, &MainApplication::onCameraSelected);
        connect(m_toolbar, &MainToolbar::startStreamClicked, this, &MainApplication::onStartStream);
        connect(m_toolbar, &MainToolbar::stopStreamClicked, this, &MainApplication::onStopStream);
        connect(m_toolbar, &MainToolbar::recordToggled, this, &MainApplication::onRecordToggled);
        
        connect(m_screenCapture, &ScreenCapture::frameCaptured, m_mixer, &VideoMixer::onScreenFrame);
        connect(m_cameraCapture, &CameraCapture::frameCaptured, m_mixer, &VideoMixer::onCameraFrame);
        connect(m_mixer, &VideoMixer::mixedFrameReady, this, &MainApplication::onMixedFrameReady);
    }
    
    void applyConfig() {
        auto captureConfig = ConfigManager::instance().captureConfig();
        auto encoderConfig = ConfigManager::instance().encoderConfig();
        auto streamConfig = ConfigManager::instance().streamConfig();
        
        // 应用捕获配置
        m_toolbar->setRegionMode(captureConfig.regionMode);
        m_toolbar->setCameraEnabled(captureConfig.cameraEnabled);
        m_toolbar->setCameraId(QString::fromStdString(captureConfig.cameraId));
        
        // 应用编码配置
        m_toolbar->setBitrate(encoderConfig.bitrate);
        m_toolbar->setGopSeconds(encoderConfig.gopSeconds);
        
        // 应用推流配置
        m_toolbar->setProtocol(QString::fromStdString(streamConfig.protocol));
        m_toolbar->setUrl(QString::fromStdString(streamConfig.url));
        m_toolbar->setRecordEnabled(streamConfig.recordLocally);
    }
    
    void onMixedFrameReady(const QVideoFrame& frame) {
        // 编码并推流
        std::vector<uint8_t> encodedData;
        if (m_encoder->encodeFrame(frame, encodedData)) {
            // TODO: 发送到推流器和录像器
        }
    }
    
private:
    MainToolbar* m_toolbar = nullptr;
    ScreenCapture* m_screenCapture = nullptr;
    CameraCapture* m_cameraCapture = nullptr;
    VideoMixer* m_mixer = nullptr;
    EncoderManager* m_encoder = nullptr;
    Streamer* m_streamer = nullptr;
    LocalRecorder* m_recorder = nullptr;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("Screen Streamer");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("ScreenStreamer");
    
    // 设置暗色主题
    app.setStyle(QStyleFactory::create("Fusion"));
    
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(darkPalette);
    
    // 加载样式表
    QFile styleFile(":/styles/dark.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
    }
    
    // 创建主应用程序
    MainApplication mainApp;
    if (!mainApp.initialize()) {
        QMessageBox::critical(nullptr, "Fatal Error", 
                             "Failed to initialize application");
        return 1;
    }
    
    int result = app.exec();
    
    // 清理
    mainApp.shutdown();
    
    return result;
}