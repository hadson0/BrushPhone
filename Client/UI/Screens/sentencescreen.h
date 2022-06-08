#ifndef SENTENCESCREEN_H
#define SENTENCESCREEN_H

#include "UI/Screens/screen.h"
#include "UI/Components/Frames/sentenceframe.h"
#include "UI/Components/Frames/timerframe.h"

class SentenceScreen : public Screen {
    Q_OBJECT

    // Frames
    SentenceFrame *gameSentence;

    // Widgets
    TimerFrame *timer;

    void recalculateGeometry();
    void resizeEvent(QResizeEvent *event);

public:
    explicit SentenceScreen(QString drawingData, QWidget *parent = nullptr);

public slots:

signals:
    void done();
    void sendSentence(QString sentence);
};

#endif // SENTENCESCREEN_H
