#ifndef TIMERWIDGET_H
#define TIMERWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QTime>

#include "backgroundedframe.h"
#include "UI/Components/customlabel.h"

class TimerWidget : public BackgroundedFrame {
    Q_OBJECT

    QTimer *timer;
    CustomLabel *label;
    int seconds;

    void resizeEvent(QResizeEvent *event);

public:
    explicit TimerWidget(int seconds, QWidget *parent = nullptr);

public slots:
    void updateTimer();

signals:
    void isOver();
};

#endif // TIMERWIDGET_H