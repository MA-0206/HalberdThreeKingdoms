#ifndef ROLESELECTWIDGET_H
#define ROLESELECTWIDGET_H

#include <QLabel>
#include <QTimer>
#include "animator.h"

class RoleSelectWidget : public QLabel
{
    Q_OBJECT
public:
    explicit RoleSelectWidget(const QString& roleName, QWidget* parent = nullptr);
    ~RoleSelectWidget() = default;

    QString getRoleName() const {return m_roleName;}
    void setSelected();
    void setUnSelected();

signals:
    void clicked(const QString& roleName);

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    QString m_roleName;
    QTimer* m_animTimer;
    Animator m_animator;

    bool m_isHovered = false;
    bool m_isSelected = false;

    void initImage();
    void initAnimation();
    void playAnimation();
    void stopAnimation();
};

#endif // ROLESELECTWIDGET_H
