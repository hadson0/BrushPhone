#include "user.h"

User::User(QString nickname, QObject *parent)
    : QObject{parent}, nickname(nickname), ready(false) {}

bool User::isReady() const { return ready; }

QString User::getNickname() const { return nickname; }

void User::toggleReady() { ready = !ready; }

void User::setNickname(QString nickName) { this->nickname = nickName; }
