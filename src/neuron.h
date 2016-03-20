#ifndef NEURON_H
#define NEURON_H

#include "types.h"

#include <iostream>
#include <inttypes.h>


class Neuron
{
public:
    Neuron(double default_val, int size);
    ~Neuron();

    void calc_value(layer_t& layer);
    void set_value(double val);
    double get_value();

    void set_connections(double default_val);
    void set_connection_at(double value, int connection);
    void randomize_connections(double min, double max);

    void print();

    //serialization
    void Neuron::write_to(std::FILE* stream);
    void Neuron::read_from(std::FILE* stream);
    friend std::ostream& operator<<(std::ostream& os, const Neuron& n);
    friend std::istream& operator>>(std::istream& is, Neuron& n);

//private:
    double value;
    values_t connections;

};



#endif // NEURON_H
