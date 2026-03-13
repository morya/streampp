#include "MainToolbar.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QScreen>
#include <QStyleOption>
#include <QDebug>

MainToolbar::MainToolbar(QWidget* parent) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    
    setupUI();
    setupConnections();
    
    // 设置初始位置（屏幕中央顶部）
    QScreen* screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - 500) / 2;
    int y = 20;
    move(x, y);
    
    // 设置阴影效果
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 100));
    shadow->setOffset(0, 2);
    setGraphicsEffect(shadow);
    
    resize(500, 60);
}

void MainToolbar::setupUI() {
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 5, 10, 5);
    mainLayout->setSpacing(10);
    
    // 区域控制
    m_regionLabel = new QLabel("1/4屏 (960x540)");
    m_regionLabel->setStyleSheet("color: white; font-size: 12px;");
    m_regionLabel->setFixedWidth(100);
    
    m_regionMinusBtn = new QPushButton("-");
    m_regionMinusBtn->setFixedSize(30, 30);
    m_regionMinusBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #2A82DA;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 4px;"
        "  font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #1E6BB8;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #155A9C;"
        "}"
    );
    
    m_regionPlusBtn = new QPushButton("+");
    m_regionPlusBtn->setFixedSize(30, 30);
    m_regionPlusBtn->setStyleSheet(m_regionMinusBtn->styleSheet());
    
    // 分隔线1
    QFrame* separator1 = new QFrame();
    separator1->setFrameShape(QFrame::VLine);
    separator1->setFrameShadow(QFrame::Sunken);
    separator1->setStyleSheet("color: #3A3A3A;");
    separator1->setFixedHeight(30);
    
    // 摄像头控制
    m_cameraToggleBtn = new QPushButton("📷");
    m_cameraToggleBtn->setFixedSize(30, 30);
    m_cameraToggleBtn->setCheckable(true);
    m_cameraToggleBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #3A3A3A;"
        "  color: white;"
        "  border: 1px solid #4A4A4A;"
        "  border-radius: 4px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:checked {"
        "  background-color: #2A82DA;"
        "}"
        "QPushButton:hover {"
        "  background-color: #4A4A4A;"
        "}"
    );
    
    m_cameraCombo = new QComboBox();
    m_cameraCombo->setFixedWidth(120);
    m_cameraCombo->setStyleSheet(
        "QComboBox {"
        "  background-color: #2A2A2A;"
        "  color: white;"
        "  border: 1px solid #4A4A4A;"
        "  border-radius: 4px;"
        "  padding: 5px;"
        "}"
        "QComboBox::drop-down {"
        "  border: none;"
        "}"
        "QComboBox::down-arrow {"
        "  image: none;"
        "  border-left: 5px solid transparent;"
        "  border-right: 5px solid transparent;"
        "  border-top: 5px solid white;"
        "}"
        "QComboBox QAbstractItemView {"
        "  background-color: #2A2A2A;"
        "  color: white;"
        "  selection-background-color: #2A82DA;"
        "}"
    );
    
    // 分隔线2
    QFrame* separator2 = new QFrame();
    separator2->setFrameShape(QFrame::VLine);
    separator2->setFrameShadow(QFrame::Sunken);
    separator2->setStyleSheet("color: #3A3A3A;");
    separator2->setFixedHeight(30);
    
    // 编码设置
    m_bitrateCombo = new QComboBox();
    m_bitrateCombo->setFixedWidth(80);
    m_bitrateCombo->addItems({"1 Mbps", "2 Mbps", "5 Mbps", "8 Mbps", "Custom"});
    m_bitrateCombo->setCurrentIndex(1); // 2 Mbps
    m_bitrateCombo->setStyleSheet(m_cameraCombo->styleSheet());
    
    m_gopCombo = new QComboBox();
    m_gopCombo->setFixedWidth(60);
    m_gopCombo->addItems({"1s", "2s", "3s", "5s"});
    m_gopCombo->setCurrentIndex(1); // 2s
    m_gopCombo->setStyleSheet(m_cameraCombo->styleSheet());
    
    // 协议选择
    m_protocolBtn = new QPushButton("SRT");
    m_protocolBtn->setFixedSize(60, 30);
    m_protocolBtn->setCheckable(true);
    m_protocolBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #3A3A3A;"
        "  color: white;"
        "  border: 1px solid #4A4A4A;"
        "  border-radius: 4px;"
        "}"
        "QPushButton:checked {"
        "  background-color: #2A82DA;"
        "}"
        "QPushButton:hover {"
        "  background-color: #4A4A4A;"
        "}"
    );
    
    m_urlEdit = new QLineEdit();
    m_urlEdit->setFixedWidth(180);
    m_urlEdit->setPlaceholderText("rtmp:// or srt://...");
    m_urlEdit->setStyleSheet(
        "QLineEdit {"
        "  background-color: #2A2A2A;"
        "  color: white;"
        "  border: 1px solid #4A4A4A;"
        "  border-radius: 4px;"
        "  padding: 5px;"
        "}"
    );
    
    // 录像开关
    m_recordCheck = new QCheckBox("录像");
    m_recordCheck->setStyleSheet(
        "QCheckBox {"
        "  color: white;"
        "  spacing: 5px;"
        "}"
        "QCheckBox::indicator {"
        "  width: 16px;"
        "  height: 16px;"
        "}"
        "QCheckBox::indicator:unchecked {"
        "  background-color: #3A3A3A;"
        "  border: 1px solid #4A4A4A;"
        "  border-radius: 3px;"
        "}"
        "QCheckBox::indicator:checked {"
        "  background-color: #2A82DA;"
        "  border: 1px solid #4A4A4A;"
        "  border-radius: 3px;"
        "}"
    );
    
    // 开始/停止按钮
    m_startStopBtn = new QPushButton("开始推流");
    m_startStopBtn->setFixedSize(80, 30);
    m_startStopBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #0B6A4B;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 4px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #09583E;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #074632;"
        "}"
    );
    
    // 状态标签
    m_statusLabel = new QLabel();
    m_statusLabel->setStyleSheet("color: #CCCCCC; font-size: 11px;");
    m_statusLabel->setFixedWidth(120);
    
    // 添加到布局
    mainLayout->addWidget(m_regionLabel);
    mainLayout->addWidget(m_regionMinusBtn);
    mainLayout->addWidget(m_regionPlusBtn);
    mainLayout->addWidget(separator1);
    mainLayout->addWidget(m_cameraToggleBtn);
    mainLayout->addWidget(m_cameraCombo);
    mainLayout->addWidget(separator2);
    mainLayout->addWidget(m_bitrateCombo);
    mainLayout->addWidget(m_gopCombo);
    mainLayout->addWidget(m_protocolBtn);
    mainLayout->addWidget(m_urlEdit);
    mainLayout->addWidget(m_recordCheck);
    mainLayout->addWidget(m_startStopBtn);
    mainLayout->addWidget(m_statusLabel);
    
    setLayout(mainLayout);
}

