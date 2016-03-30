#ifndef TESTNEURALNETWORK_H
#define TESTNEURALNETWORK_H

#include "../src/neuralnetwork.h"

#include "print.h"

#include <time.h>
#include <fstream>

#include <QObject>
#include <QtTest/QtTest>

class TestNeuralNetwork : public QObject
{
    Q_OBJECT
public:
    explicit TestNeuralNetwork(QObject *parent = 0);

signals:

private slots:
    void test_write_read_network();
};

#endif // TESTNEURALNETWORK_H
