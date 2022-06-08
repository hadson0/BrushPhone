#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QMap>
#include <QPair>
#include <QDebug>

#include <vector>

typedef QPair<QString, QString> Round; // <sentence, drawing>
typedef std::vector<Round> Story;

class Game : public QObject {
    Q_OBJECT

    int gamePhase; // even = write, odd = draw
    std::vector<Story> stories;
    QMap<QString, int> roundMap; // <userNick, storyIndex>

public:
    explicit Game(QObject *parent = nullptr);

    void startGame(QStringList &userList);
    void advancePhase();

    Round getRound(int storyIndex, int roundIndex) const;
    QString getSentence(QString userNick);

    QString getDrawing(QString userNick);

public slots:
    void setSentence(QString userNick, QString sentence);
    void setDrawing(QString userNick, QString drawingData);

signals:
    void started();
    void sentencePhase();
    void drawingPhase();
    void ended();
};

#endif // GAME_H
