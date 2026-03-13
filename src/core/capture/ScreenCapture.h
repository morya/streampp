#ifndef SCREENCAPTURE_H
#define SCREENCAPTURE_H

#include <QObject>
#include <QRect>
#include <QVideoFrame>

QT_BEGIN_NAMESPACE
class QScreenCapture;
class QMediaCaptureSession;
class QVideoSink;
QT_END_NAMESPACE

class ScreenCapture : public QObject {
    Q_OBJECT
public:
    explicit ScreenCapture(QObject* parent = nullptr);
    ~ScreenCapture();
    
    bool start(const QRect& captureRect);
    void stop();
    
    bool isCapturing() const;
    QRect captureRect() const { return m_captureRect; }
    
signals:
    void frameCaptured(const QVideoFrame& frame);
    void errorOccurred(const QString& error);
    
private slots:
    void onNewVideoFrame(const QVideoFrame& frame);
    
private:
    QScreenCapture* m_screenCapture = nullptr;
    QMediaCaptureSession* m_captureSession = nullptr;
    QVideoSink* m_videoSink = nullptr;
    QRect m_captureRect;
    bool m_isCapturing = false;
};

#endif // SCREENCAPTURE_H