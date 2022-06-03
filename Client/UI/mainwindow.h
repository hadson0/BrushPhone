#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QPainter>
#include <QPaintEvent>
#include <QApplication>
#include <QStack>
#include <QMessageBox>
#include <QCloseEvent>

#include "Controllers/clientmanager.h"
#include "Controllers/websockethandler.h"

#include "Screens/mainmenuscreen.h"
#include "Screens/selectionscreen.h"
#include "Screens/joinlobbyscreen.h"
#include "Screens/lobbyscreen.h"
#include "Screens/sentencescreen.h"
#include "Screens/drawingscreen.h"
#include "Screens/roundscreen.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    // Screens
    QStack<Screen *> menuScreenStack;
    Screen *gameScreen;

    ClientManager *clientManager;
    WebSocketHandler *webSocketHandler;

    void closeEvent(QCloseEvent *event);

public:
    MainWindow(QWidget *parent = nullptr);

    void closeAllScreens();

public slots:
    void displayMenuScreen(QString targetScreen);

    // Display game screens
    void displayLobbyScreen(QString lobbyID);
    void displaySentenceScreen(QString drawingData);
    void displayDrawingScreen(QString sentence);
    void displayRoundScreen(QString sentence, QString drawingData);

    void onGameLobbyPhase();

    void onBackRequested();
    void onConnectToServerRequest();
    void onClientConnected();
    void onClientDisconnected();

    void onErrorOccurrence(QString error);
};

#endif // MAINWINDOW_H
