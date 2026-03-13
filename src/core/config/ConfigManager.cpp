#include "ConfigManager.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDebug>

ConfigManager& ConfigManager::instance() {
    static ConfigManager instance;
    return instance;
}

QString ConfigManager::configFilePath() const {
    QString configDir;
    
#ifdef Q_OS_WIN
    configDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
#else
    configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
#endif
    
    QDir dir(configDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    return dir.filePath("config.json");
}

void ConfigManager::loadDefaults() {
    m_captureConfig = CaptureConfig{};
    m_encoderConfig = EncoderConfig{};
    m_streamConfig = StreamConfig{};
}

bool ConfigManager::load() {
    QString filePath = configFilePath();
    QFile file(filePath);
    
    if (!file.exists()) {
        loadDefaults();
        return save(); // 创建默认配置文件
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open config file:" << filePath;
        loadDefaults();
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse config file:" << error.errorString();
        loadDefaults();
        return false;
    }
    
    QJsonObject root = doc.object();
    
    // 加载捕获配置
    if (root.contains("capture") && root["capture"].isObject()) {
        QJsonObject captureObj = root["capture"].toObject();
        m_captureConfig.regionMode = captureObj.value("regionMode").toInt(1);
        m_captureConfig.cameraEnabled = captureObj.value("cameraEnabled").toBool(false);
        m_captureConfig.cameraId = captureObj.value("cameraId").toString().toStdString();
        m_captureConfig.pipEnabled = captureObj.value("pipEnabled").toBool(false);
        m_captureConfig.pipPosition = captureObj.value("pipPosition").toInt(0);
        m_captureConfig.pipSize = captureObj.value("pipSize").toInt(0);
    }
    
    // 加载编码配置
    if (root.contains("encoder") && root["encoder"].isObject()) {
        QJsonObject encoderObj = root["encoder"].toObject();
        m_encoderConfig.bitrate = encoderObj.value("bitrate").toInt(2000);
        m_encoderConfig.gopSeconds = encoderObj.value("gopSeconds").toInt(2);
        m_encoderConfig.encoderName = encoderObj.value("encoderName").toString("libx264").toStdString();
    }
    
    // 加载推流配置
    if (root.contains("stream") && root["stream"].isObject()) {
        QJsonObject streamObj = root["stream"].toObject();
        m_streamConfig.protocol = streamObj.value("protocol").toString("srt").toStdString();
        m_streamConfig.url = streamObj.value("url").toString("srt://127.0.0.1:9000?mode=caller&latency=200000").toStdString();
        m_streamConfig.recordLocally = streamObj.value("recordLocally").toBool(false);
    }
    
    return true;
}

bool ConfigManager::save() {
    QJsonObject root;
    
    // 保存捕获配置
    QJsonObject captureObj;
    captureObj["regionMode"] = m_captureConfig.regionMode;
    captureObj["cameraEnabled"] = m_captureConfig.cameraEnabled;
    captureObj["cameraId"] = QString::fromStdString(m_captureConfig.cameraId);
    captureObj["pipEnabled"] = m_captureConfig.pipEnabled;
    captureObj["pipPosition"] = m_captureConfig.pipPosition;
    captureObj["pipSize"] = m_captureConfig.pipSize;
    root["capture"] = captureObj;
    
    // 保存编码配置
    QJsonObject encoderObj;
    encoderObj["bitrate"] = m_encoderConfig.bitrate;
    encoderObj["gopSeconds"] = m_encoderConfig.gopSeconds;
    encoderObj["encoderName"] = QString::fromStdString(m_encoderConfig.encoderName);
    root["encoder"] = encoderObj;
    
    // 保存推流配置
    QJsonObject streamObj;
    streamObj["protocol"] = QString::fromStdString(m_streamConfig.protocol);
    streamObj["url"] = QString::fromStdString(m_streamConfig.url);
    streamObj["recordLocally"] = m_streamConfig.recordLocally;
    root["stream"] = streamObj;
    
    QJsonDocument doc(root);
    
    QString filePath = configFilePath();
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open config file for writing:" << filePath;
        return false;
    }
    
    file.write(doc.toJson());
    file.close();
    
    return true;
}

void ConfigManager::setCaptureConfig(const CaptureConfig& config) {
    if (m_captureConfig.regionMode != config.regionMode ||
        m_captureConfig.cameraEnabled != config.cameraEnabled ||
        m_captureConfig.cameraId != config.cameraId ||
        m_captureConfig.pipEnabled != config.pipEnabled ||
        m_captureConfig.pipPosition != config.pipPosition ||
        m_captureConfig.pipSize != config.pipSize) {
        
        m_captureConfig = config;
        emit captureConfigChanged(config);
        save();
    }
}

void ConfigManager::setEncoderConfig(const EncoderConfig& config) {
    if (m_encoderConfig.bitrate != config.bitrate ||
        m_encoderConfig.gopSeconds != config.gopSeconds ||
        m_encoderConfig.encoderName != config.encoderName) {
        
        m_encoderConfig = config;
        emit encoderConfigChanged(config);
        save();
    }
}

void ConfigManager::setStreamConfig(const StreamConfig& config) {
    if (m_streamConfig.protocol != config.protocol ||
        m_streamConfig.url != config.url ||
        m_streamConfig.recordLocally != config.recordLocally) {
        
        m_streamConfig = config;
        emit streamConfigChanged(config);
        save();
    }
}