#ifndef STREAMER_H
#define STREAMER_H

#include <QObject>
#include <QString>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

class EncoderManager;

class Streamer : public QObject {
    Q_OBJECT
public:
    explicit Streamer(QObject* parent = nullptr);
    ~Streamer();
    
    void setProtocol(const QString& protocol);
    void setUrl(const QString& url);
    void setEncoderManager(EncoderManager* encoder);
    
    bool start();
    void stop();
    bool isStreaming() const;
    
    bool sendPacket(const AVPacket* packet);
    
signals:
    void streamingStatusChanged(bool isStreaming);
    void bitrateUpdate(int currentBitrate);
    void errorOccurred(const QString& error);
    
private:
    bool initOutputContext();
    bool addStream();
    bool openOutput();
    void cleanup();
    
    QString m_protocol = "srt";
    QString m_url;
    EncoderManager* m_encoder = nullptr;
    
    AVFormatContext* m_outputContext = nullptr;
    AVStream* m_videoStream = nullptr;
    
    qint64 m_startTime = 0;
    qint64 m_bytesSent = 0;
    qint64 m_lastBitrateTime = 0;
    
    bool m_isStreaming = false;
    int m_reconnectAttempts = 0;
};

#endif // STREAMER_H