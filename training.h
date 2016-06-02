#ifndef TRAINING_H
#define TRAINING_H

#include "types.h"
#include "neuralnetwork.h"

typedef struct train_set{
    values_matrix_t input;
    values_matrix_t output;
    double learn_rate;
    int iterations;
} train_set;


#define LAMBDA 1.0

double error_single(NeuralNetwork& nn, const values_t& input, const values_t& expected_output);
double error(NeuralNetwork& nn, const train_set& set);

void back_propagation(NeuralNetwork& nn, const train_set& set);
void back_propagate(NeuralNetwork& nn, const values_t& input, const values_t& expected_output, std::vector<values_matrix_t>& delta_W, values_matrix_t& delta_b);

// bias should be 0.0
void linear_regression(NeuralNetwork& nn, train_set& t);/* simple training algorithm for 2 layer NNs */

#endif // TRAINING_H
