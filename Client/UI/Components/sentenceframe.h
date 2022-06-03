#ifndef GAMESENTENCEFRAME_H
#define GAMESENTENCEFRAME_H

#include <QPixmap>
#include <QLabel>
#include <QTemporaryFile>

#include "backgroundedframe.h"
#include "UI/Components/custompushbutton.h"
#include "UI/Components/customlineedit.h"
#include "UI/Components/customlabel.h"

class GameSentenceFrame : public BackgroundedFrame {
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
    explicit GameSentenceFrame(QString drawingData, QWidget *parent = nullptr);

public slots:
    void onDoneButtonClicked();

signals:
    void sendSentence(QString sentence);
};

#endif // GAMESENTENCEFRAME_H
