#include "testneuron.h"


TestNeuron::TestNeuron(QObject *parent) : QObject(parent)
{

}


void TestNeuron::write_read_neuron()
{

    Neuron n(0,4);
    Neuron n2(1,4);


    n.randomize_connections(-5.0, 5.0);
    FILE * f;

    f = fopen("testo.txt", "w");
    n.write_to(f);
    fclose(f);

    f = fopen("testo.txt", "r");
    n2.read_from(f);
    fclose(f);

    n.print();
    n2.print();
    QVERIFY(n.connections.size() == n2.connections.size());

    /*
    std::fstream fs;
    fs.open("testo2.txt", std::fstream::in|std::fstream::out|std::fstream::binary);
    n.randomize_connections(-5.0,5.0);
    n.print();
    fs << n;
    fs.seekg(0);
    fs >> n2;
    n2.print();
    fs.close();*/

}
