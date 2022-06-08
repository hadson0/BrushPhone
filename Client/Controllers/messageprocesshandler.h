#ifndef MESSAGEPROCESSHANDLER_H
#define MESSAGEPROCESSHANDLER_H

#include <QObject>
#include <QRegularExpression>
#include <QDebug>

class MessageProcessHandler : public QObject {
    Q_OBJECT

public:
    explicit MessageProcessHandler(QObject *parent = nullptr);

    QString getMessageData(QString message, QString dataIdentifier);

public slots:
    void processSocketMessage(QString message);
    void processScreenMessage(QString message);

signals:
    // Socket related signals
    void setClientID(QString clientID);
    void newLobby(QString newLobbyID, QStringList newUSerList);
    void userListUpdated(QStringList newUSerList);
    void readyListUpdated(QStringList newReadyList);
    void newLobbyMessageRecieved(QString message, QString senderNick);

    void gameStarted();
    void getSentence(QString drawingData);
    void getDrawing(QString sentence);
    void displayRound(QString sentence, QString drawingData);
    void gameEnded(QStringList userList);

    // Screen related signals
    void displayScreenRequest(QString destinationScreen);
    void createLobbyRequest();
    void joinLobbyRequest(QString newLobbyID);
    void sendLobbyMessageRequest(QString message);
    void toggleReadyRequest();
    void quitLobbyRequest();

    void error(QString error);
};

#endif // MESSAGEPROCESSHANDLER_H
