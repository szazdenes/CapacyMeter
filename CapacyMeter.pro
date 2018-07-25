#-------------------------------------------------
#
# Project created by QtCreator 2018-07-25T12:43:48
#
#-------------------------------------------------

QT       += core gui svg multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CapacyMeter
TEMPLATE = app
DESTDIR = ./bin
OBJECTS_DIR = ./obj
MOC_DIR = ./moc
RCC_DIR = ./rcc
UI_DIR = ./ui

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
        main.cpp \
        mainwindow.cpp \
    measurementform.cpp \
    plotform.cpp

HEADERS += \
        mainwindow.h \
    measurementform.h \
    plotform.h

FORMS += \
        mainwindow.ui \
    measurementform.ui \
    plotform.ui

INCLUDEPATH += ./qwt/build/include \
./SoundCardReader


LIBS += ./qwt/build/lib/libqwt.a \
./SoundCardReader/bin/libSoundCardReader.a
