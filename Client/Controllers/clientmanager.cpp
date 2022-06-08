#include "clientmanager.h"

ClientManager *ClientManager::instance = nullptr;

ClientManager::ClientManager(QObject *parent)
    : QObject{parent}, clientID(""), lobbyID(""), nickname("") {
    messageProcessHandler = new MessageProcessHandler(this);

    // Web Socket Handler
    webSocketHandler = WebSocketHandler::getInstance(this);
    connect(this, &ClientManager::newMessageReadyToSend, webSocketHandler, &WebSocketHandler::sendMessageToServer);
    connect(webSocketHandler, &WebSocketHandler::newMessageReadyForProcessing, this, &ClientManager::processSocketMessage);
    connect(webSocketHandler, &WebSocketHandler::connectionError, this, &ClientManager::clientDisconnected);

    // Mmessage process handler
    connect(this, &ClientManager::processSocketMessage, messageProcessHandler, &MessageProcessHandler::processSocketMessage);
    connect(messageProcessHandler, &MessageProcessHandler::setClientID, this, &ClientManager::setClientID);
    connect(messageProcessHandler, &MessageProcessHandler::newLobby, this, &ClientManager::onLobbyJoined);

    connect(messageProcessHandler, &MessageProcessHandler::userListUpdated, this, &ClientManager::userListChanged);
    connect(messageProcessHandler, &MessageProcessHandler::readyListUpdated, this, &ClientManager::readyListChanged);
    connect(messageProcessHandler, &MessageProcessHandler::newLobbyMessageRecieved, this, &ClientManager::newLobbyMessageRecieved);

    connect(messageProcessHandler, &MessageProcessHandler::gameStarted, this, &ClientManager::gameStarted);
    connect(messageProcessHandler, &MessageProcessHandler::getSentence, this, &ClientManager::sentenceRequest);
    connect(messageProcessHandler, &MessageProcessHandler::getDrawing, this, &ClientManager::drawingRequest);
    connect(messageProcessHandler, &MessageProcessHandler::gameEnded, this, &ClientManager::onGameEnded);
    connect(messageProcessHandler, &MessageProcessHandler::displayRound, this, &ClientManager::displayRound);

    connect(this, &ClientManager::processScreenMessage, messageProcessHandler, &MessageProcessHandler::processScreenMessage);
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
    } else {
        emit error("fileError");
    }
}

void ClientManager::writeNickname() {
    QFile file("nickname.txt");
    if(file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << nickname;
        file.flush();
        file.close();
    } else {
        emit error("fileError");
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
            emit lobbyJoined(newLobbyID);
        }
    }
}

void ClientManager::connectToServer() {
    webSocketHandler->connectToServer("ws://127.0.0.1:8585");
}

void ClientManager::closeConnection() {
    if (webSocketHandler->isValid()) {
        webSocketHandler->close();
    }
}

void ClientManager::toggleReadyRequest() {
    emit newMessageReadyToSend("type:toggleReady;payLoad:0;lobbyID:" + lobbyID + ";senderID:" + clientID);
}

void ClientManager::createLobbyRequest() {
    QString newNickname = NickInputDialog::getNickname(nickname);

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
    QString newNickname = NickInputDialog::getNickname(nickname);

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
}

void ClientManager::leaveLobby() {
    lobbyID = "";
}

void ClientManager::onGameEnded(QStringList userList) {
    emit lobbyJoined("");
    emit userListChanged(userList);
}

void ClientManager::sendDrawing(QString drawingData) {
    emit newMessageReadyToSend("type:drawingData;payLoad:" + drawingData + ";senderID:" + clientID);
}

void ClientManager::sendSentence(QString sentence) {
    emit newMessageReadyToSend("type:sentence;payLoad:" + sentence + ";senderID:" + clientID);
}

void ClientManager::getRoundRequest() {
    emit newMessageReadyToSend("type:getRound;payLoad:0;senderID:" + clientID);
}
