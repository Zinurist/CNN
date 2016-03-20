
#include "mainwindow.h"
#include "neuralnetwork.h"
#include "types.h"

#include <time.h>
#include <fstream>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    int testo[] = {3,5,3};
    std::vector<int> structure (testo, testo+3);
    NeuralNetwork network (1.0, structure);
    network.randomize_values(time(0), -1.0, 1.0);
    values_t ass;
    ass.resize(3);
    for(int i=0; i<ass.size(); i++){
        ass[i] = 1.0;
    }
    network.process(ass,ass);
    for(int i=0; i<ass.size(); i++){
        printf("%f\n", ass[i]);
    }

    network.print();

    return a.exec();
}
