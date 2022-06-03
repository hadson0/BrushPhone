#include "game.h"

Game::Game(QObject *parent)
    : QObject{parent}, gamePhase(0) {}

int Game::getNextIndex(int x) {
    if (x == roundMap.size() - 1) {
        x = 0;
    } else {
        x++;
    }
    return x;
}

void Game::startGame(QStringList userList) {
    gamePhase = 0;
    stories.resize(userList.size());
    for (qsizetype i = 0; i < stories.size(); i++) {
        stories[i].resize(3);
        stories[i].fill({"", ""}); // Clears the vector
    }

    int count = 0;
    for (const QString &userNick : userList) {
        roundMap[userNick] = count++;
    }

    emit started();
}

void Game::advancePhase() {
    gamePhase++;

    if (gamePhase == roundMap.size()) {
        emit ended();
    } else {
        for (auto it = roundMap.begin(); it != roundMap.end(); it++) {
            it.value() = getNextIndex(it.value());
        }

        if (gamePhase % 2 == 0) {
            emit sentencePhase();
        } else {
            emit drawingPhase();
        }
    }
}

Round Game::getRound(int storyIndex, int roundIndex) {
    return stories[storyIndex][roundIndex];
}

QString Game::getSentence(QString userNick) {
    if (gamePhase % 2 != 0 && gamePhase != 0) {
        int storyIndex = roundMap[userNick];
        int roundIndex = (gamePhase - 1) / 2;

        return stories[storyIndex][roundIndex].first;
    }

    return "";
}

QString Game::getDrawing(QString userNick) {
    if (gamePhase % 2 == 0 && gamePhase != 0) {
        int storyIndex = roundMap[userNick];
        int roundIndex = (gamePhase - 1) / 2;

        return stories[storyIndex][roundIndex].second;
    }

    return "";
}

void Game::setSentence(QString userNick, QString sentence) {
    if (gamePhase % 2 == 0) {
        int storyIndex = roundMap[userNick];
        int roundIndex = gamePhase / 2;

        stories[storyIndex][roundIndex].first = sentence;

        bool allDone = true;
        for (qsizetype i = 0; i < stories.size(); i++) {
            if (stories[i][roundIndex].first.isEmpty()) {
                allDone = false;
            }
        }

        if (allDone) {            
            this->advancePhase();
        }
    }
}

void Game::setDrawing(QString userNick, QString drawingData) {
    if (gamePhase % 2 != 0) {
        int storyIndex = roundMap[userNick];
        int roundIndex = gamePhase / 2;

        stories[storyIndex][roundIndex].second = drawingData;

        bool allDone = true;
        for (qsizetype i = 0; i < stories.size(); i++) {
            if (stories[i][roundIndex].second.isEmpty()) {
                allDone = false;
            }
        }

        if (allDone) {
            this->advancePhase();
        }
    }
}
