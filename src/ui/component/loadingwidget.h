#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QProgressBar>
#include <QLabel>

class LoadingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoadingWidget(QWidget *parent = nullptr);

    void startLoading();

signals:
    void loadingFinished();

private:
    QTimer* loadingTimer;
    QProgressBar* m_progressBar;
    int m_currentValue;

    void initComponent();
    void initLoading();
};

#endif // LOADINGWIDGET_H
