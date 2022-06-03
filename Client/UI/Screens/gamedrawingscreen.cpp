#include "gamedrawingscreen.h"

GameDrawingScreen::GameDrawingScreen(QString sentence, QWidget *parent)
    : Screen{parent} {
    setPadding(15);
    setSpacing(10);

    // Game sentence frame
    gameDrawing = new GameDrawingFrame(sentence, this);
    connect(gameDrawing, &GameDrawingFrame::sendDrawing, this, &GameDrawingScreen::sendDrawing);

    // Timer
    timer = new TimerWidget(180, this);
    timer->start();
    connect(timer, &TimerWidget::isOver, gameDrawing, &GameDrawingFrame::onDoneButtonClicked);
}

void GameDrawingScreen::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);

    // Timer
    int timerWidht = this->getAvaliableWidth() * 0.1, timerHeight = this->getAvaliableHeight() * 0.08;
    int timerX = (this->width() - timer->width()) / 2, timerY = this->getPadding();
    timer->setGeometry(timerX, timerY, timerWidht, timerHeight);

    // Game sntence frame
    int gameDrawingX = this->getPadding(), gameSenteceY = timerY + timer->height() + this->getSpacing();
    int gameDrawingWidth = this->getAvaliableWidth(), gameDrawingHeight = this->getAvaliableHeight() - timer->height() - this->getSpacing();
    gameDrawing->setGeometry(gameDrawingX, gameSenteceY, gameDrawingWidth, gameDrawingHeight);
}
