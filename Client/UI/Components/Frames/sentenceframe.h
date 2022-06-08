#ifndef SENTENCEFRAME_H
#define SENTENCEFRAME_H

#include <QPixmap>
#include <QLabel>
#include <QTemporaryFile>

#include "backgroundedframe.h"
#include "UI/Components/Widgets/custompushbutton.h"
#include "UI/Components/Widgets/customlineedit.h"
#include "UI/Components/Widgets/customlabel.h"

class SentenceFrame : public BackgroundedFrame {
    Q_OBJECT

    // Widgets
    CustomLabel *label;
    CustomLineEdit *sentenceInput;
    CustomPushButton *doneButton;
    QLabel *imageLabel;

    QTemporaryFile *tempFile;
    bool hasDrawing;

    void resizeEvent(QResizeEvent *event);

    void openImage(QString imageData);

public:
    explicit SentenceFrame(QString drawingData, QWidget *parent = nullptr);

public slots:
    void onDoneButtonClicked();

signals:
    void sendSentence(QString sentence);
};

#endif // SENTENCEFRAME_H
