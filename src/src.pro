
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += warn_off

TARGET = CNN
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    neuralnetwork.cpp \
    neuron.cpp \
    convolutionalneuralnetwork.cpp \
    types.cpp \
    training.cpp

HEADERS  += mainwindow.h \
    neuralnetwork.h \
    neuron.h \
    types.h \
    convolutionalneuralnetwork.h \
    training.h

FORMS    += mainwindow.ui
