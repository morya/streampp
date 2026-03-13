#ifndef VIDEOMIXER_H
#define VIDEOMIXER_H

#include <QObject>
#include <QVideoFrame>
#include <QMutex>

class ScreenCapture;
class CameraCapture;

class VideoMixer : public QObject {
    Q_OBJECT
public:
    explicit VideoMixer(QObject* parent = nullptr);
    
    void setScreenSource(ScreenCapture* screenCapture);
    void setCameraSource(CameraCapture* cameraCapture);
    
    void enablePip(bool enable);
    void setPipPosition(int position);
    void setPipSize(int size);
    
    void setOutputResolution(int width, int height);
    
    void start();
    void stop();
    
signals:
    void mixedFrameReady(const QVideoFrame& frame);
    
public slots:
    void onScreenFrame(const QVideoFrame& frame);
    void onCameraFrame(const QVideoFrame& frame);
    
private:
    QVideoFrame mixFrames(const QVideoFrame& screen, const QVideoFrame& camera);
    QImage videoFrameToImage(const QVideoFrame& frame);
    
    ScreenCapture* m_screenSource = nullptr;
    CameraCapture* m_cameraSource = nullptr;
    
    bool m_pipEnabled = false;
    int m_pipPosition = 0; // 0:左上,1:右上,2:左下,3:右下
    int m_pipSize = 0;     // 0:小(25%),1:中(50%),2:原始(100%)
    
    int m_outputWidth = 1920;
    int m_outputHeight = 1080;
    
    QVideoFrame m_lastScreenFrame;
    QVideoFrame m_lastCameraFrame;
    QMutex m_frameMutex;
    
    bool m_isRunning = false;
};

#endif // VIDEOMIXER_H