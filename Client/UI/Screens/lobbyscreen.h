#ifndef LOBBYSCREEN_H
#define LOBBYSCREEN_H

#include <QMessageBox>

#include "screen.h"
#include "UI/Components/userlistview.h"
#include "UI/Components/chatframe.h"
#include "UI/Components/customlabel.h"
#include "UI/Components/custompushbutton.h"
#include "UI/Components/timerwidget.h"
#include "UI/Components/gamesentenceframe.h"

class LobbyScreen : public Screen {
    Q_OBJECT

    // Custom frames
    UserListView *userListView;
    ChatFrame *chatFrame;
    GameSentenceFrame *gameSentenceFrame;

    // Widgets
    CustomPushButton *backButton, *readyButton;
    CustomLabel *lobbyIDLabel;
    TimerWidget *timer;

    QString lobbyID;

    void resizeEvent(QResizeEvent *event);

public:
    explicit LobbyScreen(QString lobbyID, QWidget *parent);

    enum GamePhases{LobbyPhase = 0, SentencePhase = 1, DrawingPhase = 2};
    static GamePhases GamePhase;

    void setGamePhase(GamePhases phase);

public slots:
    void requestSendLobbyMessage(QString message);
    void requestToggleReady();
    void onBackButtonClicked();

signals:
    void setReady(QString clientID, bool ready);
    void newMessageRecieved(QString message, QString senderNick);
    void userListChanged(QStringList newUserLsit);
    void readyListChanged(QStringList newReadyList);

    void onSentenceReceived(QString sentence);
};

#endif // LOBBYSCREEN_H
