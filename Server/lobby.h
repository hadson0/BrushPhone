#ifndef LOBBY_H
#define LOBBY_H

#include <QObject>
#include <QMap>
#include <QDebug>

#include "user.h"

class Lobby : public QObject {
    Q_OBJECT

    QString lobbyID;
    QMap<QString, User *> userMap; // <clientID, User>

public:
    explicit Lobby(QString lobbyID, QObject *parent);

    enum GamePhases{LobbyPhase = 0, SentencePhase = 1, DrawingPhase = 2};
    static GamePhases GamePhase;

    // Getters
    QString getID();
    QStringList getClientList();
    QString getUserNick(QString clientID);
    QString getUsersToStr();
    QString getReadyUsersStr();

    // Setters
    void toggleReady(QString clientID);
    void setGamePhase(GamePhases phase);

    //Methods
    void addUser(QString clientID, QString nickname);
    void removeUser(QString clientID);
    bool containsNickname(QString nickname);

private:
    GamePhases phase;

signals:
    void readyListChanged(QString readyUsers, QStringList clientList);
    void userListChanged(QString users, QStringList clientList);
    void allReady();
};

#endif // LOBBY_H
