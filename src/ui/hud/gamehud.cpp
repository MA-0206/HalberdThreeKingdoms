#include "gamehud.h"

GameHud::GameHud(QWidget* parent):QWidget(parent)
{
    this->resize(340,110);
}

void GameHud::updateStats(const EntityStats& newStats)
{
    m_stats = newStats;
    update();
}

void GameHud::setAvatar(const QString& imagePath)
{
    if(m_avatar.load(imagePath))
        update();
}

// Reissue the painting brush event
void GameHud::paintEvent(QPaintEvent* event)
{
    // No interaction of the screen
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    paintBackGround(&painter);
    paintAvatar(&painter);

    paintBarAndText(&painter, hpConfig, m_stats.runtime.hp, m_stats.base.maxHp);
    paintBarAndText(&painter, mpConfig, m_stats.runtime.mp, m_stats.base.maxMp);
    paintBarAndText(&painter, expConfig, m_stats.runtime.exp, m_stats.base.maxExp);

    paintLevelText(&painter);
}

// Render the background frame
void GameHud::paintBackGround(QPainter* painter)
{
    painter->setBrush(QColor(0,0,0,10));
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(0,0,260,75,5,5);
}

// Render the player's avatar
void GameHud::paintAvatar(QPainter* painter)
{
    int avatarX = 7;
    int avatarY = 7;
    int avatarSize = 60;

    // Draw the avatar frame and the avatar
    painter->setBrush(QColor(50,50,50));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(avatarX, avatarY, avatarSize, avatarSize);

    if(!m_avatar.isNull())
    {
        QPainterPath path;
        path.addEllipse(avatarX, avatarY, avatarSize, avatarSize);
        painter->setClipPath(path);

        painter->drawPixmap(avatarX, avatarY, avatarSize, avatarSize, m_avatar);
        painter->setClipping(false);
    }
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(109,83,16),2));
    painter->drawEllipse(avatarX, avatarY, avatarSize, avatarSize);

}

// Rendering level text
void  GameHud::paintLevelText(QPainter* painter)
{
    QFont font("Microsoft YaHei",10,QFont::Bold);
    painter->setPen(Qt::white);
    painter->setFont(font);
    painter->drawText(60,72,QString("Lv.%1").arg(m_stats.runtime.level));
}

// Rendering the magic bar, health bar, experience bar and their corresponding texts
void GameHud::paintBarAndText(QPainter* painter, const BarConfig& config, float currentVal, float maxVal)
{
    painter->setPen(Qt::white);
    painter->setBrush(config.bgColor);
    painter->drawRoundedRect(config.x, config.y, config.width, config.height, 3, 3);

    int valWidth = config.width * (currentVal/maxVal);
    if(valWidth > 0)
    {
        painter->setBrush(config.fillColor);
        painter->drawRoundedRect(config.x, config.y, valWidth, config.height, 3, 3);
    }

    QFont font("Microsoft YaHei",9,QFont::Bold);
    painter->setFont(font);
    QFontMetrics fm(font);

    QString text = QString("%1/%2").arg((int)currentVal).arg((int)maxVal);
    int textX = config.x + (config.width - fm.horizontalAdvance(text))/2;
    int textY = config.y + (config.height + fm.height())/2 - fm.descent();

    painter->setPen(Qt::black);
    painter->drawText(textX + 1, textY + 1, text);

    painter->setPen(Qt::white);
    painter->drawText(textX, textY, text);
}