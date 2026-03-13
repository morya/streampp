#include "ScreenCapture.h"
#include <QScreenCapture>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QDebug>

ScreenCapture::ScreenCapture(QObject* parent) : QObject(parent) {
    qDebug() << "ScreenCapture created";
}

ScreenCapture::~ScreenCapture() {
    stop();
    qDebug() << "ScreenCapture destroyed";
}

bool ScreenCapture::start(const QRect& captureRect) {
    qDebug() << "ScreenCapture::start" << captureRect;
    m_captureRect = captureRect;
    m_isCapturing = true;
    return true;
}

void ScreenCapture::stop() {
    qDebug() << "ScreenCapture::stop";
    m_isCapturing = false;
    m_captureRect = QRect();
}

bool ScreenCapture::isCapturing() const {
    return m_isCapturing;
}

void ScreenCapture::onNewVideoFrame(const QVideoFrame& frame) {
    if (m_isCapturing) {
        emit frameCaptured(frame);
    }
}