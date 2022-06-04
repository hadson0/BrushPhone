#ifndef DRAWINGFRAME_H
#define DRAWINGFRAME_H

#include <QInputDialog>
#include <QColorDialog>

#include "backgroundframe.h"
#include "UI/Components/custompushbutton.h"
#include "UI/Components/customlabel.h"
#include "UI/Components/canvas.h"

class DrawingFrame : public BackgroundFrame {
    Q_OBJECT

    // Widgets
    CustomLabel *label;
    CustomPushButton *doneButton, *clearButton;
    CustomPushButton *widthButton, *colorButton;
    Canvas *canvas;

    void resizeEvent(QResizeEvent *event);

public:
    explicit DrawingFrame(QString sentence, QWidget *parent = nullptr);

public slots:
    void onDoneButtonClicked();
    void setPenWidth();
    void setPenColor();

signals:
    void sendDrawing(QString drawingData);
};

#endif // DRAWINGFRAME_H
