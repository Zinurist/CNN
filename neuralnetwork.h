#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "neuron.h"
#include "types.h"

#include <fstream>
#include <iostream>


class NeuralNetwork
{
public:
    NeuralNetwork();
    NeuralNetwork(TYPE default_val, const std::vector<int>& structure);
    NeuralNetwork(const std::vector<int>& structure);

    //process
    void process(const values_t& input, values_t& output);
    values_t* process(const values_t& input);
    void process_from(int layer, values_t& output);
    void reverse_process(const values_t& input, values_t& output);
    values_t* reverse_process(const values_t& input);

    //other
    void print() const;
    void randomize_values(int seed, TYPE min, TYPE max);
    void set_values(TYPE default_val);
    void set_bias(TYPE default_val);

    //build
    void readjust_connections(int layer);
    void add_layer(size_t size);
    void add_layer(size_t size, int layer);
    void add_layer(const layer_t& lyr);
    void add_layer(const layer_t& lyr, int layer);
    void remove_layer();
    void remove_layer(int layer);
    void resize_layer(size_t size, int layer);
    void add_neuron(int layer);
    void add_neuron(int layer, const Neuron& n);
    void remove_neuron(int layer, int neuron);
    void set_input_size(size_t size);
    void set_output_size(size_t size);



    //serialization
    void write_to(std::FILE* stream) const;
    void read_from(std::FILE* stream);
    //friend declaration functions as forward declaration!
    friend bool operator==(const NeuralNetwork& n1, const NeuralNetwork& n2);
    friend std::ostream& operator<<(std::ostream& os, const NeuralNetwork& n);
    friend std::istream& operator>>(std::istream& is, NeuralNetwork& n);

    //data
    network_t net;
    size_t num_input;
    size_t num_output;

};


class NeuralNetworkSoftmax : public NeuralNetwork
{
public:
    NeuralNetworkSoftmax() : NeuralNetwork() {}
    NeuralNetworkSoftmax(TYPE default_val, const std::vector<int>& structure) : NeuralNetwork(default_val, structure) {}
    NeuralNetworkSoftmax(const std::vector<int>& structure) : NeuralNetwork(structure) {}

    void process(const values_t& input, values_t& output);

};

#endif // NEURALNETWORK_H
