#include "neuralnetwork.h"
#include "training.h"
#include "mnist.h"

#include <cstdio>


int main(int argc, char *argv[]){
	printf("Starting!");
    mnist* data = load();

    int testo_mnist[] = {28*28,28*28/4,10};
    std::vector<int> structure_mnist (testo_mnist, testo_mnist+3);
    NeuralNetwork nn(1.0, structure_mnist);
    nn.randomize_values(time(0), -1.0, 1.0);

    train_set set;
    set.iterations = 1;
    set.learn_rate = 0.001;

    to_train_set(set, data->training_set, data->training_labels);

    back_propagation(nn, set);

	printf("Done!");
	return 0;
}
