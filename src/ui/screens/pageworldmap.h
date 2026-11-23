#ifndef PAGEWORLDMAP_H
#define PAGEWORLDMAP_H

#include <QWidget>
#include "pageselectrole.h"

namespace Ui {
class PageWorldMap;
}

class PageWorldMap : public QWidget
{
    Q_OBJECT

public:
    explicit PageWorldMap(QWidget *parent = nullptr);
    ~PageWorldMap();

    void setSelectedRole(const QString& role){m_currentRole = role;}

private:
    Ui::PageWorldMap *ui;

private:
    QString m_currentRole;
    void selectLevel();
    void returnMainMenu();

signals:
    void requestEnterLevel(const QString& roleName, const QString& levelId);
    void requestReturnToMenu();
};

#endif // PAGEWORLDMAP_H
