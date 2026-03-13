#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>

class MainToolbar : public QWidget {
    Q_OBJECT
public:
    explicit MainToolbar(QWidget* parent = nullptr);
    
    void updateCameraList(const QStringList& cameras);
    void updateStreamingStatus(bool isStreaming, int fps, int bitrate);
    void showError(const QString& error);
    
    void setRegionMode(int mode);
    void setCameraEnabled(bool enabled);
    void setCameraId(const QString& id);
    void setBitrate(int bitrate);
    void setGopSeconds(int seconds);
    void setProtocol(const QString& protocol);
    void setUrl(const QString& url);
    void setRecordEnabled(bool enabled);
    
signals:
    void regionModeChanged(int mode);
    void cameraToggled(bool enabled);
    void cameraSelected(const QString& id);
    void pipSettingsChanged(bool enabled, int position, int size);
    void bitrateChanged(int bitrate);
    void gopChanged(int seconds);
    void protocolChanged(const QString& protocol);
    void urlChanged(const QString& url);
    void recordToggled(bool enabled);
    void startStreamClicked();
    void stopStreamClicked();
    
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    
private slots:
    void onRegionMinusClicked();
    void onRegionPlusClicked();
    void onCameraToggleClicked();
    void onCameraSelected(int index);
    void onBitrateChanged(int index);
    void onGopChanged(int index);
    void onProtocolToggled(bool checked);
    void onUrlChanged();
    void onRecordToggled(bool checked);
    void onStartStopClicked();
    
private:
    void setupUI();
    void setupConnections();
    void updateRegionLabel();
    void updateStartStopButton(bool isStreaming);
    
    QPoint m_dragPosition;
    bool m_isDragging = false;
    
    // UI组件
    QLabel* m_regionLabel = nullptr;
    QPushButton* m_regionMinusBtn = nullptr;
    QPushButton* m_regionPlusBtn = nullptr;
    QPushButton* m_cameraToggleBtn = nullptr;
    QComboBox* m_cameraCombo = nullptr;
    QComboBox* m_bitrateCombo = nullptr;
    QComboBox* m_gopCombo = nullptr;
    QPushButton* m_protocolBtn = nullptr;
    QLineEdit* m_urlEdit = nullptr;
    QCheckBox* m_recordCheck = nullptr;
    QPushButton* m_startStopBtn = nullptr;
    QLabel* m_statusLabel = nullptr;
    
    // 状态
    int m_regionMode = 1; // 1,2,4
    bool m_isStreaming = false;
    QStringList m_cameraList;
};

#endif // MAINTOOLBAR_H