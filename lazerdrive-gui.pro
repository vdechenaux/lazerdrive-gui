#-------------------------------------------------
#
# Project created by QtCreator 2016-01-08T22:30:57
#
#-------------------------------------------------

QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lazerdrive-gui
TEMPLATE = app


SOURCES += main.cpp\
    gamearea.cpp \
    usernamedialog.cpp \
    replaydialog.cpp

HEADERS  += \
    gamearea.h \
    usernamedialog.h \
    replaydialog.h

FORMS    += \
    usernamedialog.ui \
    replaydialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/QLazerDriveClient/release/ -lQLazerDriveClient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/QLazerDriveClient/debug/ -lQLazerDriveClient
else:unix: LIBS += -L$$PWD/QLazerDriveClient/ -lQLazerDriveClient

INCLUDEPATH += $$PWD/QLazerDriveClient
DEPENDPATH += $$PWD/QLazerDriveClient

RESOURCES += \
    resources.qrc
