#include "servermanager.h"

ServerManager::ServerManager(QObject *parent)
    : QObject{parent} {
    webSocketHandler = WebSocketHandler::getInstance(this);
    messageProcessorHandler = new MessageProcessHandler(this);

    //Coneccts signals and slots
    connect(webSocketHandler, &WebSocketHandler::newMessageToProcess, messageProcessorHandler, &MessageProcessHandler::processSocketMessage);
    connect(webSocketHandler, &WebSocketHandler::clientDisconnected, this, &ServerManager::onClientDisconnected);

    connect(messageProcessorHandler, &MessageProcessHandler::toggleReadyRequest, this, &ServerManager::toggleReadyRequest);
    connect(messageProcessorHandler, &MessageProcessHandler::createLobbyRequest, this, &ServerManager::createLobbyRequest);
    connect(messageProcessorHandler, &MessageProcessHandler::joinLobbyRequest, this, &ServerManager::joinLobbyRequest);
    connect(messageProcessorHandler, &MessageProcessHandler::quitLobbyRequest, this, &ServerManager::quitLobbyRequest);
    connect(messageProcessorHandler, &MessageProcessHandler::messageLobbyRequest, this, &ServerManager::lobbyMessageRequest);

    connect(messageProcessorHandler, &MessageProcessHandler::setDrawingRequest, this, &ServerManager::setDrawingRequest);
    connect(messageProcessorHandler, &MessageProcessHandler::setSentenceRequest, this, &ServerManager::setSentenceRequest);
    connect(messageProcessorHandler, &MessageProcessHandler::getRoundRequest, this, &ServerManager::getRoundRequest);

}

// Generates a QString composed of 4 random numbers. Ex.: "1234"
QString ServerManager::generateRandomID() {
    std::random_device rd;
    std:: default_random_engine generator(rd());
    std::uniform_int_distribution<int> idGenerator(1000, 9999);
    return QString::number(idGenerator(generator));
}

void ServerManager::deleteLobby(QString lobbyID) {
    // Checks if the lobby is registered
    if (lobbyMap.contains(lobbyID)) {
        Lobby *lobby = lobbyMap[lobbyID];
        lobbyMap.remove(lobbyID);
        lobby->deleteLater();
    }
}

void ServerManager::createLobbyRequest(QString clientID, QString nickname) {
    QString newLobbyID = generateRandomID();

    // Make surte that the generated ID is unique
    QStringList existingKeys = lobbyMap.keys();
    for (; existingKeys.contains(newLobbyID); newLobbyID = generateRandomID());

    // Registers the new lobby
    Lobby *newLobby = new Lobby(newLobbyID, this);
    connect(newLobby, &Lobby::userListChanged, this, &ServerManager::onUserListChanged);
    connect(newLobby, &Lobby::readyListChanged, this, &ServerManager::onReadyListChanged);

    connect(newLobby, &Lobby::gameStarted, this, &ServerManager::onGameStarted);
    connect(newLobby, &Lobby::drawingRequest, this, &ServerManager::onDrawingRequest);
    connect(newLobby, &Lobby::sentenceRequest, this, &ServerManager::onSentenceRequest);
    connect(newLobby, &Lobby::displayRoundRequest, this, &ServerManager::onDisplayRoundRequest);
    connect(newLobby, &Lobby::finalLobby, this, &ServerManager::onFinalLobby);

    connect(this, &ServerManager::setDrawingRequest, newLobby, &Lobby::setDrawingRequest);
    connect(this, &ServerManager::setSentenceRequest, newLobby, &Lobby::setSentenceRequest);
    connect(this, &ServerManager::getRoundRequest, newLobby, &Lobby::getGameRound);

    newLobby->addUser(clientID, nickname);
    lobbyMap[newLobbyID] = newLobby;
    clientLobbyMap[clientID] = newLobby;

    // Sends the lobby ID and the user list to the client
    webSocketHandler->sendTextMessage("type:newLobbyCreated;payLoad:" + newLobbyID + ";userList:" + newLobby->getUsersToStr(), clientID);

    qDebug() << "New Lobby created, ID: " << newLobbyID;
}

void ServerManager::joinLobbyRequest(QString lobbyID, QString clientID, QString nickname) {
    if (lobbyMap.contains(lobbyID)) {
        if (!lobbyMap[lobbyID]->containsNickname(nickname)) {
            Lobby *lobby = lobbyMap[lobbyID];
            lobby->addUser(clientID, nickname);
            clientLobbyMap[clientID] = lobby;

            // Informs the client that it was a success
            webSocketHandler->sendTextMessage("type:joinSuccess;payLoad:" + lobbyID  + ";userList:" + lobby->getUsersToStr() , clientID);
        } else {
            // Informs the client that an error has occurred
            webSocketHandler->sendTextMessage("type:error;payLoad:existingNickError", clientID);
        }
    } else {
        // Informs the client that an error has occurred
        webSocketHandler->sendTextMessage("type:error;payLoad:joinError", clientID);
    }
}

