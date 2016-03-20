#ifndef TESTNEURON_H
#define TESTNEURON_H

#include "../src/neuron.h"

#include <QObject>
#include <QtTest/QtTest>

class TestNeuron : public QObject
{
    Q_OBJECT
public:
    explicit TestNeuron(QObject *parent = 0);

signals:

private slots:
    void write_read_neuron();
};

#endif // TESTNEURON_H
