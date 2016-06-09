#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "neuron.h"
#include "types.h"

#include <fstream>
#include <iostream>


class NeuralNetwork
{
public:
    NeuralNetwork(TYPE default_val, const std::vector<int>& structure);
    NeuralNetwork(const std::vector<int>& structure);

    void process(const values_t& input, values_t& output);
    values_t* process(const values_t& input);
    void process_from(int layer, values_t& output);

    void randomize_values(int seed, TYPE min, TYPE max);
    void set_values(TYPE default_val);
    void set_bias(TYPE default_val);
    //void set_value_at(TYPE value, int layer, int neuron);

    void print() const;

    size_t num_input;
    size_t num_output;

    //serialization
    void write_to(std::FILE* stream) const;
    void read_from(std::FILE* stream);
    //friend declaration functions as forward declaration!
    friend bool operator==(const NeuralNetwork& n1, const NeuralNetwork& n2);
    friend std::ostream& operator<<(std::ostream& os, const NeuralNetwork& n);
    friend std::istream& operator>>(std::istream& is, NeuralNetwork& n);

//private:
    network_t net;

};

#endif // NEURALNETWORK_H
