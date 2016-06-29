#ifndef CONVOLUTIONALNEURALNETWORK_H
#define CONVOLUTIONALNEURALNETWORK_H

#include "types.h"
#include "neuralnetwork.h"
#include "pooling.h"
#include "training.h"

#include <fstream>
#include <iostream>

class ConvolutionalNeuralNetwork
{
public:
    ConvolutionalNeuralNetwork(const int* types, const int* widths, const int* heights, const int* pool_widths, const int* pool_heights, int size);
    ~ConvolutionalNeuralNetwork();
    void calculate_minimum_size();

    TYPE process(const values_matrix_t& img);

    void back_propagate(train_set& set);

    void print() const;

    //serialization
    void write_to(std::FILE* stream) const;
    void read_from(std::FILE* stream);
    //friend declaration functions as forward declaration!
    friend bool operator==(const ConvolutionalNeuralNetwork& n1, const ConvolutionalNeuralNetwork& n2);
    friend std::ostream& operator<<(std::ostream& os, const ConvolutionalNeuralNetwork& n);
    friend std::istream& operator>>(std::istream& is, ConvolutionalNeuralNetwork& n);

private:
    std::vector<NeuralNetwork*> cnn;
    std::vector<Pooling*> pool;
    std::vector<values_matrix_t> output_imgs;
    std::vector<values_matrix_t> output_pools;

    std::vector<int> conv_width;
    std::vector<int> conv_height;

    int minimum_width, minimum_height;
};

#endif // CONVOLUTIONALNEURALNETWORK_H
