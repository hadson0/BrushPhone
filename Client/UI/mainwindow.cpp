#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("Game Lobby Prototype");
    gameScreen = nullptr;

    // Maintains window aspect ratio
    QSizePolicy sizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    sizePolicy.setHeightForWidth(true);
    setSizePolicy(sizePolicy);

    // Creates a Web Socket Handler
    webSocketHandler = WebSocketHandler::getInstance(this);
    connect(webSocketHandler, &WebSocketHandler::connectionError, this, &MainWindow::onClientDisconnected);

    // Creates a game manager and connects it to the Web Socket Handler
    clientManager = ClientManager::getInstance(this);
    connect(webSocketHandler, &WebSocketHandler::newMessageReadyForProcessing, clientManager, &ClientManager::processSocketMessage);
    connect(clientManager, &ClientManager::newMessageReadyToSend, webSocketHandler, &WebSocketHandler::sendMessageToServer);
    connect(clientManager, &ClientManager::clientConnected, this, &MainWindow::onClientConnected);
    connect(clientManager, &ClientManager::error, this, &MainWindow::onErrorOccurrence);
    connect(clientManager, &ClientManager::lobbyLeft, this, &MainWindow::onBackRequested);

    connect(clientManager, &ClientManager::lobbyJoined, this, &MainWindow::displayLobbyScreen);
    connect(clientManager, &ClientManager::gameStarted, this, &MainWindow::onGameLobbyPhase);
    connect(clientManager, &ClientManager::sentenceRequest, this, &MainWindow::displaySentenceScreen);
    connect(clientManager, &ClientManager::drawingRequest, this, &MainWindow::displayDrawingScreen);
    connect(clientManager, &ClientManager::displayRound, this, &MainWindow::displayRoundScreen);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton response = QMessageBox::question( this, "Close Game",
                                                                    "Are you sure you want to close the game?",
                                                                    QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::Yes);
        if (response != QMessageBox::Yes) {
            event->ignore();
        } else {
            event->accept();
        }
}

// Hides the old screen and display the one mentioned in the argument
void MainWindow::displayMenuScreen(QString destinationMenuScreen) {
    Screen *newScreen = nullptr;

    if (destinationMenuScreen == "MainMenuScreen") {
        newScreen = new MainMenuScreen(this);
        connect(qobject_cast<MainMenuScreen *>(newScreen), &MainMenuScreen::connectToTheServerRequest, this, &MainWindow::onConnectToServerRequest);
    } else if (destinationMenuScreen == "SelectionScreen") {
        newScreen = new SelectionScreen(this);
    } else if (destinationMenuScreen == "JoinLobbyScreen") {
        newScreen = new JoinLobbyScreen(this);
    }

    // If the screen was created successfully, deletes the old screen and displays the current one
    if (newScreen != nullptr) {
        connect(newScreen, &Screen::backRequest, this, &MainWindow::onBackRequested);
        connect(newScreen, &Screen::quitAppRequest, this, &QApplication::quit);
        connect(newScreen, &Screen::sendRequestMessage, clientManager, &ClientManager::processScreenMessage);
        connect(newScreen, &Screen::menuScreenDisplayRequest, this, &MainWindow::displayMenuScreen);
        connect(newScreen, &Screen::error, this, &MainWindow::onErrorOccurrence);

        if (!menuScreenStack.isEmpty()) {
            menuScreenStack.top()->hide();
        }

        menuScreenStack.push(newScreen);
        menuScreenStack.top()->show();
    }
}

void MainWindow::displayLobbyScreen(QString lobbyID) {
    if (gameScreen != nullptr) {
        gameScreen->deleteLater();
        gameScreen = nullptr; // Sets the lobbyScreen pointer to null, to avoid errors
    }

    LobbyScreen *lobbyScreen = new LobbyScreen(lobbyID, this);
    connect(clientManager, &ClientManager::userListChanged, lobbyScreen, &LobbyScreen::userListChanged);
    connect(clientManager, &ClientManager::readyListChanged, lobbyScreen, &LobbyScreen::readyListChanged);
    connect(clientManager, &ClientManager::newLobbyMessageRecieved, lobbyScreen, &LobbyScreen::newMessageRecieved);
    gameScreen = lobbyScreen;


    if (gameScreen != nullptr) {
        connect(gameScreen, &Screen::backRequest, this, &MainWindow::onBackRequested);
        connect(gameScreen, &Screen::sendRequestMessage, clientManager, &ClientManager::processScreenMessage);
        connect(gameScreen, &Screen::menuScreenDisplayRequest, this, &MainWindow::displayMenuScreen);
        connect(gameScreen, &Screen::error, this, &MainWindow::onErrorOccurrence);

        menuScreenStack.top()->hide();
        gameScreen->show();
    }
}

void MainWindow::displaySentenceScreen(QString drawingData) {
    if (gameScreen != nullptr) {
        gameScreen->deleteLater();
        gameScreen = nullptr; // Sets the lobbyScreen pointer to null, to avoid errors
    }

    SentenceScreen *gameSentenceScreen = new SentenceScreen(drawingData, this);
    connect(gameSentenceScreen, &SentenceScreen::sendSentence, clientManager, &ClientManager::sendSentence);
    gameScreen = gameSentenceScreen;

    if (gameScreen != nullptr) {
        connect(gameScreen, &Screen::backRequest, this, &MainWindow::onBackRequested);
        connect(gameScreen, &Screen::sendRequestMessage, clientManager, &ClientManager::processScreenMessage);
        connect(gameScreen, &Screen::menuScreenDisplayRequest, this, &MainWindow::displayMenuScreen);
        connect(gameScreen, &Screen::error, this, &MainWindow::onErrorOccurrence);

        menuScreenStack.top()->hide();
        gameScreen->show();
    }
}

