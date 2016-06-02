#ifndef NEURON_H
#define NEURON_H

#include "types.h"

#include <fstream>
#include <iostream>


double func(double x);
double func_deriv(double x);//derivative
double func_inv(double x);//inverse

class Neuron
{
    friend class NeuralNetwork;
public:
    Neuron();
    Neuron(double default_val, int size);
    Neuron(double default_val, double bias, int size);

    void calc_value(layer_t& layer);

    void set_connections(double default_val);
    size_t size();
    double& operator[](size_t index);
    /*
    void set_value(double val);
    double get_value() const;
    size_t get_size() const;
    void set_connection_at(int connection, double value);
    double get_connection_at(int connection) const;
    */
    void randomize_connections(double min, double max);

    void print() const;

    //serialization
    void write_to(std::FILE* stream) const;
    void read_from(std::FILE* stream);
    //friend declaration functions as forward declaration!
    friend bool operator==(const Neuron& n1, const Neuron& n2);
    friend std::ostream& operator<<(std::ostream& os, const Neuron& n);
    friend std::istream& operator>>(std::istream& is, Neuron& n);

//private:
    double value;
    double bias;
    values_t connections;

};



#endif // NEURON_H
