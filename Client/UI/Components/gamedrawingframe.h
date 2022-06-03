#ifndef GAMEDRAWINGFRAME_H
#define GAMEDRAWINGFRAME_H

#include <QInputDialog>
#include <QColorDialog>

#include "backgroundedframe.h"
#include "UI/Components/custompushbutton.h"
#include "UI/Components/customlabel.h"
#include "UI/Components/canvas.h"

class GameDrawingFrame : public BackgroundedFrame {
    Q_OBJECT

    // Widgets
    CustomLabel *label;
    CustomPushButton *doneButton, *clearButton;
    CustomPushButton *widthButton, *colorButton;
    Canvas *canvas;

    void resizeEvent(QResizeEvent *event);

public:
    explicit GameDrawingFrame(QString sentence, QWidget *parent = nullptr);

public slots:
    void onDoneButtonClicked();
    void setPenWidth();
    void setPenColor();

signals:
    void sendDrawing(QString drawingData);
};

#endif // GAMEDRAWINGFRAME_H
