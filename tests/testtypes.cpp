#include "testtypes.h"

TestTypes::TestTypes(QObject *parent) : QObject(parent)
{

}


void TestTypes::test_vector_abs()
{
    values_t a(4,2.0);
    values_t b(4,0.0);
    values_t c(4,-2.0);

    QVERIFY(abs(abs(a) - 4.0) < EPSILON);
    QVERIFY(abs(abs(b) - 0.0) < EPSILON);
    QVERIFY(abs(abs(c) - 4.0) < EPSILON);

}

void TestTypes::test_vector_arithmic()
{
    values_t a(4,1);
    values_t b(4,1);//not 0! for division
    values_t c(4,0);
    values_t e(4,0);

    for(int i=0; i<a.size(); i++){
        e[i] = a[i] + b[i];
    }
    c = a+b;
    for(int i=0; i<c.size(); i++){
        QVERIFY(abs(c[i] - e[i]) < EPSILON);
    }

    for(int i=0; i<a.size(); i++){
        e[i] = a[i] - b[i];
    }
    c = a-b;
    for(int i=0; i<c.size(); i++){
        QVERIFY(abs(c[i] - e[i]) < EPSILON);
    }

    for(int i=0; i<a.size(); i++){
        e[i] = a[i] * b[i];
    }
    c = a*b;
    for(int i=0; i<c.size(); i++){
        QVERIFY(abs(c[i] - e[i]) < EPSILON);
    }

    for(int i=0; i<a.size(); i++){
        e[i] = a[i] / b[i];
    }
    c = a/b;
    for(int i=0; i<c.size(); i++){
        QVERIFY(abs(c[i] - e[i]) < EPSILON);
    }


    double z = 3;

    for(int i=0; i<a.size(); i++){
        e[i] = z + a[i];
    }
    c = z+a;
    for(int i=0; i<c.size(); i++){
        QVERIFY(abs(c[i] - e[i]) < EPSILON);
    }

    for(int i=0; i<a.size(); i++){
        e[i] = z - a[i];
    }
    c = z-a;
    for(int i=0; i<c.size(); i++){
        QVERIFY(abs(c[i] - e[i]) < EPSILON);
    }

    for(int i=0; i<a.size(); i++){
        e[i] = z * a[i];
    }
    c = z*a;
    for(int i=0; i<c.size(); i++){
        QVERIFY(abs(c[i] - e[i]) < EPSILON);
    }

    for(int i=0; i<a.size(); i++){
        e[i] = z / a[i];
    }
    c = z/a;
    for(int i=0; i<c.size(); i++){
        QVERIFY(abs(c[i] - e[i]) < EPSILON);
    }



}
