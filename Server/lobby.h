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

public:
    explicit Lobby(QString lobbyID, QObject *parent);

    // Getters
    QString getID();
    QStringList getClientList();
    QString getUserNick(QString clientID);
    QStringList getUsers();
    QString getUsersToStr();
    QString getReadyUsersStr();

    Round getGameRound();

    // Setters
    void toggleReady(QString clientID);    

    //Methods
    void addUser(QString clientID, QString nickname);
    void removeUser(QString clientID);
    bool containsNickname(QString nickname);

public slots:
    void onGameStarted();
    void onGameSentencePhase();
    void onGameDrawingPhase();
    void onGameEnded();

    void setDrawingRequest(QString clientID, QString drawing);
    void setSentenceRequest(QString clientID, QString sentence);

private:
    void createGame();

signals:
    void readyListChanged(QString readyUsers, QStringList clientList);
    void userListChanged(QString users, QStringList clientList);

    void gameStarted(QStringList clientList);

    void sentenceRequest(QString clientID, QString drawing);
    void drawingRequest(QString clientID, QString sentence);

    void gameEnded(QString users, QStringList clientList);

    void setSentence(QString userNick, QString sentence);
    void setDrawing(QString userNick, QString drawing);
};

#endif // LOBBY_H
