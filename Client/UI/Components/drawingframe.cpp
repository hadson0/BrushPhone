#include "drawingframe.h"

DrawingFrame::DrawingFrame(QString sentence, QWidget *parent)
    : BackgroundFrame{parent} {
    setPadding(20);

    // Label
    label = new CustomLabel("Draw the sentence: " + sentence, this);

    // Canvas
    canvas = new Canvas(this);
    connect(canvas, &Canvas::sendDrawing, this, &DrawingFrame::sendDrawing);

    // Done button
    doneButton = new CustomPushButton("Done", this);
    connect(doneButton, &QPushButton::clicked, this, &DrawingFrame::onDoneButtonClicked);

    // Clear button
    clearButton = new CustomPushButton("Clear", this);
    connect(clearButton, &QPushButton::clicked, canvas, &Canvas::clearImage);

    // Width button
    widthButton = new CustomPushButton("Width", this);
    connect(widthButton, &QPushButton::clicked, this, &DrawingFrame::setPenWidth);

    // Color button
    colorButton = new CustomPushButton("Color", this);
    connect(colorButton, &QPushButton::clicked, this, &DrawingFrame::setPenColor);
}

void DrawingFrame::onDoneButtonClicked() {
    canvas->saveImage();
}

void DrawingFrame::setPenWidth() {
    bool ok;
    int width = QInputDialog::getInt(this, "Brush Phone", "Select pen width:",
                                        canvas->getPenWidth(), 1, 30, 1, &ok);
    if (ok) {
        canvas->setPenWidth(width);
    }
}

void DrawingFrame::setPenColor() {
    QColor color = QColorDialog::getColor(canvas->getPenColor(), this,
                                          "Brush Phone", QColorDialog::DontUseNativeDialog);
    if (color.isValid()) {
        canvas->setPenColor(color);
    }
}

void DrawingFrame::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
    setSpacing(10);

    // Label
    int labelWidth = this->getAvaliableWidth(), labelHeight = this->getAvaliableHeight() * 0.15;
    int labelX = this->getPadding(), labelY = labelX;
    label->setGeometry(labelX, labelY, labelWidth, labelHeight);

    // Done nutton
    int buttonWidht = this->getAvaliableWidth() * 0.1, buttonHeight = this->getAvaliableHeight() * 0.12;
    int buttonX = this->getPadding(), buttonY = labelY + labelHeight + this->getSpacing();
    doneButton->setGeometry(buttonX, buttonY, buttonWidht, buttonHeight);

    int oldSpacing = this->getSpacing();
    int newSpacing = (this->getAvaliableHeight() - buttonY - 4 * buttonHeight) / 3;
    setSpacing(newSpacing);

    // Clear nutton
    buttonY +=  this->getSpacing() + buttonHeight + 7;
    clearButton->setGeometry(buttonX, buttonY, buttonWidht, buttonHeight);

    // Width nutton
    buttonY += this->getSpacing() + buttonHeight + 7;
    widthButton->setGeometry(buttonX, buttonY, buttonWidht, buttonHeight);

    // Color nutton
    buttonY += this->getSpacing() + buttonHeight + 7;
    colorButton->setGeometry(buttonX, buttonY, buttonWidht, buttonHeight);

    setSpacing(oldSpacing);

    // Canvas
    int canvasWidht = this->getAvaliableWidth() - buttonWidht - this->getPadding();
    int canvasHeight = this->getAvaliableHeight() - labelHeight - this->getSpacing();
    int canvasX = buttonWidht +  2 * this->getPadding(), canvasY = labelY + labelHeight + this->getSpacing();
    canvas->setGeometry(canvasX, canvasY, canvasWidht, canvasHeight);
}
