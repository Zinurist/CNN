#include "neuralnetwork.h"
#include "training.h"
#include "mnist.h"

#include <ctime>

int main(int argc, char *argv[]){
    mnist* data = load();

    int testo_mnist[] = {28*28,28*28/4,10};
    std::vector<int> structure_mnist (testo_mnist, testo_mnist+3);
    NeuralNetwork nn(1.0, structure_mnist);
    nn.randomize_values(time(0), -1.0, 1.0);

    train_set set;
    set.iterations = 100;
    set.learn_rate = 0.2;
    set.weight_decay = 0.0001;

    to_train_set(set, data->training_set, data->training_labels);

    std::cout << "Starting!" << std::endl;
    
    clock_t begin = clock();
    double err = error(nn, set, 300*100);
    back_propagation(nn, set, 300);
    err = err-error(nn, set, 300*100);
    clock_t end = clock();

    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "Done! " << err << ", " << elapsed_secs << "s" << std::endl;
	return 0;
}
