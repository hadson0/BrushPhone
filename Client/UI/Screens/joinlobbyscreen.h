#ifndef JOINLOBBYSCREEN_H
#define JOINLOBBYSCREEN_H

#include <QMessageBox>

#include "screen.h"
#include "UI/Components/Widgets/customlabel.h"
#include "UI/Components/Widgets/custompushbutton.h"
#include "UI/Components/Widgets/customlineedit.h"
#include "UI/Components/nickinputdialog.h"

class JoinLobbyScreen : public Screen {
    Q_OBJECT

    // Widgets
    CustomLabel *label;
    CustomPushButton *joinButton, *backButton;
    CustomLineEdit *lobbyIDInput;

    void recalculateGeometry();
    void resizeEvent(QResizeEvent *event);

public:
    explicit JoinLobbyScreen(QWidget *parent = nullptr);

public slots:
    void onJoinButtonClicked();
};

#endif // JOINLOBBYSCREEN_H
