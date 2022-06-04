#include "roundscreen.h"

RoundScreen::RoundScreen(QString sentence, QString drawingData, QWidget *parent)
    : Screen{parent} {
    setPadding(15);
    setSpacing(10);

    // Next button
    nextButton = new CustomPushButton("Next", this);
    connect(nextButton, &QPushButton::clicked, this, &RoundScreen::nextRound);

    // Game sentence frame
    displayFrame = new RoundFrame(sentence, drawingData, this);
}

void RoundScreen::recalculateGeometry() {
    // Next Button
    int nextButtonWidth = this->getAvaliableWidth() * 0.08, nextButtonHeight = this->getAvaliableHeight() * 0.08;
    int nextButtonX = this->getAvaliableWidth() - nextButtonWidth, nextButtonY = this->getPadding();
    nextButton->setGeometry(nextButtonX, nextButtonY, nextButtonWidth, nextButtonHeight);

    // Game sntence frame
    int displayFrameX = this->getPadding(), displayFrameY = this->getPadding() + nextButtonY + nextButtonHeight;
    int gameSentenceWidth = this->getAvaliableWidth(), gameSentenceHeight = this->getAvaliableHeight() - nextButtonHeight - this->getSpacing();
    displayFrame->setGeometry(displayFrameX, displayFrameY, gameSentenceWidth, gameSentenceHeight);
}

void RoundScreen::resizeEvent(QResizeEvent *event) {
    recalculateGeometry();
    event->accept();
}
