#ifndef DRAWINGSCREEN_H
#define DRAWINGSCREEN_H

#include "UI/Screens/screen.h"
#include "UI/Components/Frames/drawingframe.h"
#include "UI/Components/Frames/timerframe.h"

class DrawingScreen : public Screen {
    Q_OBJECT

    // Widgets
    TimerFrame *timer;
    DrawingFrame *gameDrawing;

    void recalculateGeometry();
    void resizeEvent(QResizeEvent *event);

public:
    explicit DrawingScreen(QString sentence, QWidget *parent = nullptr);

public slots:

signals:
    void sendDrawing(QString drawingData);
};

#endif // DRAWINGSCREEN_H
