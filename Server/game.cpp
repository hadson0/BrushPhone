#include "game.h"

Game::Game(QObject *parent)
    : QObject{parent}, gamePhase(0) {}

void Game::startGame(QStringList &userList) {
    int numbUsers = userList.size();
    gamePhase = 0;
    stories.resize(numbUsers);
    for (qsizetype i = 0; i < numbUsers; i++) {
        stories[i].resize(numbUsers / 2);
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
            if (it.value() == roundMap.size() - 1) {
                it.value() = 0;
            } else {
                it.value()++;
            }
        }

        if (gamePhase % 2 == 0) {
            emit sentencePhase();
        } else {
            emit drawingPhase();
        }
    }
}

// Retruns the sentence and the drawing of the respective round
Round Game::getRound(int storyIndex, int roundIndex) const {
    return stories[storyIndex][roundIndex];
}

// Gets the sentence that the user has to draw
QString Game::getSentence(QString userNick) {
    if (gamePhase % 2 != 0 && gamePhase != 0) {
        int storyIndex = roundMap[userNick];
        int roundIndex = gamePhase / 2;
        return stories[storyIndex][roundIndex].first;
    }

    return "";
}

// Gets the drawing that the user has to describe
QString Game::getDrawing(QString userNick) {
    if (gamePhase % 2 == 0 && gamePhase != 0) {
        int storyIndex = roundMap[userNick];
        int roundIndex = (gamePhase - 1) / 2; // The sentence describes the drawing of the previous round
        return stories[storyIndex][roundIndex].second;
    }

    return "";
}


// Sets the sentence in the stories
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

// Sets the drawing in the stories
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
