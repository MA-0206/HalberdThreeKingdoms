#include "animator.h"
#include <QDebug>

Animator::Animator():currentFrame(0),timer(0),finshed(false) {}


void Animator::addAnimation(const QString& name, const QPixmap& sheet, int totalFrames, int rows,
                            int cols, int tickRate)
{
    AnimationData data;
    data.sheet = sheet;
    data.totalFrames = totalFrames;
    data.rows = rows;
    data.cols = cols;
    data.frameWidth = sheet.width()/cols;
    data.frameHeight = sheet.height()/rows;
    data.tickRate = tickRate;
    animations[name] = data;

    // qDebug()<<"sheet:"<<sheet;
    // qDebug()<<"totalFrames:"<<totalFrames;
    // qDebug()<<"rows:"<<rows;
    // qDebug()<<"cols:"<<cols;
    // qDebug()<<"frameWidth:"<<sheet.width()/cols;
    // qDebug()<<"frameHeight:"<<sheet.height()/rows;
    // qDebug()<<"tickRate:"<<tickRate;
    // qDebug()<<"sheet:"<<sheet;
}

// Play the animation
void Animator::play(const QString& name)
{
    if(currentAnimation == name && !finshed)
    {
        return;
    }

    if(animations.contains(name))
    {
        currentAnimation = name;
        currentFrame = 0;
        timer = 0;
        finshed = false;
    }
}

void Animator::update()
{
    if(currentAnimation.isNull() || finshed)
    {
        qDebug()<<"current animation is null or the animaton is play finshed";
        return;
    }

    const AnimationData& data = animations[currentAnimation];
    timer++;
    if(timer >= data.tickRate)
    {
        timer = 0;
        currentFrame++;
        if(currentFrame >= data.totalFrames)
        {
            currentFrame = 0;
            finshed = true;
        }
    }
}

// Obtain the original sprite image
QPixmap Animator::getCurrentPixmap()const
{
    if(currentAnimation.isEmpty() || !animations.contains(currentAnimation))
    {
        qDebug()<<"have no this animation";
        return QPixmap();
    }
    const AnimationData& data = animations[currentAnimation];

    int realCol = currentFrame%data.cols;
    int realRow = currentFrame/data.cols;

    return data.sheet.copy(realCol*data.frameWidth ,realRow*data.frameHeight, data.frameWidth, data.frameHeight);

}

int Animator::getFrameWidth() const
{
    if(currentAnimation.isEmpty() || !animations.contains(currentAnimation))
    {
        qDebug()<<"have no the getFrameWidth";
        return 0;
    }
    return animations[currentAnimation].frameWidth;
}

int Animator::getFrameHeight() const
{
    if(currentAnimation.isEmpty() || !animations.contains(currentAnimation))
    {
        qDebug()<<"have no the getFrameHeight";
        return 0;
    }
    return animations[currentAnimation].frameHeight;
}

int Animator::getCurrentFrame() const
{
    if(currentAnimation.isEmpty() || !animations.contains(currentAnimation))
    {
        qDebug()<<"have no the getCurrentFrame";
    }
    return currentFrame;
}

bool Animator::isFinshed() const
{
    return finshed;
}