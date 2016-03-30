#include "testneuron.h"

TestNeuron::TestNeuron(QObject *parent) : QObject(parent)
{

}


void TestNeuron::test_write_read_neuron()
{

    srand(time(0));

    Neuron n1(0,4);
    Neuron n2(1,6);
    Neuron n3(5,10);
    Neuron n4(6,3);


    n1.randomize_connections(-5.0, 5.0);
    FILE * f;

    f = fopen("testo.txt", "w");
    n1.write_to(f);
    fclose(f);

    f = fopen("testo.txt", "r");
    n2.read_from(f);
    fclose(f);

    if(NEURON_WRN){
        n1.print();
        n2.print();
    }
    QVERIFY(n1 == n2);



    n3.randomize_connections(-5.0,5.0);
    std::ifstream is;
    std::ofstream os;

    os.open("testo2.txt", std::fstream::out);
    os << n3;
    os.close();

    is.open("testo2.txt", std::fstream::in);
    is >> n4;
    is.close();

    if(NEURON_WRN){
        n3.print();
        n4.print();
    }
    QVERIFY(n3 == n4);


}
