#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QString>
#include <string>
#include <memory>

class ConfigManager : public QObject {
    Q_OBJECT
public:
    static ConfigManager& instance();
    
    bool load();
    bool save();
    
    struct CaptureConfig {
        int regionMode = 1;        // 1,2,4 对应1/4,1/2,全屏
        bool cameraEnabled = false;
        std::string cameraId;
        bool pipEnabled = false;
        int pipPosition = 0;       // 0:左上,1:右上,2:左下,3:右下
        int pipSize = 0;           // 0:小,1:中,2:原始
    };
    
    struct EncoderConfig {
        int bitrate = 2000;        // kbps
        int gopSeconds = 2;        // GOP间隔(秒)
        std::string encoderName = "libx264";
    };
    
    struct StreamConfig {
        std::string protocol = "srt";
        std::string url = "srt://127.0.0.1:9000?mode=caller&latency=200000";
        bool recordLocally = false;
    };
    
    CaptureConfig captureConfig() const { return m_captureConfig; }
    EncoderConfig encoderConfig() const { return m_encoderConfig; }
    StreamConfig streamConfig() const { return m_streamConfig; }
    
    void setCaptureConfig(const CaptureConfig& config);
    void setEncoderConfig(const EncoderConfig& config);
    void setStreamConfig(const StreamConfig& config);
    
signals:
    void captureConfigChanged(const CaptureConfig& config);
    void encoderConfigChanged(const EncoderConfig& config);
    void streamConfigChanged(const StreamConfig& config);
    
private:
    ConfigManager() = default;
    ~ConfigManager() = default;
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    QString configFilePath() const;
    void loadDefaults();
    
    CaptureConfig m_captureConfig;
    EncoderConfig m_encoderConfig;
    StreamConfig m_streamConfig;
};

#endif // CONFIGMANAGER_H