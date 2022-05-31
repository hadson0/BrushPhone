#include "clientmanager.h"

ClientManager *ClientManager::instance = nullptr;

ClientManager::ClientManager(QObject *parent)
    : QObject{parent}, clientID(""), lobbyID("") {
    messageProcessHandler = new MessageProcessHandler(this);

    // Socket related message process handler connections
    connect(this, &ClientManager::processSocketMessage, messageProcessHandler, &MessageProcessHandler::processSocketMessage);
    connect(messageProcessHandler, &MessageProcessHandler::setClientID, this, &ClientManager::setClientID);
    connect(messageProcessHandler, &MessageProcessHandler::newLobby, this, &ClientManager::onLobbyJoined);
    connect(messageProcessHandler, &MessageProcessHandler::userListUpdated, this, &ClientManager::userListChanged);
    connect(messageProcessHandler, &MessageProcessHandler::readyListUpdated, this, &ClientManager::readyListChanged);
    connect(messageProcessHandler, &MessageProcessHandler::newLobbyMessageRecieved, this, &ClientManager::newLobbyMessageRecieved);

    // Screen related message process handler connections
    connect(this, &ClientManager::processScreenMessage, messageProcessHandler, &MessageProcessHandler::processScreenMessage);
    connect(messageProcessHandler, &MessageProcessHandler::connectToServerRequest, this, &ClientManager::connectToServerRequest);
    connect(messageProcessHandler, &MessageProcessHandler::createLobbyRequest, this, &ClientManager::createLobbyRequest);
    connect(messageProcessHandler, &MessageProcessHandler::joinLobbyRequest, this, &ClientManager::joinLobbyRequested);
    connect(messageProcessHandler, &MessageProcessHandler::quitLobbyRequest, this, &ClientManager::quitLobbyRequest);
    connect(messageProcessHandler, &MessageProcessHandler::sendLobbyMessageRequest, this, &ClientManager::sendLobbyMessageRequested);
    connect(messageProcessHandler, &MessageProcessHandler::toggleReadyRequest, this, &ClientManager::toggleReadyRequest);

    connect(messageProcessHandler, &MessageProcessHandler::error, this, &ClientManager::error);

    readNickname();
}

ClientManager::~ClientManager() {
    writeNickname();
}


void ClientManager::readNickname() {
    QFile file("nickname.txt");

    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        QString text = in.readAll();

        nickname = text;

        file.close();
    }
}

void ClientManager::writeNickname() {
    QFile file("nickname.txt");
    if(file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << nickname;
        file.flush();
        file.close();
    }
}

QString ClientManager::getLobbyID() { return lobbyID; }


void ClientManager::setClientID(QString clientID) {
    this->clientID = clientID;
    emit clientConnected();
}

void ClientManager::setLobbyID(QString newLobbyID) {
    // This if statement prevents an unecessary signal emission
    if (lobbyID != newLobbyID) {
        lobbyID = newLobbyID;

        if (lobbyID.isEmpty()) {
            emit lobbyLeft();
        } else {
            emit lobbyIDChanged(newLobbyID);
        }
    }
}

void ClientManager::toggleReadyRequest() {
    emit newMessageReadyToSend("type:toggleReady;payLoad:0;lobbyID:" + lobbyID + ";senderID:" + clientID);
}

void ClientManager::createLobbyRequest() {
    QString newNickname = NicknameInputDialog::getNickname(nickname);

    qDebug() << nickname << " " << newNickname;

    if (newNickname.isEmpty()) {
        emit error("blankNickError");
    } else if (newNickname != "*CLOSED") {
        emit newMessageReadyToSend("type:createGame;payLoad:0;senderID:" + clientID + ";nickname:" + newNickname);

        if (nickname != newNickname) {
            nickname = newNickname;
            writeNickname();
        }
    }
}

void ClientManager::joinLobbyRequested(QString targetLobbyID) {
    QString newNickname = NicknameInputDialog::getNickname(nickname);

    if (newNickname.isEmpty()) {
        emit error("blankNickError");
    } else if (newNickname[0] != '*') {
        emit newMessageReadyToSend("type:joinGame;payLoad:" + targetLobbyID + ";senderID:" + clientID + ";nickname:" + newNickname);

        if (nickname != newNickname) {
            nickname = newNickname;
            writeNickname();
        }
    }
}

void ClientManager::sendLobbyMessageRequested(QString message) {
    emit newMessageReadyToSend("type:message;payLoad:" + message + ";lobbyID:" + lobbyID + ";senderID:" + clientID);
}

void ClientManager::quitLobbyRequest() {
    emit newMessageReadyToSend("type:quitLobbyRequest;payLoad:0;lobbyID:" + lobbyID + ";senderID:" + clientID);
}

void ClientManager::onLobbyJoined(QString lobbyID, QStringList newUserList) {
    setLobbyID(lobbyID);
    emit userListChanged(newUserList);
    emit joinedLobby();
}

void ClientManager::leaveLobby() {
    lobbyID = "";
}
