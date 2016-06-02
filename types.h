#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <stdlib.h>

#define values_t std::vector<double>
#define values_matrix_t std::vector<values_t>
#define layer_t std::vector<Neuron>
#define network_t std::vector<layer_t>

#define RAND_DOUBLE(min,max) ((max - min) * ( (double)rand() / (double)RAND_MAX ) + min)

#define EPSILON  1e-15

void print(values_t& v);

double abs(const values_t& a);
double sum(const values_t& a);

values_t operator+(const double a, const values_t& b);
values_t operator-(const double a, const values_t& b);
values_t operator*(const double a, const values_t& b);
values_t operator/(const double a, const values_t& b);

values_t operator+(const values_t& b, const double a);
values_t operator-(const values_t& b, const double a);
values_t operator*(const values_t& b, const double a);
values_t operator/(const values_t& b, const double a);

values_t operator+(const values_t& a, const values_t& b);
values_t operator-(const values_t& a, const values_t& b);
values_t operator*(const values_t& a, const values_t& b);
values_t operator/(const values_t& a, const values_t& b);

values_t& operator+=(values_t& a, const values_t& b);
values_t& operator-=(values_t& a, const values_t& b);
values_t& operator*=(values_t& a, const values_t& b);
values_t& operator/=(values_t& a, const values_t& b);

//remove for certain bound checks
#define OPTIMIZED

#endif // TYPES_H
