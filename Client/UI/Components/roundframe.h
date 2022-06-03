#ifndef ROUNDFRAME_H
#define ROUNDFRAME_H

#include <QPixmap>
#include <QLabel>
#include <QTemporaryFile>
#include <QDebug>

#include "backgroundframe.h"
#include "UI/Components/customlabel.h"
#include "UI/Components/custompushbutton.h"

class RoundFrame : public BackgroundFrame {
    Q_OBJECT

    // Widgets
    CustomLabel *sentenceLabel;
    QLabel *imageLabel;

    QPixmap getImagePixmap(QString imageData);

    void resizeEvent(QResizeEvent *event);

public:
    explicit RoundFrame(QString sentence, QString drawingData, QWidget *parent = nullptr);
};

#endif // ROUNDFRAME_H
