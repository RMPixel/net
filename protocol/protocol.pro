#-------------------------------------------------
#
# Project created by QtCreator 2019-04-09T13:36:09
#
#-------------------------------------------------

QT       -= gui
CONFIG += c++11 console

TARGET = protocol
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        protocol.cpp

HEADERS += \
        protocol.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}


unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libzmq


win32: LIBS += -L$$PWD/../libs/zeromq_4.3.1/mingw64/lib/ -llibzmq.dll

INCLUDEPATH += $$PWD/../libs/zeromq_4.3.1/mingw64/include
DEPENDPATH += $$PWD/../libs/zeromq_4.3.1/mingw64/include
