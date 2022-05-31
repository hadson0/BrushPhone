#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>

#include "Controllers/messageprocesshandler.h"
#include "UI/Components/nicknameinputdialog.h"

class ClientManager : public QObject {
    Q_OBJECT

    QString clientID; // Lobby host ID (who created the lobby)
    QString lobbyID, nickname;

    MessageProcessHandler *messageProcessHandler;

    static ClientManager *instance;
    ClientManager(QObject *parent = nullptr);

    void readNickname();
    void writeNickname();

public:
    ClientManager(const ClientManager &obj) = delete;
    ClientManager(const ClientManager &&obj) = delete;
    ClientManager operator=(ClientManager &obj) = delete;
    ClientManager operator=(ClientManager &&obj) = delete;
    ~ClientManager();

    static ClientManager *getInstance(QObject *parent = nullptr) {
        if (instance == nullptr) {
            instance = new ClientManager(parent);
        }

        return instance;
    }

    // Getters
    QString getLobbyID();

public slots:    
    // Setters
    void setClientID(QString clientID);
    void setLobbyID(QString lobbyID);

    // Methods   
    void toggleReadyRequest();
    void createLobbyRequest();
    void joinLobbyRequested(QString lobbyID);
    void quitLobbyRequest();
    void sendLobbyMessageRequested(QString message);
    void onLobbyJoined(QString lobbyID, QStringList userList);
    void leaveLobby();

signals:
    void processSocketMessage(QString message);
    void processScreenMessage(QString message);

    void connectToServerRequest();
    void clientConnected();
    void userListChanged(QStringList newUserList);
    void readyListChanged(QStringList readyUsers);
    void lobbyIDChanged(QString newLobbyID);
    void lobbyLeft();

    void newLobbyMessageRecieved(QString message, QString senderNick);
    void newMessageReadyToSend(QString message);
    void joinedLobby();

    void error(QString error);    
};

#endif // CLIENTMANAGER_H
