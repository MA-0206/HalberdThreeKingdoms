#include "loadingwidget.h"

#include <QVBoxLayout>

LoadingWidget::LoadingWidget(QWidget *parent)
    : QWidget{parent},m_currentValue(0)
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    initComponent();
    initLoading();
}

void LoadingWidget::initComponent()
{
    // this->setStyleSheet("border-image:url(:/assets/images/ui/loading/progress) 0 0 0 0 stretch stretch");
    QLabel* bgLabel = new QLabel(this);
    QPixmap pixMap(":/assets/images/ui/loading/progress");
    bgLabel->setPixmap(pixMap);
    bgLabel->setScaledContents(true);

    QWidget* contentWidget = new QWidget(this);
    contentWidget->setStyleSheet("background: transparent;");

    QVBoxLayout* layout = new QVBoxLayout(contentWidget);
    layout->setAlignment(Qt::AlignCenter);

    QLabel* tipLabel = new QLabel("正在加载游戏资源，主公请稍候。。。", this);
    tipLabel->setStyleSheet("color: #28160c; font-size: 20px; "
                            " font-weight: bold; font-family: Microsoft YaHei;");
    tipLabel->setAlignment(Qt::AlignCenter);

    m_progressBar = new QProgressBar(this);
    m_progressBar->setFixedSize(400,30);
    m_progressBar->setRange(0,100);
    m_progressBar->setValue(0);
    m_progressBar->setTextVisible(true);
    m_progressBar->setStyleSheet(
                "QProgressBar{"
                    "border: 2px solid #555;"
                    "border-radius: 10px;"
                    "background-color: #222;"
                    "text-align: center;"
                    "color: white;"
                    "}"
                "QProgressBar::chunk {"
                    "background-color:#d4af37;"
                    "border-radius:8px;"
                    "}"
        );
    layout->addWidget(tipLabel);
    layout->addSpacing(20);
    layout->addWidget(m_progressBar, 0, Qt::AlignCenter);

    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(bgLabel, 0, 0);
    mainLayout->addWidget(contentWidget, 0, 0);
}

void LoadingWidget::initLoading()
{
    loadingTimer = new QTimer(this);
    connect(loadingTimer, &QTimer::timeout, this, [=](){
        m_currentValue += 10;
        m_progressBar->setValue(m_currentValue);
        if(m_currentValue >= 100)
        {
            loadingTimer->stop();
            emit loadingFinished();
        }
    });
}

void LoadingWidget::startLoading()
{
    m_currentValue = 0;
    m_progressBar->setValue(0);
    loadingTimer->start(30);
}