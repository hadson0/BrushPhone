#include "roundframe.h"

RoundFrame::RoundFrame(QString sentence, QString drawingData, QWidget *parent)
    : BackgroundFrame{parent} {
    // Sentence label
    sentenceLabel = new CustomLabel(sentence, this);

    // Image label
    imageLabel = new QLabel(this);
    QPixmap image = getImagePixmap(drawingData);
    imageLabel->setPixmap(image);
}

QPixmap RoundFrame::getImagePixmap(QString imageData) {
    QTemporaryFile tempFile("./tempXXXX.png");

    if (tempFile.open()) {
        QByteArray bytes = QByteArray::fromHex(imageData.toLocal8Bit());
        tempFile.write(bytes);
        tempFile.flush();

        QPixmap image(tempFile.fileName());
        tempFile.close();
        return image;
    }

    return QPixmap();
}

void RoundFrame::resizeEvent(QResizeEvent *event) {
     Q_UNUSED(event);

    // Sentence label
    int labelWidth = this->getAvaliableWidth() * 0.75, labelHeight = this->getAvaliableHeight() * 0.15;
    int labelX = (this->width() - labelWidth) / 2, labelY = this->getPadding();
    sentenceLabel->setGeometry(labelX, labelY, labelWidth, labelHeight);

    // Image label
    int imageWidth = this->getAvaliableWidth() * 0.85, imageHeight = this->getAvaliableHeight() * 0.8;
    int imageX = (this->width() - imageWidth) / 2, imageY = this->height() - imageHeight - this->getPadding();
    imageLabel->setGeometry(imageX, imageY, imageWidth, imageHeight);
}
