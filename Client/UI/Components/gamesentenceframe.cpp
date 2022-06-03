#include "gamesentenceframe.h"

GameSentenceFrame::GameSentenceFrame(QString drawingData, QWidget *parent)
    : BackgroundedFrame{parent}, hasDrawing(false) {
    setPadding(10);
    setSpacing(10);

    // Label
    QString displayText;
    if (hasDrawing) {
        displayText = "Describe the drawing!";
    } else {
        displayText = "Start writing a sentence!";
    }
    label = new CustomLabel(displayText, this);

    // Sentence input
    sentenceInput = new CustomLineEdit(this);
    connect(sentenceInput, &QLineEdit::returnPressed, this, &GameSentenceFrame::onDoneButtonClicked);

    // Done button
    doneButton = new CustomPushButton("Done", this);
    connect(doneButton, &QPushButton::clicked, this, &GameSentenceFrame::onDoneButtonClicked);

    // Gambiarra
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

    if (hasDrawing) {
        // Image label
        int imageWidth = this->getAvaliableWidth() * 0.85, imageHeight = this->getAvaliableHeight() * 0.8;
        int imageX = (this->width() - imageWidth) / 2, imageY = this->height() - imageHeight - this->getPadding();
        imageLabel->setGeometry(imageX, imageY, imageWidth, imageHeight);

        // Sentence input
        int inputWidth = this->getAvaliableWidth() * 0.85, inputHeight = this->getAvaliableHeight() * 0.08;
        int inputX = this->getPadding(), inputY = imageY - inputHeight - this->getSpacing();
        sentenceInput->setGeometry(inputX, inputY, inputWidth, inputHeight);

        // Done nutton
        int buttonWidht = this->getAvaliableWidth() * 0.12, buttonHeight = inputHeight;
        int buttonX = this->width() - buttonWidht -  this->getPadding(), buttonY = inputY;
        doneButton->setGeometry(buttonX, buttonY, buttonWidht, buttonHeight);

        // Label
        int labelWidth = this->getAvaliableWidth() * 0.75, labelHeight = this->getAvaliableHeight() * 0.07;
        int labelX = (this->width() - labelWidth) / 2, labelY = inputY - labelHeight - this ->getSpacing();
        label->setGeometry(labelX, labelY, labelWidth, labelHeight);

    } else {
        // Label
        int labelWidth = this->getAvaliableWidth(), labelHeight = this->getAvaliableHeight() * 0.15;
        int labelX = this->getPadding(), labelY = (this->height() / 2) - labelHeight - 50;
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
