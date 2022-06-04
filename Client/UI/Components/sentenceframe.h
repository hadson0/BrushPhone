#ifndef SENTENCEFRAME_H
#define SENTENCEFRAME_H

#include <QPixmap>
#include <QLabel>
#include <QTemporaryFile>

#include "backgroundframe.h"
#include "UI/Components/custompushbutton.h"
#include "UI/Components/customlineedit.h"
#include "UI/Components/customlabel.h"

class SentenceFrame : public BackgroundFrame {
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
