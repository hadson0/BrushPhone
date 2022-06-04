QT += quick websockets gui
CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    Controllers/clientmanager.cpp \
    Controllers/messageprocesshandler.cpp \
    Controllers/websockethandler.cpp \
    UI/Components/Frames/backgroundframe.cpp \
    UI/Components/Widgets/canvas.cpp \
    UI/Components/Frames/chatframe.cpp \
    UI/Components/Widgets/customlabel.cpp \
    UI/Components/Widgets/customlineedit.cpp \
    UI/Components/Widgets/custompushbutton.cpp \
    UI/Components/Widgets/customtextedit.cpp \
    UI/Components/Frames/drawingframe.cpp \
    UI/Components/Frames/roundframe.cpp \
    UI/Components/Frames/sentenceframe.cpp \
    UI/Components/nickinputdialog.cpp \
    UI/Components/Widgets/timerwidget.cpp \
    UI/Components/Widgets/userlistviewitem.cpp \
    UI/Components/Frames/userlistview.cpp \
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
    UI/Components/Frames/backgroundframe.h \
    UI/Components/Widgets/canvas.h \
    UI/Components/Frames/chatframe.h \
    UI/Components/Widgets/customlabel.h \
    UI/Components/Widgets/customlineedit.h \
    UI/Components/Widgets/custompushbutton.h \
    UI/Components/Widgets/customtextedit.h \
    UI/Components/Frames/drawingframe.h \
    UI/Components/Frames/roundframe.h \
    UI/Components/Frames/sentenceframe.h \
    UI/Components/nickinputdialog.h \
    UI/Components/Widgets/timerwidget.h \
    UI/Components/Widgets/userlistviewitem.h \
    UI/Components/Frames/userlistview.h \
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
