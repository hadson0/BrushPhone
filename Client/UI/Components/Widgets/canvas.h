#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTemporaryFile>
#include <QDebug>

class Canvas : public QWidget {
    Q_OBJECT

    QColor penColor, backgroundColor;
    bool isDrawing;
    int penWidth;
    QPoint startPoint, endPoint;
    QImage image;

    void paintEvent(QPaintEvent *event) ;
    void resizeEvent(QResizeEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void draw();

public:
    explicit Canvas(QWidget *parent = nullptr);

    void setPenColor(QColor &color);
    void setPenWidth(int newWidth);

    QColor getPenColor() const;
    int getPenWidth() const;

    bool saveImage();

public slots:
    void clearImage();

signals:
    void sendDrawing(QString drawingData);
};

#endif // CANVAS_H
