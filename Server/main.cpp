#include <QCoreApplication>
#include "servermanager.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    ClientManager::getInstance();

    return a.exec();
}
