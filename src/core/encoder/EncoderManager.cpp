#include "EncoderManager.h"
#include <QVideoFrame>
#include <QDebug>

EncoderManager::EncoderManager(QObject* parent) : QObject(parent) {
    qDebug() << "EncoderManager created";
}

EncoderManager::~EncoderManager() {
    cleanup();
    qDebug() << "EncoderManager destroyed";
}

QStringList EncoderManager::detectAvailableEncoders() {
    QStringList encoders;
    encoders << "libx264" << "h264_nvenc" << "h264_amf" << "h264_qsv";
    qDebug() << "EncoderManager::detectAvailableEncoders" << encoders;
    return encoders;
}

bool EncoderManager::initialize(int width, int height, int bitrate, int gopSeconds, const QString& encoderName) {
    qDebug() << "EncoderManager::initialize" << width << "x" << height << "bitrate:" << bitrate << "gop:" << gopSeconds << "encoder:" << encoderName;
    m_width = width;
    m_height = height;
    m_currentEncoder = encoderName.isEmpty() ? "libx264" : encoderName;
    m_initialized = true;
    emit encoderChanged(m_currentEncoder);
    return true;
}

bool EncoderManager::encodeFrame(const QVideoFrame& frame, std::vector<uint8_t>& output) {
    if (!m_initialized) {
        return false;
    }
    output.resize(100); // 模拟编码输出
    return true;
}

bool EncoderManager::flush() {
    qDebug() << "EncoderManager::flush";
    return true;
}

QString EncoderManager::currentEncoder() const {
    return m_currentEncoder;
}

bool EncoderManager::initCodec(const QString& encoderName) {
    qDebug() << "EncoderManager::initCodec" << encoderName;
    return true;
}

bool EncoderManager::setupCodecContext(int width, int height, int bitrate, int gopSeconds) {
    qDebug() << "EncoderManager::setupCodecContext" << width << "x" << height;
    return true;
}

void EncoderManager::cleanup() {
    qDebug() << "EncoderManager::cleanup";
    m_codec = nullptr;
    m_codecContext = nullptr;
    m_frame = nullptr;
    m_packet = nullptr;
    m_initialized = false;
}