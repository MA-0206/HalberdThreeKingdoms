#ifndef GAMEHUD_H
#define GAMEHUD_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include "entity.h"

struct BarConfig{
    int x ,y, width,height;
    QColor bgColor;
    QColor fillColor;
};

class GameHud : public QWidget
{
    Q_OBJECT
public:
    GameHud(QWidget* parent = nullptr);

    void updateStats(const EntityStats& newStats);
    void setAvatar(const QString& imagePath);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    EntityStats m_stats;
    QPixmap m_avatar;

    void paintBackGround(QPainter* painter);
    void paintAvatar(QPainter* painter);
    void paintLevelText(QPainter*  painter);
    void paintBarAndText(QPainter* painter, const BarConfig& config, float currentVal, float maxVal);

    BarConfig hpConfig   = { 70, 15, 180, 15, Qt::darkRed, Qt::red };
    BarConfig mpConfig = {70, 32, 180, 15, Qt::darkBlue, QColor(30, 144, 255)};
    BarConfig expConfig = {70, 50, 180, 10, Qt::darkGray, Qt::yellow};
};

#endif // GAMEHUD_H
