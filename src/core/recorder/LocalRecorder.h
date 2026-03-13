#ifndef LOCALRECORDER_H
#define LOCALRECORDER_H

#include <QObject>
#include <QString>
#include <QVideoFrame>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

class LocalRecorder : public QObject {
    Q_OBJECT
public:
    explicit LocalRecorder(QObject* parent = nullptr);
    ~LocalRecorder();
    
    bool startRecording(const QString& filePath, 
                        int width, int height, 
                        int bitrate, int gopSeconds);
    
    void stopRecording();
    
    bool writeFrame(const QVideoFrame& frame);
    
    bool isRecording() const;
    
signals:
    void recordingStarted(const QString& filePath);
    void recordingStopped();
    void errorOccurred(const QString& error);
    
private:
    bool initEncoder(int width, int height, int bitrate, int gopSeconds);
    bool initOutput(const QString& filePath);
    void cleanup();
    
    AVFormatContext* m_outputContext = nullptr;
    AVCodecContext* m_encoderContext = nullptr;
    AVStream* m_videoStream = nullptr;
    AVFrame* m_frame = nullptr;
    AVPacket* m_packet = nullptr;
    
    QString m_filePath;
    int64_t m_frameCount = 0;
    bool m_isRecording = false;
};

#endif // LOCALRECORDER_H