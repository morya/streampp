#include "Streamer.h"
#include "core/encoder/EncoderManager.h"
#include <QDebug>

Streamer::Streamer(QObject* parent) : QObject(parent) {
    qDebug() << "Streamer created";
}

Streamer::~Streamer() {
    cleanup();
    qDebug() << "Streamer destroyed";
}

void Streamer::setProtocol(const QString& protocol) {
    m_protocol = protocol;
    qDebug() << "Streamer::setProtocol" << protocol;
}

void Streamer::setUrl(const QString& url) {
    m_url = url;
    qDebug() << "Streamer::setUrl" << url;
}

void Streamer::setEncoderManager(EncoderManager* encoder) {
    m_encoder = encoder;
    qDebug() << "Streamer::setEncoderManager";
}

bool Streamer::start() {
    qDebug() << "Streamer::start" << m_protocol << m_url;
    m_isStreaming = true;
    emit streamingStatusChanged(true);
    return true;
}

void Streamer::stop() {
    qDebug() << "Streamer::stop";
    m_isStreaming = false;
    cleanup();
    emit streamingStatusChanged(false);
}

bool Streamer::isStreaming() const {
    return m_isStreaming;
}

bool Streamer::sendPacket(const AVPacket* packet) {
    if (!m_isStreaming) {
        return false;
    }
    m_bytesSent += packet->size;
    return true;
}

bool Streamer::initOutputContext() {
    qDebug() << "Streamer::initOutputContext";
    return true;
}

bool Streamer::addStream() {
    qDebug() << "Streamer::addStream";
    return true;
}

bool Streamer::openOutput() {
    qDebug() << "Streamer::openOutput";
    return true;
}

void Streamer::cleanup() {
    qDebug() << "Streamer::cleanup";
    m_outputContext = nullptr;
    m_videoStream = nullptr;
    m_bytesSent = 0;
    m_reconnectAttempts = 0;
}