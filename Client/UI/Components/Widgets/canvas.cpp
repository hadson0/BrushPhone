#include "canvas.h"

Canvas::Canvas(QWidget *parent)
    : QWidget{parent}, penColor(Qt::black), backgroundColor(Qt::white), isDrawing(false), penWidth(5) {
    startPoint = endPoint = QPoint();
    // Image
    image = QImage(this->size(),  QImage::Format_ARGB32);
}

void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(image.rect(), backgroundColor); // Otherwise the background would be transparent
    painter.drawImage(0, 0, image);
    event->accept();
}

void Canvas::resizeEvent(QResizeEvent *event) {
    image = QImage(this->size(),  QImage::Format_ARGB32);
    clearImage();
    event->accept();
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        startPoint = event->pos();
        endPoint = event->pos();
        isDrawing = true;
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) && isDrawing) {
        endPoint = event->pos();
        this->draw();
        isDrawing = false;
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) && isDrawing) {
        endPoint = event->pos();
        this->draw();
    }
}

void Canvas::draw() {
    // Painter setup
    QPainter pantier(&image);
    QPen pen(penColor, penWidth, Qt:: SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pantier.setPen(pen);

    // Draws a line from the start point to the endpoint
    pantier.drawLine(startPoint, endPoint);
    startPoint = endPoint;

    this->update();
}

void Canvas::setPenColor(QColor &color) { penColor = color; }

void Canvas::setPenWidth(int width) { penWidth = width; }

QColor Canvas::getPenColor() const { return penColor; }

int Canvas::getPenWidth() const { return penWidth; }

void Canvas::clearImage() {
    image.fill(backgroundColor);
    update();
}

bool Canvas::saveImage() {
    QImage drawing = image;
    QTemporaryFile file("tempXXXXXX.png");
    if (file.open()) {
        drawing.save(file.fileName());

        QByteArray drawingData = file.readAll();
        file.write(drawingData);

        drawing = QImage(this->size(),  QImage::Format_ARGB32);

        emit sendDrawing(drawingData.toHex());
        return true;
    } else {
        return false;
    }
}