void MainToolbar::setupConnections() {
    connect(m_regionMinusBtn, &QPushButton::clicked, this, &MainToolbar::onRegionMinusClicked);
    connect(m_regionPlusBtn, &QPushButton::clicked, this, &MainToolbar::onRegionPlusClicked);
    connect(m_cameraToggleBtn, &QPushButton::toggled, this, &MainToolbar::cameraToggled);
    connect(m_cameraCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainToolbar::onCameraSelected);
    connect(m_bitrateCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainToolbar::onBitrateChanged);
    connect(m_gopCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainToolbar::onGopChanged);
    connect(m_protocolBtn, &QPushButton::toggled, this, &MainToolbar::onProtocolToggled);
    connect(m_urlEdit, &QLineEdit::editingFinished, this, &MainToolbar::onUrlChanged);
    connect(m_recordCheck, &QCheckBox::toggled, this, &MainToolbar::onRecordToggled);
    connect(m_startStopBtn, &QPushButton::clicked, this, &MainToolbar::onStartStopClicked);
}

void MainToolbar::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制半透明磨砂玻璃背景
    QPainterPath path;
    path.addRoundedRect(rect(), 8, 8);
    
    painter.setClipPath(path);
    painter.fillRect(rect(), QColor(30, 30, 30, 217)); // #1E1E1E with 85% opacity
    
    // 绘制边框
    painter.setPen(QPen(QColor(58, 58, 58), 1)); // #3A3A3A
    painter.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 8, 8);
    
    QWidget::paintEvent(event);
}

void MainToolbar::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        m_isDragging = true;
        event->accept();
    }
}

