#include "lobby.h"

Lobby::Lobby(QString lobbyID, QObject *parent)
    : QObject{parent} , lobbyID(lobbyID), isGameOn(false), game(nullptr), storyIndex(0), roundIndex(0) {}

QString Lobby::getID() const { return lobbyID; }

// Returns a QStringList containing all the usersIDs
QStringList Lobby::getClientList() { return userMap.keys(); }

// Gets the next round index, in cycles
void Lobby::getNextIndex(int &x) {
    if (x == (userMap.size() / 2) - 1) {
        x = 0;
    } else {
        x++;
    }
}

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

void Lobby::toggleReady(QString clientID) {
    if (userMap.contains(clientID)) {
        userMap[clientID]->toggleReady();

        QStringList clientList = this->getClientList();
        emit readyListChanged(getReadyUsersStr(), clientList);
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

    QStringList clientList = getClientList();
    emit userListChanged(getUsersToStr(), clientList);
    emit readyListChanged(getReadyUsersStr(), clientList);
}

// Remove the user from the lobby and resets all the ready status
void Lobby::removeUser(QString clientID) {
    // removes the user
    if (userMap.contains(clientID)) {
        userMap.remove(clientID);
    }

    // Set all the ready status to false
     QStringList clientList = getClientList();
    for (const QString &user : clientList) {
        if (userMap[user]->isReady()) {
            userMap[user]->toggleReady();
        }
    }

    emit userListChanged(getUsersToStr(), clientList);
    emit readyListChanged(getReadyUsersStr(), clientList);
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
    QStringList clientList = this->getClientList();
    emit gameStarted(clientList);
}

void Lobby::onSentencePhase() {
    QStringList clientList = this->getClientList();
    for (const QString &client : clientList) {
        QString userNick = userMap[client]->getNickname();
        emit sentenceRequest(client, game->getDrawing(userNick));
    }
}

void Lobby::onDrawingPhase() {
    QStringList clientList = this->getClientList();
    for (const QString &client : clientList) {
        QString userNick = userMap[client]->getNickname();
        emit drawingRequest(client, game->getSentence(userNick));
    }
}

void Lobby::getGameRound() {
    QStringList clientList = this->getClientList();
    if (storyIndex < userMap.size()) {
        Round round = game->getRound(storyIndex, roundIndex);
        emit displayRoundRequest(round.first, round.second, clientList);
    } else {
        emit finalLobby(this->getUsersToStr(), clientList);
    }
    getNextIndex(roundIndex);

    if (roundIndex == 0) {
        storyIndex ++;
    }
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
        connect(game, &Game::sentencePhase, this, &Lobby::onSentencePhase);
        connect(game, &Game::drawingPhase, this, &Lobby::onDrawingPhase);
        connect(game, &Game::ended, this, &Lobby::getGameRound);

        QStringList userList = this->getUsers();
        game->startGame(userList);

        qDebug() << "Game started, Lobby ID: " + lobbyID;
    }
}

