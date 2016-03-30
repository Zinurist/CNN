
#include "testneuron.h"
#include "testneuralnetwork.h"
#include "testtypes.h"
#include "testtraining.h"

int main(){

    TestNeuron neuron;
    TestNeuralNetwork neuralnet;
    TestTypes types;
    TestTraining training;

    //QTest::qExec(&neuron);
    //QTest::qExec(&neuralnet);
    //QTest::qExec(&types);
    QTest::qExec(&training);

    return 0;
}
