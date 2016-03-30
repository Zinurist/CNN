#include "testneuralnetwork.h"

TestNeuralNetwork::TestNeuralNetwork(QObject *parent) : QObject(parent)
{

}


void TestNeuralNetwork::test_write_read_network()
{
    srand(time(0));

    int layers = rand()%20;
    std::vector<int> structure(layers, 0);
    for(int i=0; i<layers; i++){
        structure[i] = rand()%20;
    }
    std::vector<int> structure2 (3, 4);

    NeuralNetwork n1 (0.0, structure);
    NeuralNetwork n2 (1.0, structure2);
    NeuralNetwork n3 (0.0, structure);
    NeuralNetwork n4 (1.0, structure2);

    n1.randomize_values(time(0),-5.0, 5.0);
    FILE * f;

    f = fopen("testoNN.txt", "w");
    n1.write_to(f);
    fclose(f);

    f = fopen("testoNN.txt", "r");
    n2.read_from(f);
    fclose(f);

    if(NN_WRN){
        n1.print();
        n2.print();
    }
    QVERIFY(n1 == n2);


    n3.randomize_values(time(0),-5.0, 5.0);
    std::ifstream is;
    std::ofstream os;

    os.open("testoNN2.txt", std::fstream::out);
    os << n3;
    os.close();

    is.open("testoNN2.txt", std::fstream::in);
    is >> n4;
    is.close();

    if(NN_WRN){
        n3.print();
        n4.print();
    }
    QVERIFY(n3 == n4);

}




