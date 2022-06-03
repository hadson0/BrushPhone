#include "gamesentencescreen.h"

GameSentenceScreen::GameSentenceScreen(QString drawingData, QWidget *parent)
    : Screen{parent} {
    setPadding(15);
    setSpacing(10);

    // Timer
    timer = new TimerWidget(60, this);
    timer->start();
    connect(timer, &TimerWidget::isOver, this, &GameSentenceScreen::done);

    // Game sentence frame
    gameSentence = new GameSentenceFrame(drawingData, this);
    connect(this, &GameSentenceScreen::done, gameSentence, &GameSentenceFrame::onDoneButtonClicked);
    connect(gameSentence, &GameSentenceFrame::sendSentence, this, &GameSentenceScreen::sendSentence);
}

void GameSentenceScreen::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);

    // Timer
    int timerWidht = this->getAvaliableWidth() * 0.1, timerHeight = this->getAvaliableHeight() * 0.08;
    int timerX = (this->width() - timer->width()) / 2, timerY = this->getPadding();
    timer->setGeometry(timerX, timerY, timerWidht, timerHeight);

    // Game sntence frame
    int gameSentenceX = this->getPadding(), gameSenteceY = timerY + timer->height() + this->getSpacing();
    int gameSentenceWidth = this->getAvaliableWidth(), gameSentenceHeight = this->getAvaliableHeight() - timer->height() - this->getSpacing();
    gameSentence->setGeometry(gameSentenceX, gameSenteceY, gameSentenceWidth, gameSentenceHeight);
}

