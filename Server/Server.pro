QT -= gui
QT += websockets

CONFIG += c++17 console
CONFIG -= app_bundle


SOURCES += \
        game.cpp \
        lobby.cpp \
        main.cpp \
        messageprocesshandler.cpp \
        servermanager.cpp \
        user.cpp \
        websockethandler.cpp
        main.cpp \

HEADERS += \
        game.h \
        lobby.h \
        messageprocesshandler.h \
        servermanager.h \
        user.h \
        websockethandler.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
