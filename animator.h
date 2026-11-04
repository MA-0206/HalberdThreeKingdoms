#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <QPixmap>
#include <QString>
#include <QMap>

struct AnimationData {
    QPixmap sheet;
    int totalFrames;
    int rows;
    int cols;
    int frameWidth;
    int frameHeight;
    int tickRate;
};

class Animator
{
public:
    Animator();

    void addAnimation(const QString& name, const QPixmap& sheet, int totalFrames, int rows,
                      int cols, int tickRate);
    void play(const QString& name);
    void update();
    QPixmap getCurrentPixmap()const;
    bool isFinshed() const;
    int getFrameWidth() const;
    int getFrameHeight() const;

private:
    QMap<QString,AnimationData>animations;

    QString currentAnimation;
    int currentFrame;
    int timer;
    bool finshed;
};

#endif // ANIMATOR_H
