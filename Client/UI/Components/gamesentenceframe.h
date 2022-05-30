#ifndef GAMESENTENCEFRAME_H
#define GAMESENTENCEFRAME_H

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

    void resizeEvent(QResizeEvent *event);

public:
    explicit GameSentenceFrame(QWidget *parent = nullptr);

public slots:
    void onDoneButtonClicked();

signals:
    void sendSentence(QString sentence);
};

#endif // GAMESENTENCEFRAME_H
