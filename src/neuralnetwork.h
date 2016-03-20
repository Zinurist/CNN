#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "neuron.h"
#include "types.h"

class NeuralNetwork
{
public:
    NeuralNetwork(double default_val, const std::vector<int>& structure);
    ~NeuralNetwork();

    void process(values_t& input, values_t& output);
    void randomize_values(int seed, double min, double max);
    void set_values(double default_val);
    void set_value_at(double value, int layer, int neuron);

    void print();

    int num_input;
    int num_output;
private:
    network_t neurons;

};

#endif // NEURALNETWORK_H
