#ifndef DRAWINGSCREEN_H
#define DRAWINGSCREEN_H

#include "UI/Screens/screen.h"
#include "UI/Components/drawingframe.h"
#include "UI/Components/timerwidget.h"

class DrawingScreen : public Screen {
    Q_OBJECT

    // Widgets
    TimerWidget *timer;
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
