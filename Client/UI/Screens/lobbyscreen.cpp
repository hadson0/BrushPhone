#include "lobbyscreen.h"

LobbyScreen::LobbyScreen(QString lobbyID, QWidget *parent)
    : Screen{parent}, lobbyID(lobbyID), isStartLobby(false) {
    setPadding(15);
    // Gambiarra
    if (!lobbyID.isEmpty()) {
        isStartLobby = true;
    }

    // User List View
    userListView = new UserListView(this);
    connect(this, &LobbyScreen::userListChanged, userListView, &UserListView::onUserListChanged);
    connect(this, &LobbyScreen::readyListChanged, userListView, &UserListView::onReadyListChanged);
    connect(this, &LobbyScreen::setReady, userListView, &UserListView::setReady);

    // Chat Frame
    chatFrame = new ChatFrame(this);
    connect(chatFrame, &ChatFrame::sendMessage, this, &LobbyScreen::requestSendLobbyMessage);
    connect(this, &LobbyScreen::newMessageRecieved, chatFrame, &ChatFrame::onMessageRecieved);

    // Back Button
    backButton = new CustomPushButton("Back", this);
    connect(backButton, &QPushButton::clicked, this, &LobbyScreen::onBackButtonClicked);

    if (isStartLobby) {
        // Ready Button
        readyButton = new CustomPushButton("Ready", this);
        connect(readyButton, &QPushButton::clicked, this, &LobbyScreen::requestToggleReady);

        // Lobby ID Label
        lobbyIDLabel = new CustomLabel("Lobby ID: " + lobbyID, this);
    } else if (!chatFrame->readHistoric()) {
        emit error("fileError");
    }
}

LobbyScreen::~LobbyScreen() {
    if (!chatFrame->saveHistoric()) {
        emit error("fileError");
    }
}

void LobbyScreen::recalculateGeometry() {
    // Back Button
    int backButtonX = this->getPadding(), backButtonY = this->getPadding();
    int backButtonWidth = this->getAvaliableWidth() * 0.08, backButtonHeight = this->getAvaliableHeight() * 0.08;
    backButton->setGeometry(backButtonX, backButtonY, backButtonWidth, backButtonHeight);

    if (isStartLobby){
        // Ready Button
        int readyButtonWidth = backButtonWidth, readyButtonHeight = backButtonHeight;
        int readyButtonX = this->getAvaliableWidth() - readyButtonWidth, readyButtonY = this->getPadding();
        readyButton->setGeometry(readyButtonX, readyButtonY, readyButtonWidth, readyButtonHeight);

        // Lobby ID Label
        int lobbyIDLabelX = backButtonX + backButtonWidth, lobbyIDLabelY = backButtonY;
        int lobbyIDLabelWidth = this->getAvaliableWidth() - lobbyIDLabelX - readyButtonWidth, lobbyIDLabelHeight = backButtonHeight;
        lobbyIDLabel->setGeometry(lobbyIDLabelX, lobbyIDLabelY, lobbyIDLabelWidth, lobbyIDLabelHeight);

    }

    // User List View
    int clientListViewX = this->getPadding(), clientListViewY = this->getPadding() + 75;
    int clientListViewWidth = (this->getAvaliableWidth() - this->getPadding()) * 0.4, clientListViewHeight = this->getAvaliableHeight() - 75;
    userListView->setGeometry(clientListViewX, clientListViewY, clientListViewWidth, clientListViewHeight);

    // Chat Frame
    int chatFrameX = clientListViewWidth + 2 * this->getPadding(), chatFrameY = clientListViewY;
    int chatFrameWidth = (this->getAvaliableWidth() - this->getPadding()) * 0.6, chatFrameHeight = clientListViewHeight;
    chatFrame->setGeometry(chatFrameX, chatFrameY, chatFrameWidth, chatFrameHeight);
}

void LobbyScreen::resizeEvent(QResizeEvent *event) {
    recalculateGeometry();
    event->accept();
}

void LobbyScreen::requestSendLobbyMessage(QString message) {
    emit sendRequestMessage("type:sendLobbyMessageRequest;payLoad:" + message);
}

void LobbyScreen::requestToggleReady() {
    emit sendRequestMessage("type:toggleReadyRequest;payLoad:0");
}

void LobbyScreen::onBackButtonClicked() {
    QMessageBox::StandardButton response = QMessageBox::question( this, "Leave lobby",
                                                                    "Are you sure you want to leave the lobby?",
                                                                    QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::Yes);

    if (response == QMessageBox::StandardButton::Yes) {
        emit sendRequestMessage("type:quitLobbyRequest;payLoad:0;");
        emit backRequest();
    }
}
