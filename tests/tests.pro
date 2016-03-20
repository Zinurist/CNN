#-------------------------------------------------
#
# Project created by QtCreator 2016-03-19T18:45:20
#
#-------------------------------------------------

QT += core gui
QT += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CNNtest
TEMPLATE = app

#INCLUDEPATH += $$PWD/../src

SOURCES += \
    main.cpp \
    testneuron.cpp

HEADERS += \
    testneuron.h


SOURCES += ../src/mainwindow.cpp \
    ../src/neuralnetwork.cpp \
    ../src/neuron.cpp

HEADERS  += ../src/mainwindow.h \
    ../src/neuralnetwork.h \
    ../src/neuron.h \
    ../src/types.h

FORMS    += ../src/mainwindow.ui
