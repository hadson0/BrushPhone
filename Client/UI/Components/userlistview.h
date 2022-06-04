#ifndef CLIENTLISTVIEW_H
#define CLIENTLISTVIEW_H

#include "backgroundframe.h"
#include "userlistviewitem.h"

class UserListView : public BackgroundFrame {
    Q_OBJECT

    QMap<QString, UserListViewItem *> userMap;

    void paintEvent(QPaintEvent *event);

public:
    explicit UserListView(QWidget *parent = nullptr);

    void addUser(QString userNick);
    void removeUSer(QString userNick);

public slots:
    void setReady(QString userNick, bool ready);
    void onUserListChanged(QStringList newUserList);
    void onReadyListChanged(QStringList newReadyList);
};

#endif // CLIENTLISTVIEW_H
