#ifndef ENCODERMANAGER_H
#define ENCODERMANAGER_H

#include <QObject>
#include <vector>
#include <cstdint>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
}

class EncoderManager : public QObject {
    Q_OBJECT
public:
    explicit EncoderManager(QObject* parent = nullptr);
    ~EncoderManager();
    
    static QStringList detectAvailableEncoders();
    
    bool initialize(int width, int height, 
                    int bitrate, int gopSeconds,
                    const QString& encoderName = QString());
    
    bool encodeFrame(const QVideoFrame& frame, 
                     std::vector<uint8_t>& output);
    
    bool flush();
    
    QString currentEncoder() const;
    
signals:
    void encoderChanged(const QString& newEncoder);
    void errorOccurred(const QString& error);
    
private:
    bool initCodec(const QString& encoderName);
    bool setupCodecContext(int width, int height, int bitrate, int gopSeconds);
    void cleanup();
    
    AVCodec* m_codec = nullptr;
    AVCodecContext* m_codecContext = nullptr;
    AVFrame* m_frame = nullptr;
    AVPacket* m_packet = nullptr;
    
    int m_width = 0;
    int m_height = 0;
    QString m_currentEncoder;
    bool m_initialized = false;
};

#endif // ENCODERMANAGER_H