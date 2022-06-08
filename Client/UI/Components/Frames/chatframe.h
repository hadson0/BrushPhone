#ifndef CHATFRAME_H
#define CHATFRAME_H

#include <QFile>

#include "backgroundedframe.h"
#include "UI/Components/Widgets/customtextedit.h"
#include "UI/Components/Widgets/custompushbutton.h"

class ChatFrame : public BackgroundedFrame {
    Q_OBJECT

    // Widgets
    CustomTextEdit *chatView, *chatInput;
    CustomPushButton *sendMessageButton;

    void resizeEvent(QResizeEvent *event);

public:
    explicit ChatFrame(QWidget *parent = nullptr);

    bool readHistoric();
    bool saveHistoric();

public slots:
    void onSendButtonClicked();
    void onMessageRecieved(QString message, QString senderNick);

signals:
    void sendMessage(QString message);
};

#endif // CHATFRAME_H
