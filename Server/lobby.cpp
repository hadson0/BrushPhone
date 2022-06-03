#include "lobby.h"

Lobby::Lobby(QString lobbyID, QObject *parent)
    : QObject{parent} , lobbyID(lobbyID), isGameOn(false), game(nullptr) {}

QString Lobby::getID() { return lobbyID; }

// Returns a QStringList containing all the usersIDs
QStringList Lobby::getClientList() { return userMap.keys(); }

QString Lobby::getUserNick(QString clientID) { return userMap[clientID]->getNickname(); }

QStringList Lobby::getUsers() {
    QStringList userNicks;

    QMap<QString, User *>::iterator it = userMap.begin();
    for (; it != userMap.end(); it++) {
        userNicks.push_back(it.value()->getNickname());
    }

    return userNicks;
}

// Returns a QString containing the user nicknames. Ex.: hadson0,whoamI,user_1
QString Lobby::getUsersToStr() {
    QString userNicks;

    QMap<QString, User *>::iterator it = userMap.begin();
    for (; it != userMap.end(); it++) {
        userNicks.append(it.value()->getNickname() + ",");
    }
    userNicks.chop(1);

    return userNicks;
}

QString Lobby::getReadyUsersStr() {
    QString readyUserList;

    QMap<QString, User *>::iterator it = userMap.begin();
    for (; it != userMap.end(); it++) {
        if (it.value()->isReady()) { // If the user is ready
            readyUserList.append(it.value()->getNickname() + ",");
        }
    }
    readyUserList.chop(1);

    return readyUserList;
}

Round Lobby::getGameRound() {
    static int storyIndex = 0, roundIndex = 0;

    if (roundIndex == 5) {
        roundIndex = 0;
        storyIndex++;
    }

    return game->getRound(storyIndex, roundIndex);
}

void Lobby::toggleReady(QString clientID) {
    if (userMap.contains(clientID)) {
        userMap[clientID]->toggleReady();

        emit readyListChanged(getReadyUsersStr(), getClientList());
    }
    bool allReady = true;
    QMap<QString, User*>::iterator it = userMap.begin();
    for (; it != userMap.end(); it++) {
        if (!it.value()->isReady()) {
            allReady = false;
        }
    }

    bool isReadyToStart = allReady && !userMap.isEmpty()  && (userMap.size() % 2 == 0) && !isGameOn;

    if (isReadyToStart) {
        this->createGame();        
    }
}

// Registers the user if it isn't registered and resets all the ready status
void Lobby::addUser(QString clientID, QString nickname) {
    // Adds the user
    if (!userMap.contains(clientID)) {
        User *newUser = new User(nickname, this);
        userMap[clientID] = newUser;
    }

    // Set all the ready status to false
    for (const QString &user : getClientList()) {
        if (userMap[user]->isReady()) {
            userMap[user]->toggleReady();
        }
    }

    emit userListChanged(getUsersToStr(), getClientList());
    emit readyListChanged(getReadyUsersStr(), getClientList());
}

// Remove the user from the lobby and resets all the ready status
void Lobby::removeUser(QString clientID) {
    // removes the user
    if (userMap.contains(clientID)) {
        userMap.remove(clientID);
    }

    // Set all the ready status to false
    for (const QString &user : getClientList()) {
        if (userMap[user]->isReady()) {
            userMap[user]->toggleReady();
        }
    }

    emit userListChanged(getUsersToStr(), getClientList());
    emit readyListChanged(getReadyUsersStr(), getClientList());
}

bool Lobby::containsNickname(QString nickname) {
    QMap<QString, User*>::iterator it = userMap.begin();
    for (; it != userMap.end(); it++) {
        if (it.value()->getNickname() == nickname) {
            return true;
        }
    }
    return false;
}

void Lobby::onGameStarted() {
    emit gameStarted(this->getClientList());
}

void Lobby::onGameSentencePhase() {
    QStringList clientList = this->getClientList();
    for (const QString &client : clientList) {
        QString userNick = userMap[client]->getNickname();
        emit sentenceRequest(client, game->getDrawing(userNick));
    }
}

void Lobby::onGameDrawingPhase() {
    QStringList clientList = this->getClientList();
    for (const QString &client : clientList) {
        QString userNick = userMap[client]->getNickname();
        emit drawingRequest(client, game->getSentence(userNick));
    }
}

void Lobby::onGameEnded() {
    emit gameEnded(this->getUsersToStr(), this->getClientList());
}

void Lobby::setDrawingRequest(QString clientID, QString drawing) {
    emit setDrawing(userMap[clientID]->getNickname(), drawing);
}

void Lobby::setSentenceRequest(QString clientID, QString sentence) {
    emit setSentence(userMap[clientID]->getNickname(), sentence);
}

void Lobby::createGame() {
    // If the game doesn't exists, creates it
    if (game == nullptr) {
        game = new Game(this);
        connect(this, &Lobby::setSentence, game, &Game::setSentence);
        connect(this, &Lobby::setDrawing, game, &Game::setDrawing);

        connect(game, &Game::started, this, &Lobby::onGameStarted);
        connect(game, &Game::sentencePhase, this, &Lobby::onGameSentencePhase);
        connect(game, &Game::drawingPhase, this, &Lobby::onGameDrawingPhase);
        connect(game, &Game::ended, this, &Lobby::onGameEnded);

        game->startGame(this->getUsers());

        qDebug() << "Game started, Lobby ID: " + lobbyID;
    }
}

