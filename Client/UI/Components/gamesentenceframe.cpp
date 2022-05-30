#include "gamesentenceframe.h"

GameSentenceFrame::GameSentenceFrame(QWidget *parent)
    : BackgroundedFrame{parent} {
    setPadding(10);
    setSpacing(20);

    // Label
    label = new CustomLabel("Start writing a sentence", this);

    // Sentence input
    sentenceInput = new CustomLineEdit(this);
    connect(sentenceInput, &QLineEdit::returnPressed, this, &GameSentenceFrame::onDoneButtonClicked);

    // Done button
    doneButton = new CustomPushButton("Done", this);
    connect(doneButton, &QPushButton::clicked, this, &GameSentenceFrame::onDoneButtonClicked);
}

void GameSentenceFrame::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);

    // Label
    int labelWidth = this->getAvaliableWidth(), labelHeight = this->getAvaliableHeight() * 0.15;
    int labelX = this->getPadding(), labelY = (this->height() / 2) - labelHeight;
    label->setGeometry(labelX, labelY, labelWidth, labelHeight);

    // Sentence input
    int inputWidth = this->getAvaliableWidth(), inputHeight = this->getAvaliableHeight() * 0.15;
    int inputX = labelX, inputY = labelY + labelHeight + this->getSpacing();
    sentenceInput->setGeometry(inputX, inputY, inputWidth, inputHeight);

    // Done nutton
    int buttonWidht = this->getAvaliableWidth() * 0.3, buttonHeight = this->getAvaliableHeight() * 0.2;
    int buttonX = (this->width() - buttonWidht) / 2, buttonY = inputY + inputHeight + this->getSpacing();
    doneButton->setGeometry(buttonX, buttonY, buttonWidht, buttonHeight);
}

void GameSentenceFrame::onDoneButtonClicked() {
    QString sentence = sentenceInput->text();
    sentenceInput->clear();
    emit sendSentence(sentence);
}
