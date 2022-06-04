#ifndef SELECTIONSCREEN_H
#define SELECTIONSCREEN_H

#include <QApplication>
#include <QMessageBox>

#include "screen.h"
#include "UI/Components/Widgets/customlabel.h"
#include "UI/Components/Widgets/custompushbutton.h"
#include "UI/Components/nickinputdialog.h"

class SelectionScreen : public Screen {
    Q_OBJECT

    // Widgets
    CustomLabel *label;
    QPushButton *createLobbyButton, *joinLobbyButton;
    QPushButton *backButton;

    void recalculateGeometry();
    void resizeEvent(QResizeEvent *event);

public:
    SelectionScreen(QWidget *parent);

public slots:
    void onCreateLobbyCklicked();
    void onJoinLobbyButtonClicked();
};

#endif // SELECTIONSCREEN_H
