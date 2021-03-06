#ifndef MAINMENUSCREEN_H
#define MAINMENUSCREEN_H

#include <QLabel>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "screen.h"
#include "UI/Components/Widgets/customlabel.h"
#include "UI/Components/Widgets/custompushbutton.h"
#include "UI/Components/Widgets/customlineedit.h"

class MainMenuScreen : public Screen {
    Q_OBJECT

    // Widgets
    CustomLabel *label;
    CustomPushButton *playButton, *quitButton;

    void recalculateGeometry();
    void resizeEvent(QResizeEvent *event);

public:
    MainMenuScreen(QWidget *parent);

signals:
    void connectToTheServerRequest();
};

#endif // MAINMENUSCREEN_H
