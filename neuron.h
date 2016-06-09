#ifndef NEURON_H
#define NEURON_H

#include "types.h"

#include <fstream>
#include <iostream>


TYPE func(TYPE x);
TYPE func_deriv(TYPE x);//derivative
TYPE func_inv(TYPE x);//inverse

class Neuron
{
    friend class NeuralNetwork;
public:
    Neuron();
    Neuron(TYPE default_val, int size);
    Neuron(TYPE default_val, TYPE bias, int size);

    void calc_value(layer_t& layer);

    void set_connections(TYPE default_val);
    size_t size();
    TYPE& operator[](size_t index);
    /*
    void set_value(TYPE val);
    TYPE get_value() const;
    size_t get_size() const;
    void set_connection_at(int connection, TYPE value);
    TYPE get_connection_at(int connection) const;
    */
    void randomize_connections(TYPE min, TYPE max);

    void print() const;

    //serialization
    void write_to(std::FILE* stream) const;
    void read_from(std::FILE* stream);
    //friend declaration functions as forward declaration!
    friend bool operator==(const Neuron& n1, const Neuron& n2);
    friend std::ostream& operator<<(std::ostream& os, const Neuron& n);
    friend std::istream& operator>>(std::istream& is, Neuron& n);

//private:
    TYPE value;
    TYPE bias;
    values_t connections;

};



#endif // NEURON_H
