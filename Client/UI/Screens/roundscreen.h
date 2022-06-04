#ifndef ROUNDSCREEN_H
#define ROUNDSCREEN_H

#include "UI/Screens/screen.h"
#include "UI/Components/roundframe.h"
#include "UI/Components/custompushbutton.h"

class RoundScreen : public Screen {
    Q_OBJECT

    // Widgets
    CustomPushButton *nextButton;

    // Frame
    RoundFrame *displayFrame;

    void recalculateGeometry();
    void resizeEvent(QResizeEvent *event);

public:
    explicit RoundScreen(QString sentence, QString drawingData, QWidget *parent = nullptr);

signals:
    void nextRound();
};

#endif // ROUNDSCREEN_H
