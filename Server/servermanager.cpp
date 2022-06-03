#include "servermanager.h"

ClientManager::ClientManager(QObject *parent)
    : QObject{parent} {
    webSocketHandler = WebSocketHandler::getInstance(this);
    messageProcessorHandler = new MessageProcessHandler(this);

    //Coneccts signals and slots
    connect(webSocketHandler, &WebSocketHandler::newMessageToProcess, messageProcessorHandler, &MessageProcessHandler::processSocketMessage);
    connect(webSocketHandler, &WebSocketHandler::clientDisconnected, this, &ClientManager::onClientDisconnected);

    connect(messageProcessorHandler, &MessageProcessHandler::toggleReadyRequest, this, &ClientManager::toggleReadyRequest);
    connect(messageProcessorHandler, &MessageProcessHandler::createLobbyRequest, this, &ClientManager::createLobbyRequest);
    connect(messageProcessorHandler, &MessageProcessHandler::joinLobbyRequest, this, &ClientManager::joinLobbyRequest);
    connect(messageProcessorHandler, &MessageProcessHandler::quitLobbyRequest, this, &ClientManager::quitLobbyRequest);
    connect(messageProcessorHandler, &MessageProcessHandler::messageLobbyRequest, this, &ClientManager::lobbyMessageRequest);

    connect(messageProcessorHandler, &MessageProcessHandler::setDrawingRequest, this, &ClientManager::setDrawingRequest);
    connect(messageProcessorHandler, &MessageProcessHandler::setSentenceRequest, this, &ClientManager::setSentenceRequest);
}

// Generates a QString composed of 4 random numbers. Ex.: "1234"
QString ClientManager::generateRandomID() {
    std::random_device rd;
    std:: default_random_engine generator(rd());
    std::uniform_int_distribution<int> idGenerator(1000, 9999);
    return QString::number(idGenerator(generator));
}

void ClientManager::createLobbyRequest(QString clientID, QString nickname) {
    QString newLobbyID = generateRandomID();

    // Make surte that the generated ID is unique
    QStringList existingKeys = lobbyMap.keys();
    for (; existingKeys.contains(newLobbyID); newLobbyID = generateRandomID());

    // Registers the new lobby
    Lobby *newLobby = new Lobby(newLobbyID, this);
    connect(newLobby, &Lobby::userListChanged, this, &ClientManager::onUserListChanged);
    connect(newLobby, &Lobby::readyListChanged, this, &ClientManager::onReadyListChanged);

    connect(newLobby, &Lobby::gameStarted, this, &ClientManager::onGameStarted);
    connect(newLobby, &Lobby::drawingRequest, this, &ClientManager::onDrawingRequest);
    connect(newLobby, &Lobby::sentenceRequest, this, &ClientManager::onSentenceRequest);
    connect(newLobby, &Lobby::gameEnded, this, &ClientManager::onGameEnded);

    connect(this, &ClientManager::setDrawingRequest, newLobby, &Lobby::setDrawingRequest);
    connect(this, &ClientManager::setSentenceRequest, newLobby, &Lobby::setSentenceRequest);

    newLobby->addUser(clientID, nickname);
    lobbyMap[newLobbyID] = newLobby;
    clientLobbyMap[clientID] = newLobby;

    // Sends the lobby ID and the user list to the client
    webSocketHandler->sendTextMessage("type:newLobbyCreated;payLoad:" + newLobbyID + ";userList:" + newLobby->getUsersToStr(), clientID);

    qDebug() << "New Lobby created, ID: " << newLobbyID;
}

void ClientManager::joinLobbyRequest(QString lobbyID, QString clientID, QString nickname) {
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

void ClientManager::quitLobbyRequest(QString lobbyID, QString clientID) {
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

void ClientManager::lobbyMessageRequest(QString message, QString lobbyID, QString clientID) {
    // Checks if the lobby is registered
    if (lobbyMap.contains(lobbyID)) {
        Lobby *lobby = lobbyMap[lobbyID];
        QString senderNick = lobby->getUserNick(clientID);

        // Sends the message to all the clients in the lobby
        webSocketHandler->sendTextMessage("type:lobbyMessage;payLoad:" + message + ";senderNick:" + senderNick, lobby->getClientList());
    } else {
        // Informs the client that an error has occurred
        webSocketHandler->sendTextMessage("type:error;payLoad:lobbyMessageError", clientID);
    }
}

void ClientManager::toggleReadyRequest(QString lobbyID, QString clientID) {
    // Checks if the lobby is registered
    if (lobbyMap.contains(lobbyID)) {
        Lobby *lobby = lobbyMap[lobbyID];
        lobby->toggleReady(clientID);
    }
}

void ClientManager::onUserListChanged(QString userList, QStringList clientList) {
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

void ClientManager::onReadyListChanged(QString readyUSers, QStringList clientList) {
    // Update the ready user list to all the clients in the lobby
    webSocketHandler->sendTextMessage("type:updatedReadyUserList;payLoad:0;userList:" + readyUSers, clientList);
}

void ClientManager::onGameStarted(QStringList clientList) {
    // Informs all clients that the game started
    webSocketHandler->sendTextMessage("type:gameStarted;payLoad:0", clientList);
}

void ClientManager::onSentenceRequest(QString clientID, QString drawingData) {
    // Pass a drawing to the client and asks to describe it
    webSocketHandler->sendTextMessage("type:describeDrawing;payLoad:" + drawingData, clientID);
}

void ClientManager::onDrawingRequest(QString clientID, QString sentence) {
    // Pass a sentence to the client and asks to draw it
    webSocketHandler->sendTextMessage("type:drawSentence;payLoad:" + sentence, clientID);
}

void ClientManager::onGameEnded(QString userList, QStringList clientList) {
    // Informs all clients that the game ended
    webSocketHandler->sendTextMessage("type:gameEnded;payLoad:0;userList:" + userList, clientList);
}

void ClientManager::onClientDisconnected(QString clientID) {
    if (clientLobbyMap.contains(clientID)) { // If the client was in a lobby
        clientLobbyMap[clientID]->removeUser(clientID); // Exits the lobby
        clientLobbyMap.remove(clientID);
        qDebug() << "Lobby deleted";
    }
}
