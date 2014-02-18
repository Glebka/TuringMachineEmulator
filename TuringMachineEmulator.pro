#-------------------------------------------------
#
# Project created by QtCreator 2014-01-21T19:06:41
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TuringMachineEmulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    functionalschememodel.cpp \
    alphabetmodel.cpp \
    commands.cpp \
    tapemodel.cpp \
    TuringIO.cpp

HEADERS  += mainwindow.h \
    common.h \
    functionalschememodel.h \
    alphabetmodel.h \
    commands.h \
    tapemodel.h \
    TuringIO.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
