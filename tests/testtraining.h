#ifndef TESTTRAINING_H
#define TESTTRAINING_H

#include "../src/neuralnetwork.h"
#include "../src/training.h"
#include "../src/types.h"

#include "print.h"

#include <QObject>
#include <QtTest/QtTest>

class TestTraining : public QObject
{
    Q_OBJECT
public:
    explicit TestTraining(QObject *parent = 0);

signals:

private slots:
    void test_linear_regression();
    void test_error();
    void test_back_propagation_simple();
    void test_back_propagation_random();
};

#endif // TESTTRAINING_H
