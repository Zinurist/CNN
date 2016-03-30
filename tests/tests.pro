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
    testneuron.cpp \
    testneuralnetwork.cpp \
    testtypes.cpp \
    testtraining.cpp

HEADERS += \
    testneuron.h \
    testneuralnetwork.h \
    testtypes.h \
    print.h \
    testtraining.h


SOURCES += ../src/mainwindow.cpp \
    ../src/neuralnetwork.cpp \
    ../src/neuron.cpp \
    ../src/convolutionalneuralnetwork.cpp \
    ../src/types.cpp \
    ../src/training.cpp

HEADERS  += ../src/mainwindow.h \
    ../src/neuralnetwork.h \
    ../src/neuron.h \
    ../src/convolutionalneuralnetwork.h \
    ../src/types.h \
    ../src/training.h

FORMS    += ../src/mainwindow.ui
