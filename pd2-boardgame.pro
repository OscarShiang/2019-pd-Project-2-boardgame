#-------------------------------------------------
#
# Project created by QtCreator 2019-04-22T17:30:38
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pd2-boardgame
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    animatepixmap.cpp \
    cursor.cpp \
    main.cpp \
    game.cpp \
    radio.cpp \
    textinput.cpp \
    tile.cpp \
    button.cpp \
    editbox.cpp \
    animaterect.cpp \
    animatetext.cpp

HEADERS += \
    animatepixmap.h \
    cursor.h \
    game.h \
    radio.h \
    textinput.h \
    tile.h \
    button.h \
    editbox.h \
    animaterect.h \
    animatetext.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
