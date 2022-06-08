#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <random>
#include <QDebug>
#include <QMap>

#include "websockethandler.h"
#include "messageprocesshandler.h"
#include "lobby.h"

class ServerManager : public QObject {
    Q_OBJECT

    QMap<QString, Lobby*> lobbyMap;
    QMap<QString, Lobby*> clientLobbyMap;

    WebSocketHandler *webSocketHandler;
    MessageProcessHandler *messageProcessorHandler;

    ServerManager(QObject *parent = nullptr);

public:
    ServerManager(const ServerManager &obj) = delete;
    ServerManager(ServerManager &&obj) = delete;
    ServerManager operator=(ServerManager &obj) = delete;
    ServerManager operator=(ServerManager &&obj) = delete;

    static ServerManager& getInstance() {
        static ServerManager instance;
        return instance;
    }

    QString generateRandomID();
    void deleteLobby(QString lobbyID);

public slots:
    void createLobbyRequest(QString clientID, QString nickname);
    void joinLobbyRequest(QString lobbyID, QString clientID, QString nickname);    
    void quitLobbyRequest(QString lobbyID, QString clientID);
    void lobbyMessageRequest(QString message, QString lobbyID, QString senderID);
    void toggleReadyRequest(QString lobbyID, QString clintID);

    void onUserListChanged(QString users, QStringList &clientList);
    void onReadyListChanged(QString readyUSers, QStringList &clientList);

    void onGameStarted(QStringList &clientList);
    void onSentenceRequest(QString clientID, QString drawing);
    void onDrawingRequest(QString clientID, QString sentence);
    void onDisplayRoundRequest(QString sentence, QString drawing, QStringList &clientList);
    void onFinalLobby(QString users, QStringList &clientList);

    void onClientDisconnected(QString clientID);

signals:
    void setDrawingRequest(QString clientID, QString drawing);
    void setSentenceRequest(QString clientID, QString sentence);
    void getRoundRequest(QString clientID);
};

#endif // SERVERMANAGER_H
