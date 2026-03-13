#include "CameraCapture.h"
#include <QCamera>
#include <QCameraDevice>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QDebug>

CameraCapture::CameraCapture(QObject* parent) : QObject(parent) {
    qDebug() << "CameraCapture created";
}

CameraCapture::~CameraCapture() {
    stop();
    qDebug() << "CameraCapture destroyed";
}

QList<QCameraDevice> CameraCapture::availableCameras() {
    QList<QCameraDevice> cameras;
    qDebug() << "CameraCapture::availableCameras";
    return cameras;
}

bool CameraCapture::start(const QString& cameraId) {
    qDebug() << "CameraCapture::start" << cameraId;
    m_currentCameraId = cameraId;
    m_isCapturing = true;
    return true;
}

void CameraCapture::stop() {
    qDebug() << "CameraCapture::stop";
    m_isCapturing = false;
    m_currentCameraId.clear();
}

bool CameraCapture::isCapturing() const {
    return m_isCapturing;
}

QString CameraCapture::currentCameraId() const {
    return m_currentCameraId;
}

void CameraCapture::onNewVideoFrame(const QVideoFrame& frame) {
    if (m_isCapturing) {
        emit frameCaptured(frame);
    }
}