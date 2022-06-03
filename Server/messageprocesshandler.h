#ifndef MESSAGEPROCESSHANDLER_H
#define MESSAGEPROCESSHANDLER_H

#include <QObject>
#include <QDebug>
#include <QRegularExpression>

class MessageProcessHandler : public QObject {
    Q_OBJECT

    QString getMessageData(QString message, QString dataIdentifier);

public:
    explicit MessageProcessHandler(QObject *parent = nullptr);

public slots:
    void processSocketMessage(QString message);

signals:
    void createLobbyRequest(QString clientID, QString nickname);
    void joinLobbyRequest(QString lobbyID, QString clientID, QString nickname);
    void messageLobbyRequest(QString message, QString lobbyID, QString senderID);
    void toggleReadyRequest(QString lobbyID, QString clientID);
    void quitLobbyRequest(QString lobbyID, QString clientID);

    void setDrawingRequest(QString clientID, QString drawing);
    void setSentenceRequest(QString clientID, QString sentence);
};

#endif // MESSAGEPROCESSHANDLER_H
