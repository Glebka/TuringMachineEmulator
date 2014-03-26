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
    TuringIO.cpp \
    errortablemodel.cpp \
    addcolumndialog.cpp

HEADERS  += mainwindow.h \
    common.h \
    functionalschememodel.h \
    alphabetmodel.h \
    commands.h \
    tapemodel.h \
    TuringIO.h \
    errortablemodel.h \
    addcolumndialog.h

FORMS    += mainwindow.ui \
    addcolumndialog.ui

RESOURCES += \
    resources.qrc
CCFLAG = -std=c++11 -O0 -ggdb3 -Wall -Wextra -pedantic