void MainToolbar::mouseMoveEvent(QMouseEvent* event) {
    if (m_isDragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void MainToolbar::mouseReleaseEvent(QMouseEvent* event) {
    m_isDragging = false;
    QWidget::mouseReleaseEvent(event);
}

void MainToolbar::onRegionMinusClicked() {
    if (m_regionMode > 1) {
        m_regionMode /= 2;
        updateRegionLabel();
        emit regionModeChanged(m_regionMode);
    }
}

void MainToolbar::onRegionPlusClicked() {
    if (m_regionMode < 4) {
        m_regionMode *= 2;
        updateRegionLabel();
        emit regionModeChanged(m_regionMode);
    }
}

void MainToolbar::onCameraSelected(int index) {
    if (index >= 0 && index < m_cameraList.size()) {
        emit cameraSelected(m_cameraList[index]);
    }
}

void MainToolbar::onBitrateChanged(int index) {
    int bitrate = 0;
    switch (index) {
        case 0: bitrate = 1000; break;
        case 1: bitrate = 2000; break;
        case 2: bitrate = 5000; break;
        case 3: bitrate = 8000; break;
        case 4: bitrate = 0; // Custom
    }
    if (bitrate > 0) {
        emit bitrateChanged(bitrate);
    }
}

void MainToolbar::onGopChanged(int index) {
    int gop = 0;
    switch (index) {
        case 0: gop = 1; break;
        case 1: gop = 2; break;
        case 2: gop = 3; break;
        case 3: gop = 5; break;
    }
    if (gop > 0) {
        emit gopChanged(gop);
    }
}

void MainToolbar::onProtocolToggled(bool checked) {
    QString protocol = checked ? "rtmp" : "srt";
    m_protocolBtn->setText(protocol.toUpper());
    emit protocolChanged(protocol);
}

void MainToolbar::onUrlChanged() {
    emit urlChanged(m_urlEdit->text());
}

void MainToolbar::onRecordToggled(bool checked) {
    emit recordToggled(checked);
}

void MainToolbar::onStartStopClicked() {
    if (m_isStreaming) {
        emit stopStreamClicked();
    } else {
        emit startStreamClicked();
    }
}

void MainToolbar::updateRegionLabel() {
    QScreen* screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    
    int width = 0, height = 0;
    switch (m_regionMode) {
        case 1: // 1/4屏
            width = screenGeometry.width() / 2;
            height = screenGeometry.height() / 2;
            m_regionLabel->setText(QString("1/4屏 (%1x%2)").arg(width).arg(height));
            break;
        case 2: // 1/2屏
            width = screenGeometry.width() / qSqrt(2);
            height = screenGeometry.height() / qSqrt(2);
            m_regionLabel->setText(QString("1/2屏 (%1x%2)").arg(width).arg(height));
            break;
        case 4: // 全屏
            width = screenGeometry.width();
            height = screenGeometry.height();
            m_regionLabel->setText(QString("全屏 (%1x%2)").arg(width).arg(height));
            break;
    }
}

void MainToolbar::updateStartStopButton(bool isStreaming) {
    m_isStreaming = isStreaming;
    if (isStreaming) {
        m_startStopBtn->setText("停止推流");
        m_startStopBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: #E81123;"
            "  color: white;"
            "  border: none;"
            "  border-radius: 4px;"
            "  font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "  background-color: #C50F1E;"
            "}"
            "QPushButton:pressed {"
            "  background-color: #A30D19;"
            "}"
        );
    } else {
        m_startStopBtn->setText("开始推流");
        m_startStopBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: #0B6A4B;"
            "  color: white;"
            "  border: none;"
            "  border-radius: 4px;"
            "  font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "  background-color: #09583E;"
            "}"
            "QPushButton:pressed {"
            "  background-color: #074632;"
            "}"
        );
    }
}

void MainToolbar::updateCameraList(const QStringList& cameras) {
    m_cameraList = cameras;
    m_cameraCombo->clear();
    m_cameraCombo->addItems(cameras);
    
    if (cameras.isEmpty()) {
        m_cameraToggleBtn->setEnabled(false);
        m_cameraCombo->setEnabled(false);
    } else {
        m_cameraToggleBtn->setEnabled(true);
        m_cameraCombo->setEnabled(true);
    }
}

void MainToolbar::updateStreamingStatus(bool isStreaming, int fps, int bitrate) {
    updateStartStopButton(isStreaming);
    
    if (isStreaming) {
        QString status = QString("▶ %1fps | %2Mbps").arg(fps).arg(bitrate / 1000.0, 0, 'f', 1);
        if (m_recordCheck->isChecked()) {
            status += " | 录像中";
        }
        m_statusLabel->setText(status);
    } else {
        m_statusLabel->clear();
    }
}

void MainToolbar::showError(const QString& error) {
    m_statusLabel->setText("错误: " + error.left(20));
    m_statusLabel->setStyleSheet("color: #E81123; font-size: 11px;");
    
    // 3秒后恢复
    QTimer::singleShot(3000, this, [this]() {
        m_statusLabel->setStyleSheet("color: #CCCCCC; font-size: 11px;");
        m_statusLabel->clear();
    });
}

void MainToolbar::setRegionMode(int mode) {
    m_regionMode = mode;
    updateRegionLabel();
}

void MainToolbar::setCameraEnabled(bool enabled) {
    m_cameraToggleBtn->setChecked(enabled);
}

void MainToolbar::setCameraId(const QString& id) {
    int index = m_cameraList.indexOf(id);
    if (index >= 0) {
        m_cameraCombo->setCurrentIndex(index);
    }
}

void MainToolbar::setBitrate(int bitrate) {
    int index = 1; // 默认2 Mbps
    if (bitrate == 1000) index = 0;
    else if (bitrate == 2000) index = 1;
    else if (bitrate == 5000) index = 2;
    else if (bitrate == 8000) index = 3;
    else index = 4; // Custom
    
    m_bitrateCombo->setCurrentIndex(index);
}

void MainToolbar::setGopSeconds(int seconds) {
    int index = 1; // 默认2s
    if (seconds == 1) index = 0;
    else if (seconds == 2) index = 1;
    else if (seconds == 3) index = 2;
    else if (seconds == 5) index = 3;
    
    m_gopCombo->setCurrentIndex(index);
}

void MainToolbar::setProtocol(const QString& protocol) {
    bool isRtmp = (protocol.toLower() == "rtmp");
    m_protocolBtn->setChecked(isRtmp);
    m_protocolBtn->setText(protocol.toUpper());
}

void MainToolbar::setUrl(const QString& url) {
    m_urlEdit->setText(url);
}

void MainToolbar::setRecordEnabled(bool enabled) {
    m_recordCheck->setChecked(enabled);
}