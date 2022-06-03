#include "timerwidget.h"

TimerWidget::TimerWidget(int seconds, QWidget *parent)
    : BackgroundedFrame{parent}, seconds(seconds) {
    setPadding(10);

    // Label
    label = new CustomLabel(QString::number(seconds) + "s", this);

    // Timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TimerWidget::updateTimer);
}

void TimerWidget::start() {
    timer->stop();
    timer->start(1000);
}

void TimerWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);

    // Label
    int labelWidth = this->getAvaliableWidth(), labelHeight = this->getAvaliableHeight();
    int labelX = this->getPadding(), labelY = this->getPadding();
    label->setGeometry(labelX, labelY, labelWidth, labelHeight);
}

void TimerWidget::updateTimer() {
    if (seconds != 0) {
        seconds--;
        label->setText(QString::number(seconds) + "s");
    } else {
        timer->stop();
        emit isOver();
    }
}
