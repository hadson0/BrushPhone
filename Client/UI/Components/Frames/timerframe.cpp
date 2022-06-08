#include "timerframe.h"

TimerFrame::TimerFrame(int seconds, QWidget *parent)
    : BackgroundedFrame{parent}, seconds(seconds) {
    setPadding(10);

    // Label
    label = new CustomLabel(QString::number(seconds) + "s", this);

    // Timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TimerFrame::updateTimer);
}

void TimerFrame::start() {
    timer->stop();
    timer->start(1000);
}

void TimerFrame::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);

    // Label
    int labelWidth = this->getAvaliableWidth(), labelHeight = this->getAvaliableHeight();
    int labelX = this->getPadding(), labelY = this->getPadding();
    label->setGeometry(labelX, labelY, labelWidth, labelHeight);
}

void TimerFrame::updateTimer() {
    if (seconds != 0) {
        seconds--;
        label->setText(QString::number(seconds) + "s");
    } else {
        timer->stop();
        emit isOver();
    }
}
