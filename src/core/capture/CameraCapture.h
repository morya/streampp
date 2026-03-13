#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <QObject>
#include <QList>
#include <QVideoFrame>

QT_BEGIN_NAMESPACE
class QCamera;
class QCameraDevice;
class QMediaCaptureSession;
class QVideoSink;
QT_END_NAMESPACE

class CameraCapture : public QObject {
    Q_OBJECT
public:
    explicit CameraCapture(QObject* parent = nullptr);
    ~CameraCapture();
    
    static QList<QCameraDevice> availableCameras();
    
    bool start(const QString& cameraId);
    void stop();
    
    bool isCapturing() const;
    QString currentCameraId() const;
    
signals:
    void frameCaptured(const QVideoFrame& frame);
    void cameraListChanged();
    void errorOccurred(const QString& error);
    
private slots:
    void onNewVideoFrame(const QVideoFrame& frame);
    
private:
    QCamera* m_camera = nullptr;
    QMediaCaptureSession* m_captureSession = nullptr;
    QVideoSink* m_videoSink = nullptr;
    QString m_currentCameraId;
    bool m_isCapturing = false;
};

#endif // CAMERACAPTURE_H