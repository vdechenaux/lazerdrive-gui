#-------------------------------------------------
#
# Project created by QtCreator 2016-01-08T22:30:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lazerdrive-gui
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/QLazerDriveClient/release/ -lQLazerDriveClient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/QLazerDriveClient/debug/ -lQLazerDriveClient
else:unix: LIBS += -L$$PWD/QLazerDriveClient/ -lQLazerDriveClient

INCLUDEPATH += $$PWD/QLazerDriveClient
DEPENDPATH += $$PWD/QLazerDriveClient