void MainWindow::displayDrawingScreen(QString sentence) {
    if (gameScreen != nullptr) {
        gameScreen->deleteLater();
        gameScreen = nullptr; // Sets the lobbyScreen pointer to null, to avoid errors
    }

    DrawingScreen *gameDrawingScreen = new DrawingScreen(sentence, this);
    connect(gameDrawingScreen, &DrawingScreen::sendDrawing, clientManager, &ClientManager::sendDrawing);
    gameScreen = gameDrawingScreen;

    if (gameScreen != nullptr) {
        connect(gameScreen, &Screen::backRequest, this, &MainWindow::onBackRequested);
        connect(gameScreen, &Screen::sendRequestMessage, clientManager, &ClientManager::processScreenMessage);
        connect(gameScreen, &Screen::menuScreenDisplayRequest, this, &MainWindow::displayMenuScreen);
        connect(gameScreen, &Screen::error, this, &MainWindow::onErrorOccurrence);

        menuScreenStack.top()->hide();
        gameScreen->show();
    }
}

void MainWindow::displayRoundScreen(QString sentence, QString drawingData) {
    if (gameScreen != nullptr) {
        gameScreen->deleteLater();
        gameScreen = nullptr; // Sets the lobbyScreen pointer to null, to avoid errors
    }

    RoundScreen *roundDisplayScreen = new RoundScreen(sentence, drawingData, this);
    connect(roundDisplayScreen, &RoundScreen::nextRound, clientManager, &ClientManager::getRoundRequest);
    gameScreen = roundDisplayScreen;

    if (gameScreen != nullptr) {
        connect(gameScreen, &Screen::backRequest, this, &MainWindow::onBackRequested);
        connect(gameScreen, &Screen::sendRequestMessage, clientManager, &ClientManager::processScreenMessage);
        connect(gameScreen, &Screen::menuScreenDisplayRequest, this, &MainWindow::displayMenuScreen);
        connect(gameScreen, &Screen::error, this, &MainWindow::onErrorOccurrence);

        menuScreenStack.top()->hide();
        gameScreen->show();
    }
}

void MainWindow::onGameLobbyPhase() {
    displaySentenceScreen("");
}


void MainWindow::closeAllScreens() {
    // If the lobby screen is open, closes it
    if (gameScreen != nullptr) {
        clientManager->leaveLobby();
        gameScreen->deleteLater();
        gameScreen = nullptr; // Sets the lobbyScreen pointer to null, to avoid errors
    }

    // Closes all screens
    while (!menuScreenStack.isEmpty()) {
        menuScreenStack.top()->deleteLater();
        menuScreenStack.pop();
    }

    webSocketHandler->close();
    this->displayMenuScreen("MainMenuScreen");
}

// Goes to the previous screen
void MainWindow::onBackRequested() {
    // Deletes the current screen
    if (gameScreen != nullptr) { // If the current screen is the lobby screen
        gameScreen->deleteLater();
        gameScreen = nullptr; // Sets the lobbyScreen pointer to null, to avoid errors

        if (qobject_cast<LobbyScreen *>(sender())) {
            clientManager->leaveLobby();
        }

        menuScreenStack.top()->show();
    } else if (qobject_cast<SelectionScreen *>(sender())) { // If the current screen is a SelectionScreen
        this->closeAllScreens();
    } else {
        delete menuScreenStack.top();
        menuScreenStack.pop();

        menuScreenStack.top()->show();
    }
}

void MainWindow::onConnectToServerRequest() {
    webSocketHandler->connectToServer("ws://127.0.0.1:8585");
}

void MainWindow::onClientConnected() {
    displayMenuScreen("SelectionScreen");
}

void MainWindow::onClientDisconnected() {
    this->onErrorOccurrence("connectionError");
}

void MainWindow::onErrorOccurrence(QString errorCode) {
    // "joinError" || "quitError" || "lobbyMessageError" || "connectionError" || "nicknameError"
    if (errorCode == "joinError") {
        QMessageBox::warning(this, "Error", "An error occurred while trying to join the lobby");
    }

    else if (errorCode == "lobbyMessageError") {
        QMessageBox::warning(this, "Error", "An error occurred while trying to send a message to the lobby");
    }

    else if (errorCode == "blankNickError") {
        QMessageBox::warning(this, "Error", "Nickname field cannot be left blank.");
    }

    else if (errorCode == "joinFieldsError") {
        QMessageBox::warning(this, "Error", "Please fill in all fields correctly.");
    }

    else if (errorCode == "existingNickError") {
        QMessageBox::warning(this, "Error", "This nickname is already being used in the lobby.\n"
                                            "Try a different one.");
    }

    else if (errorCode == "quitError") {
        QMessageBox::warning(this, "Error", "An unexpected error has occurred.\n "
                                                         "You will be disconnected from the lobby");
        closeAllScreens();
    }

    else if (errorCode == "connectionError") {
        QMessageBox::warning(this, "Error", "Connection to server failed.");
        closeAllScreens();
    }
}

