#ifndef LOBBYSCREEN_H
#define LOBBYSCREEN_H

#include <QMessageBox>

#include "screen.h"
#include "UI/Components/Frames/userlistview.h"
#include "UI/Components/Frames/chatframe.h"
#include "UI/Components/Widgets/customlabel.h"
#include "UI/Components/Widgets/custompushbutton.h"
#include "UI/Components/Frames/timerframe.h"
#include "UI/Components/Frames/sentenceframe.h"

class LobbyScreen : public Screen {
    Q_OBJECT

    // Custom frames
    UserListView *userListView;
    ChatFrame *chatFrame;

    // Widgets
    CustomPushButton *backButton, *readyButton;
    CustomLabel *lobbyIDLabel;

    QString lobbyID;    
    bool isStartLobby;

    void recalculateGeometry();
    void resizeEvent(QResizeEvent *event);

public:
    explicit LobbyScreen(QString lobbyID, QWidget *parent);
    ~LobbyScreen();

public slots:
    void requestSendLobbyMessage(QString message);
    void requestToggleReady();
    void onBackButtonClicked();

signals:
    void setReady(QString clientID, bool ready);
    void newMessageRecieved(QString message, QString senderNick);
    void userListChanged(QStringList newUserLsit);
    void readyListChanged(QStringList newReadyList);
};

#endif // LOBBYSCREEN_H
