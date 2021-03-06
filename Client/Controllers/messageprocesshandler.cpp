#include "messageprocesshandler.h"

MessageProcessHandler::MessageProcessHandler(QObject *parent)
    : QObject{parent} {}

// Extracts the data from the message and returns it
QString MessageProcessHandler::getMessageData(QString message, QString dataIdentifier) {
    QRegularExpression separator;
    separator.setPattern(";");

    // Searches for the data and returns it;
    for (QString &message : message.split(separator)) {
        if (message.contains(dataIdentifier)) {
            return message.remove(dataIdentifier + ":");
        }
    }

    // If the data isn't in the message, returns an empty QString
    return "";
}

void MessageProcessHandler::processSocketMessage(QString message) {
    // qDebug() << "User App: Message to process: " << message;

    static QRegularExpression separator(",");

    QString type = getMessageData(message, "type");
    QString clientID, lobbyID, senderNick, lobbyMessage , errorCode;
    QString sentence, drawingData;
    QStringList userList;

    // type:uniqueId;payLoad:1234
    if (type == "uniqueID") {

        clientID = getMessageData(message, "payLoad");
        if (!clientID.isEmpty()) {
            qDebug() << "User ID received: " << clientID;
            emit setClientID(clientID);
        } else {
            emit error("comunicationError");
        }
    }

    // type:newLobbyCreated;payLoad:"1234;userList:hadson0,whoam1
    else if (type == "newLobbyCreated" || type == "joinSuccess") {
        lobbyID = getMessageData(message, "payLoad");
        userList = getMessageData(message, "userList").split(separator);

        if (!lobbyID.isEmpty() && !userList.isEmpty()) {
            qDebug() << "New lobby, ID: " << lobbyID;
            emit newLobby(lobbyID, userList);
        } else {
            emit error("comunicationError");
        }
    }

    // type:updatedUserList;payLoad:0;userList:hadson0,whoam1
    else if (type == "updatedUserList") {
        userList = getMessageData(message, "userList").split(separator);

        if (!userList.isEmpty()) {
            qDebug() << "Users in lobby: " << userList;
            emit userListUpdated(userList);
        } else {
            emit error("comunicationError");
        }
    }

    // type:lobbyMessage;payLoad:HelloLobby;senderNick:hadson0
    else if (type == "lobbyMessage") {
        lobbyMessage = getMessageData(message, "payLoad");
        senderNick = getMessageData(message, "senderNick");

        if (!lobbyMessage.isEmpty() && !senderNick.isEmpty()) {
            emit newLobbyMessageRecieved(lobbyMessage, senderNick);
        } else {
            emit error("comunicationError");
        }
    }

    // type:updatedReadyUserList;payLoad:0;userList:hadson0,whoam1
    else if (type == "updatedReadyUserList") {
        userList = getMessageData(message, "userList").split(separator);

        if (!userList.isEmpty()) {
            qDebug() << "Ready users in lobby: " << userList;
            emit readyListUpdated(userList);
        } else {
            emit error("comunicationError");
        }
    }

    // type:gameStarted;payLoad:0
    else if (type == "gameStarted") {
        emit gameStarted();
    }

    // type:describeDrawing;payLoad:<drawingData>
    else if (type == "describeDrawing") {
        drawingData = getMessageData(message, "payLoad");

        if (!drawingData.isEmpty()) {
            emit getSentence(drawingData);
        } else {
            emit error("comunicationError");
        }
    }

    // type:drawSentence;payLoad:A blue sky
    else if (type == "drawSentence") {
        sentence = getMessageData(message, "payLoad");

        if (!sentence.isEmpty()) {
            emit getDrawing(sentence);
        } else {
            emit error("comunicationError");
        }
    }

    // type:displayRound;sentence:abc;drawingData:<drawingData>
    else if (type == "displayRound") {
        sentence = getMessageData(message, "sentence");
        drawingData = getMessageData(message, "drawingData");

        if (!sentence.isEmpty() && !drawingData.isEmpty()) {
            qDebug() << "Users in lobby: " << userList;
            emit displayRound(sentence, drawingData);
        } else {
            emit error("comunicationError");
        }
    }

    // type:finalLobby;payLoad:0;userList:hadson0,whoam1
    else if (type == "finalLobby") {
        userList = getMessageData(message, "userList").split(separator);

        if (!userList.isEmpty()) {
            qDebug() << "Users in lobby: " << userList;
            emit gameEnded(userList);
        }
    }

    // Errors
    // type:error;payLoad:existingNickError
    else if (type == "error") {
        errorCode = getMessageData(message, "payLoad");
        if (!errorCode.isEmpty()) {            
            emit error(errorCode);
        } else {
            emit error("comunicationError");
        }
    }
}

void MessageProcessHandler::processScreenMessage(QString message) {
    QString type = getMessageData(message, "type");
    QString newLobbyID, lobbyMessage;

    // type:createLobbyRequest;payload:0
    if (type == "createLobbyRequest") {
        emit createLobbyRequest();
    }

    // type:joinLobbyRequest;payLoad:1234
    else if (type == "joinLobbyRequest") {
        newLobbyID = getMessageData(message, "payLoad");

        if (!newLobbyID.isEmpty()) {
            emit joinLobbyRequest(newLobbyID);
        }
    }

    // type:sendLobbyMessageRequest;payload:HelloLobby
    else if (type == "sendLobbyMessageRequest") {
        lobbyMessage = getMessageData(message, "payLoad");
        if (!lobbyMessage.isEmpty()) {
            emit sendLobbyMessageRequest(lobbyMessage);
        }
    }

    // type:toggleReadyRequest;payLoad:0
    else if (type == "toggleReadyRequest") {
        emit toggleReadyRequest();
    }

    // type:quitLobbyRequest;payLoad:0;
    else if (type == "quitLobbyRequest") {
        emit quitLobbyRequest();
    }
}
