#ifndef CHATFRAME_H
#define CHATFRAME_H

#include <QFile>

#include "backgroundframe.h"
#include "UI/Components/customtextedit.h"
#include "UI/Components/custompushbutton.h"

class ChatFrame : public BackgroundFrame {
    Q_OBJECT

    // Widgets
    CustomTextEdit *chatView, *chatInput;
    CustomPushButton *sendMessageButton;

    void resizeEvent(QResizeEvent *event);

    void readHistory();
        void saveHistory();
      //void watchHistoryChannel();

public:
    explicit ChatFrame(QWidget *parent = nullptr);
    ~ChatFrame();

public slots:
    void onSendButtonClicked();
    void onMessageRecieved(QString message, QString senderNick);

signals:
    void sendMessage(QString message);
};

#endif // CHATFRAME_H
