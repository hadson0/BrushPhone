#ifndef TIMERFRAME_H
#define TIMERFRAME_H

#include <QWidget>
#include <QTimer>
#include <QTime>

#include "UI/Components/Frames/backgroundedframe.h"
#include "UI/Components/Widgets/customlabel.h"

class TimerFrame : public BackgroundedFrame {
    Q_OBJECT

    QTimer *timer;
    CustomLabel *label;
    int seconds;

    void resizeEvent(QResizeEvent *event);

public:
    explicit TimerFrame(int seconds, QWidget *parent = nullptr);

    void start();

public slots:
    void updateTimer();

signals:
    void isOver();
};

#endif // TIMERFRAME_H
