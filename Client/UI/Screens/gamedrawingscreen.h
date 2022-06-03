#ifndef GAMEDRAWINGSCREEN_H
#define GAMEDRAWINGSCREEN_H

#include "UI/Screens/screen.h"
#include "UI/Components/gamedrawingframe.h"
#include "UI/Components/timerwidget.h"

class GameDrawingScreen : public Screen {
    Q_OBJECT

    // Widgets
    TimerWidget *timer;
    GameDrawingFrame *gameDrawing;

    void resizeEvent(QResizeEvent *event);

public:
    explicit GameDrawingScreen(QString sentence, QWidget *parent = nullptr);

public slots:

signals:
    void sendDrawing(QString drawingData);
};

#endif // GAMEDRAWINGSCREEN_H