void ServerManager::quitLobbyRequest(QString lobbyID, QString clientID) {
    // Checks if the lobby is registered
    if (lobbyMap.contains(lobbyID)) {
        Lobby *lobby = lobbyMap[lobbyID];
        lobby->removeUser(clientID);
        clientLobbyMap.remove(clientID);
    } else {
        // Informs the client that an error has occurred
        webSocketHandler->sendTextMessage("type:error;payLoad:quitError", clientID);
    }
}

void ServerManager::lobbyMessageRequest(QString message, QString lobbyID, QString clientID) {
    // Checks if the lobby is registered
    if (lobbyMap.contains(lobbyID)) {
        Lobby *lobby = lobbyMap[lobbyID];
        QString senderNick = lobby->getUserNick(clientID);

        // Sends the message to all the clients in the lobby
        QStringList clientList = lobby->getClientList();
        webSocketHandler->sendTextMessage("type:lobbyMessage;payLoad:" + message + ";senderNick:" + senderNick, clientList);
    } else {
        // Informs the client that an error has occurred
        webSocketHandler->sendTextMessage("type:error;payLoad:lobbyMessageError", clientID);
    }
}

void ServerManager::toggleReadyRequest(QString lobbyID, QString clientID) {
    // Checks if the lobby is registered
    if (lobbyMap.contains(lobbyID)) {
        Lobby *lobby = lobbyMap[lobbyID];
        lobby->toggleReady(clientID);
    }
}

void ServerManager::onUserListChanged(QString userList, QStringList &clientList) {
    // If there is clients in lobby
    if (!userList.isEmpty() && !clientList.isEmpty()) {
        // Update the user list to all the clients in the lobby
         webSocketHandler->sendTextMessage("type:updatedUserList;payLoad:0;userList:" + userList, clientList);
    } else {
        try {
            // Delete the lobby
            Lobby *lobby = qobject_cast<Lobby *>(sender());
            QString lobbyID = lobby->getID();
            lobbyMap[lobbyID]->deleteLater();
            lobbyMap.remove(lobbyID);
        } catch (...) {
            qDebug() << "Error deleting lobby";
        }
    }
}

void ServerManager::onReadyListChanged(QString readyUSers, QStringList &clientList) {
    // Update the ready user list to all the clients in the lobby
    webSocketHandler->sendTextMessage("type:updatedReadyUserList;payLoad:0;userList:" + readyUSers, clientList);
}

void ServerManager::onGameStarted(QStringList &clientList) {
    // Informs all clients that the game started
    webSocketHandler->sendTextMessage("type:gameStarted;payLoad:0", clientList);
}

void ServerManager::onSentenceRequest(QString clientID, QString drawingData) {
    // Pass a drawing to the client and asks to describe it
    webSocketHandler->sendTextMessage("type:describeDrawing;payLoad:" + drawingData, clientID);
}

void ServerManager::onDrawingRequest(QString clientID, QString sentence) {
    // Pass a sentence to the client and asks to draw it
    webSocketHandler->sendTextMessage("type:drawSentence;payLoad:" + sentence, clientID);
}

void ServerManager::onDisplayRoundRequest(QString sentence, QString drawingData, QStringList &clientList) {
    // Informs all clients to display the round
    webSocketHandler->sendTextMessage("type:displayRound;sentence:" + sentence + ";drawingData:" + drawingData, clientList);
}

void ServerManager::onFinalLobby(QString userList, QStringList &clientList) {
    // Informs all clients to display the final lobby
    webSocketHandler->sendTextMessage("type:finalLobby;payLoad:0;userList:" + userList, clientList);
}

void ServerManager::onClientDisconnected(QString clientID) {
    if (clientLobbyMap.contains(clientID)) { // If the client was in a lobby
        Lobby *lobby = clientLobbyMap[clientID];

        // Disconnects the client
        lobby->removeUser(clientID);
        clientLobbyMap.remove(clientID);


        QStringList clientList = lobby->getClientList();

        if (lobby->isGameOn()) {
            webSocketHandler->sendTextMessage("type:error;payLoad:userDisconnected", clientList);
        }

        else if (clientList.isEmpty()) {
            deleteLobby(lobby->getID());
            qDebug() << "Lobby deleted";
        }

    }
}
