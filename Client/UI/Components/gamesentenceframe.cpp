#include "gamesentenceframe.h"

GameSentenceFrame::GameSentenceFrame(QString drawingData, QWidget *parent)
    : BackgroundedFrame{parent}, hasDrawing(false) {
    setPadding(15);
    setSpacing(20);

    // Label
    label = new CustomLabel("Start writing a sentence", this);

    // Sentence input
    sentenceInput = new CustomLineEdit(this);
    //connect(sentenceInput, &QLineEdit::returnPressed, this, &GameSentenceFrame::onDoneButtonClicked);

    // Done button
    doneButton = new CustomPushButton("Done", this);
    connect(doneButton, &QPushButton::clicked, this, &GameSentenceFrame::onDoneButtonClicked);

    hasDrawing = drawingData != "";
    if (hasDrawing) {
        // Temporary file
        tempFile = new QTemporaryFile("./tempXXXX.png", this);
        openImage(drawingData);

        // Image label
        imageLabel = new QLabel(this);
        QPixmap image(tempFile->fileName());
        imageLabel->setPixmap(image);
    } else {
        tempFile = nullptr;
        imageLabel = nullptr;
    }
}

void GameSentenceFrame::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);

    // Label
    int offSet = (hasDrawing) ? 50:0;
    int labelWidth = this->getAvaliableWidth(), labelHeight = this->getAvaliableHeight() * 0.15;
    int labelX = this->getPadding(), labelY = (this->height() / 2) - labelHeight - 50 - offSet;
    label->setGeometry(labelX, labelY, labelWidth, labelHeight);

    // Sentence input
    int inputWidth = this->getAvaliableWidth(), inputHeight = this->getAvaliableHeight() * 0.15;
    int inputX = labelX, inputY = labelY + labelHeight + this->getSpacing();
    sentenceInput->setGeometry(inputX, inputY, inputWidth, inputHeight);

    // Done nutton
    int buttonWidht = this->getAvaliableWidth() * 0.3, buttonHeight = this->getAvaliableHeight() * 0.2;
    int buttonX = (this->width() - buttonWidht) / 2, buttonY = inputY + inputHeight + this->getSpacing();
    doneButton->setGeometry(buttonX, buttonY, buttonWidht, buttonHeight);

    if (hasDrawing) {
        int imageWidth = this->getAvaliableWidth() * 0.9, imageHeight = this->getAvaliableHeight() * 0.85;
        int imageX = (this->width() - imageWidth) / 2, imageY = buttonY + buttonHeight + getSpacing();
        imageLabel->setGeometry(imageX, imageY, imageWidth, imageHeight);
    }
}

void GameSentenceFrame::openImage(QString imageData) {
    if (tempFile->open()) {
        QByteArray bytes = QByteArray::fromHex(imageData.toLocal8Bit());
        tempFile->write(bytes);
        tempFile->flush();
        tempFile->close();
    }
}

void GameSentenceFrame::onDoneButtonClicked() {
    QString sentence = sentenceInput->text();

    if (sentence.isEmpty()) {
        sentence = " ";
    }

    emit sendSentence(sentence);
}
