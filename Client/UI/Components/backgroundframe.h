#ifndef BACKGROUNDFRAME_H
#define BACKGROUNDFRAME_H

#include <QFrame>
#include <QPaintEvent>
#include <QPainter>

#include <QPainterPath>

class BackgroundFrame : public QFrame {
    Q_OBJECT    

    QColor color;
    QSize avaliableSize;
    int borderRadius, padding, spacing;

public:
    explicit BackgroundFrame(QWidget *parent = nullptr);

    // Getters
    QColor getColor();
    int getAvaliableWidth();
    int getAvaliableHeight();
    int getBorderRadius();
    int getPadding();
    int getSpacing();

    // Setters
    void setColor(QColor color);    
    void setBorderRadius(int borderRadius);
    void setPadding(int padding);
    void setSpacing(int spacing);

    // Methods
    void updateAvaliableSize();

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // BACKGROUNDFRAME_H
