#include "roleselectwidget.h"
#include "jsonparser.h"
#include <QMouseEvent>

RoleSelectWidget::RoleSelectWidget(const QString& roleName, QWidget* parent)
    :QLabel(parent),m_roleName(roleName)
{
    initImage();
    initAnimation();
}

void RoleSelectWidget::initImage()
{
    setScaledContents(true);
    PortraitConfig config = JsonParser::getInstance().getPortraitConfig(m_roleName);
    qDebug()<< "config.staticPath:"<< config.staticPath;
    qDebug()<< "config.dynamicPath:"<< config.dynamicPath;
    qDebug()<< "config.totalFrames:"<< config.totalFrames;
    qDebug()<< "config.rows:"<< config.rows;
    qDebug()<< "config.cols:"<< config.cols;
    qDebug()<< "config.tickRate:"<< config.tickRate;
    m_animator.addAnimation("hover", QPixmap(config.dynamicPath), config.totalFrames,
                            config.rows, config.cols, config.tickRate);
    setPixmap(config.staticPath);
    setStyleSheet("border: 4px solid dimgray; border-radius:10px;");
}

void RoleSelectWidget::initAnimation()
{
    m_animTimer = new QTimer(this);
    connect(m_animTimer, &QTimer::timeout, this, [=](){
        if(m_animator.isFinshed()){
            m_animator.play("hover");
        }
        m_animator.update();
        QPixmap currentPix = m_animator.getCurrentPixmap();
        if(!currentPix.isNull()){
            setPixmap(currentPix);
        }
    });
}

void RoleSelectWidget::playAnimation()
{
    m_animator.play("hover");
    if(!m_animTimer->isActive()){
    m_animTimer->start(16);
    }
}

void RoleSelectWidget::stopAnimation()
{
    m_animTimer->stop();
    QString staticPath = ":/assets/images/ui/characters/" + m_roleName + "/static";
    setPixmap(staticPath);
}

void RoleSelectWidget::setSelected()
{
    m_isSelected = true;
    playAnimation();
    setStyleSheet("border: 4px solid gold; border-radius:10px;");
}

void RoleSelectWidget::setUnSelected()
{
    m_isSelected = false;
    setStyleSheet("border: 4px solid dimgray; border-radius:10px;");
    if(!m_isHovered)
    {
        stopAnimation();
    }
}

void RoleSelectWidget::enterEvent(QEnterEvent* event)
{
    m_isHovered = true;
    playAnimation();
    QLabel::enterEvent(event);
}

void RoleSelectWidget::leaveEvent(QEvent* event)
{
    m_isHovered = false;
    if(!m_isSelected){
        stopAnimation();
    }
    QLabel::leaveEvent(event);
}

void RoleSelectWidget::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        setSelected();
        emit(clicked(m_roleName));
    }
    QLabel::mousePressEvent(event);
}