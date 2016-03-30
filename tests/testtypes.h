#ifndef TESTTYPES_H
#define TESTTYPES_H

#include "../src/types.h"

#include "print.h"

#include <QObject>
#include <QtTest/QtTest>

class TestTypes : public QObject
{
    Q_OBJECT
public:
    explicit TestTypes(QObject *parent = 0);

signals:

private slots:
    void test_vector_abs();
    void test_vector_arithmic();

};

#endif // TESTTYPES_H
