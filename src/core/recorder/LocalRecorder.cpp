#include "LocalRecorder.h"
#include <QDebug>

LocalRecorder::LocalRecorder(QObject* parent) : QObject(parent) {
    qDebug() << "LocalRecorder created";
}

LocalRecorder::~LocalRecorder() {
    cleanup();
    qDebug() << "LocalRecorder destroyed";
}

bool LocalRecorder::startRecording(const QString& filePath, int width, int height, int bitrate, int gopSeconds) {
    qDebug() << "LocalRecorder::startRecording" << filePath << width << "x" << height << "bitrate:" << bitrate << "gop:" << gopSeconds;
    m_filePath = filePath;
    m_isRecording = true;
    emit recordingStarted(filePath);
    return true;
}

void LocalRecorder::stopRecording() {
    qDebug() << "LocalRecorder::stopRecording";
    m_isRecording = false;
    cleanup();
    emit recordingStopped();
}

bool LocalRecorder::writeFrame(const QVideoFrame& frame) {
    if (!m_isRecording) {
        return false;
    }
    m_frameCount++;
    return true;
}

bool LocalRecorder::isRecording() const {
    return m_isRecording;
}

bool LocalRecorder::initEncoder(int width, int height, int bitrate, int gopSeconds) {
    qDebug() << "LocalRecorder::initEncoder" << width << "x" << height;
    return true;
}

bool LocalRecorder::initOutput(const QString& filePath) {
    qDebug() << "LocalRecorder::initOutput" << filePath;
    return true;
}

void LocalRecorder::cleanup() {
    qDebug() << "LocalRecorder::cleanup";
    m_outputContext = nullptr;
    m_encoderContext = nullptr;
    m_videoStream = nullptr;
    m_frame = nullptr;
    m_packet = nullptr;
    m_frameCount = 0;
}