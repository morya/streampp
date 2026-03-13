#include "VideoMixer.h"
#include "ScreenCapture.h"
#include "CameraCapture.h"
#include <QImage>
#include <QDebug>

VideoMixer::VideoMixer(QObject* parent) : QObject(parent) {
    qDebug() << "VideoMixer created";
}

void VideoMixer::setScreenSource(ScreenCapture* screenCapture) {
    m_screenSource = screenCapture;
    qDebug() << "VideoMixer::setScreenSource";
}

void VideoMixer::setCameraSource(CameraCapture* cameraCapture) {
    m_cameraSource = cameraCapture;
    qDebug() << "VideoMixer::setCameraSource";
}

void VideoMixer::enablePip(bool enable) {
    m_pipEnabled = enable;
    qDebug() << "VideoMixer::enablePip" << enable;
}

void VideoMixer::setPipPosition(int position) {
    m_pipPosition = position;
    qDebug() << "VideoMixer::setPipPosition" << position;
}

void VideoMixer::setPipSize(int size) {
    m_pipSize = size;
    qDebug() << "VideoMixer::setPipSize" << size;
}

void VideoMixer::setOutputResolution(int width, int height) {
    m_outputWidth = width;
    m_outputHeight = height;
    qDebug() << "VideoMixer::setOutputResolution" << width << "x" << height;
}

void VideoMixer::start() {
    m_isRunning = true;
    qDebug() << "VideoMixer::start";
}

void VideoMixer::stop() {
    m_isRunning = false;
    qDebug() << "VideoMixer::stop";
}

void VideoMixer::onScreenFrame(const QVideoFrame& frame) {
    if (!m_isRunning) return;
    
    QMutexLocker locker(&m_frameMutex);
    m_lastScreenFrame = frame;
    
    if (m_pipEnabled && !m_lastCameraFrame.isValid()) {
        emit mixedFrameReady(frame);
    } else if (m_pipEnabled) {
        QVideoFrame mixed = mixFrames(frame, m_lastCameraFrame);
        emit mixedFrameReady(mixed);
    } else {
        emit mixedFrameReady(frame);
    }
}

void VideoMixer::onCameraFrame(const QVideoFrame& frame) {
    if (!m_isRunning) return;
    
    QMutexLocker locker(&m_frameMutex);
    m_lastCameraFrame = frame;
    
    if (m_pipEnabled && m_lastScreenFrame.isValid()) {
        QVideoFrame mixed = mixFrames(m_lastScreenFrame, frame);
        emit mixedFrameReady(mixed);
    }
}

QVideoFrame VideoMixer::mixFrames(const QVideoFrame& screen, const QVideoFrame& camera) {
    qDebug() << "VideoMixer::mixFrames";
    return screen; // 简化：直接返回屏幕帧
}

QImage VideoMixer::videoFrameToImage(const QVideoFrame& frame) {
    QImage image;
    qDebug() << "VideoMixer::videoFrameToImage";
    return image;
}