#ifndef GAMESENTENCESCREEN_H
#define GAMESENTENCESCREEN_H

#include "UI/Screens/screen.h"
#include "UI/Components/gamesentenceframe.h"
#include "UI/Components/timerwidget.h"

class GameSentenceScreen : public Screen {
    Q_OBJECT

    // Frames
    GameSentenceFrame *gameSentence;

    // Widgets
    TimerWidget *timer;

    void resizeEvent(QResizeEvent *event);

public:
    explicit GameSentenceScreen(QString drawingData, QWidget *parent = nullptr);

public slots:

signals:
    void done();
    void sendSentence(QString sentence);
};

#endif // GAMESENTENCESCREEN_H
