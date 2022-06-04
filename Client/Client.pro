QT += quick websockets gui
CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    Controllers/clientmanager.cpp \
    Controllers/messageprocesshandler.cpp \
    Controllers/websockethandler.cpp \
    UI/Components/backgroundframe.cpp \
    UI/Components/canvas.cpp \
    UI/Components/chatframe.cpp \
    UI/Components/customlabel.cpp \
    UI/Components/customlineedit.cpp \
    UI/Components/custompushbutton.cpp \
    UI/Components/customtextedit.cpp \
    UI/Components/drawingframe.cpp \
    UI/Components/roundframe.cpp \
    UI/Components/sentenceframe.cpp \
    UI/Components/nickinputdialog.cpp \
    UI/Components/timerwidget.cpp \
    UI/Components/userlistviewitem.cpp \
    UI/Components/userlistview.cpp \
    UI/Screens/drawingscreen.cpp \
    UI/Screens/sentencescreen.cpp \
    UI/Screens/joinlobbyscreen.cpp \
    UI/Screens/lobbyscreen.cpp \
    UI/Screens/roundscreen.cpp \
    UI/Screens/screen.cpp \
    UI/Screens/mainmenuscreen.cpp \
    UI/Screens/selectionscreen.cpp \
    UI/mainwindow.cpp \
    main.cpp \


HEADERS += \
    Controllers/clientmanager.h \
    Controllers/messageprocesshandler.h \
    Controllers/websockethandler.h \
    UI/Components/backgroundframe.h \
    UI/Components/canvas.h \
    UI/Components/chatframe.h \
    UI/Components/customlabel.h \
    UI/Components/customlineedit.h \
    UI/Components/custompushbutton.h \
    UI/Components/customtextedit.h \
    UI/Components/drawingframe.h \
    UI/Components/roundframe.h \
    UI/Components/sentenceframe.h \
    UI/Components/nickinputdialog.h \
    UI/Components/timerwidget.h \
    UI/Components/userlistviewitem.h \
    UI/Components/userlistview.h \
    UI/Screens/drawingscreen.h \
    UI/Screens/sentencescreen.h \
    UI/Screens/joinlobbyscreen.h \
    UI/Screens/lobbyscreen.h \
    UI/Screens/roundscreen.h \
    UI/Screens/screen.h \
    UI/Screens/mainmenuscreen.h \
    UI/Screens/selectionscreen.h \
    UI/mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
