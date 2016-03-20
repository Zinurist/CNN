
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = CNN
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    neuralnetwork.cpp \
    neuron.cpp

HEADERS  += mainwindow.h \
    neuralnetwork.h \
    neuron.h \
    types.h

FORMS    += mainwindow.ui
