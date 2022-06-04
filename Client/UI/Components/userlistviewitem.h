#ifndef USERLISTVIEWITEM_H
#define USERLISTVIEWITEM_H

#include "backgroundframe.h"
#include "customlabel.h"

class UserListViewItem : public BackgroundFrame {
    Q_OBJECT

    CustomLabel *nicknameLabel;
    QColor defaultColor, readyColor;
    bool ready;

    void resizeEvent(QResizeEvent *event);

public:
    explicit UserListViewItem(QString nickname, QWidget *parent = nullptr);

    // Getters
    bool isReady();

    // Setters
    void setReady(bool ready);

    // Methods
    void setHighlighted();
};

#endif // USERLISTVIEWITEM_H
