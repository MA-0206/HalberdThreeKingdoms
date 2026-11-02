#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPaintEvent>
#include <memory>
#include "gameview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public:
    static const int WINDOW_WIDTH = 1280;
    static const int WINDOW_HEIGHT = 720;

protected:
    void paintEvent(QPaintEvent *event)override;

private slots:
    void on_pushButtonStartGame_clicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<GameView> m_gameview;
};
#endif // MAINWINDOW_H
