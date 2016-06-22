#ifndef TRAINING_H
#define TRAINING_H

#include "types.h"
#include "neuralnetwork.h"

typedef struct train_set{
    values_matrix_t input;
    values_matrix_t output;
    TYPE learn_rate, weight_decay;
    int iterations, progress;
} train_set;


TYPE error_single(NeuralNetwork& nn, const values_t& input, const values_t& expected_output);
TYPE error(NeuralNetwork& nn, const train_set& set, int batch_size = -1);
TYPE transform(TYPE value, TYPE lower_from, TYPE upper_from, TYPE lower_to, TYPE upper_to);
TYPE transform(TYPE value, TYPE lower, TYPE upper);//standard from [0;1] -> [l;u]

void back_propagation(NeuralNetwork& nn, train_set& set,  int batch_size = -1, int offset = 0);
void back_propagation_instant(NeuralNetwork& nn, train_set& set);

void back_propagate(NeuralNetwork& nn, const values_t& input, const values_t& expected_output, std::vector<values_matrix_t>& delta_W, values_matrix_t& delta_b);
void back_propagate(NeuralNetworkSoftmax& nn, const values_t& input, const values_t& expected_output, std::vector<values_matrix_t>& delta_W, values_matrix_t& delta_b);

// bias should be 0.0
void linear_regression(NeuralNetwork& nn, train_set& t);/* simple training algorithm for 2 layer NNs */
void logistic_regression(NeuralNetwork& nn, train_set& t);
void softmax_regression(NeuralNetwork& nn, train_set& t);

#endif // TRAINING_H
