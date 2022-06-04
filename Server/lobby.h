#ifndef LOBBY_H
#define LOBBY_H

#include <QObject>
#include <QMap>
#include <QPair>
#include <QDebug>

#include "user.h"
#include "game.h"

typedef QPair<QString, QString> Round; // <sentence, drawing>

class Lobby : public QObject {
    Q_OBJECT

    QString lobbyID;
    QMap<QString, User *> userMap; // <clientID, User>

    bool isGameOn;
    Game *game;

    int storyIndex, roundIndex;
    void getNextIndex(int &x);

public:
    explicit Lobby(QString lobbyID, QObject *parent);

    // Getters
    QString getID() const;
    QStringList getClientList();
    QString getUserNick(QString clientID);
    QStringList getUsers();
    QString getUsersToStr();
    QString getReadyUsersStr();

    // Setters
    void toggleReady(QString clientID);    

    //Methods
    void addUser(QString clientID, QString nickname);
    void removeUser(QString clientID);
    bool containsNickname(QString nickname);

public slots:
    void onGameStarted();
    void onSentencePhase();
    void onDrawingPhase();
    void getGameRound();

    void setDrawingRequest(QString clientID, QString drawing);
    void setSentenceRequest(QString clientID, QString sentence);

private:
    void createGame();

signals:
    void readyListChanged(QString readyUsers, QStringList &clientList);
    void userListChanged(QString users, QStringList &clientList);

    void gameStarted(QStringList &clientList);

    void sentenceRequest(QString clientID, QString drawing);
    void drawingRequest(QString clientID, QString sentence);

    void setSentence(QString userNick, QString sentence);
    void setDrawing(QString userNick, QString drawingData);

    void displayRoundRequest(QString sentence, QString drawing, QStringList &clientList);
    void finalLobby(QString userList, QStringList &clientList);
};

#endif // LOBBY_H
