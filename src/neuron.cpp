#include "neuron.h"

#include <math.h>
#include <inttypes.h>
#include <iomanip>


double func(double x)
{
    //return x;

    //return std::tanh(x);

    return 1/(1+exp(-x));
}
double func_deriv(double x)
{
    //return 1;

    //x = tanh(x);
    //return 1.0 - x*x;

    return x*(1-x);
}
double func_inv(double x)
{
    //return x;

    //return std::atanh(x);

    return x;
}


Neuron::Neuron()
{
    value = 0.0;
    connections.resize(0);
}

Neuron::Neuron(double default_val, int size)
{
    value = 0.0;
    connections.resize(size);
    set_connections(default_val);
}

Neuron::Neuron(double default_val, double biasv, int size)
{
    value = 0.0;
    connections.resize(size);
    set_connections(default_val);
    bias = biasv;
}



void Neuron::calc_value(layer_t& layer)
{
#ifndef OPTIMIZED
    if(layer.size() != connections.size()){
        throw "Invalid fire sizes!";
    }
#endif

    value = bias;

    for(unsigned int i=0; i<layer.size(); i++){
        value += layer[i].value * connections[i];
    }

    value = func(value);
}


void Neuron::set_connections(double default_val)
{
    for(unsigned int i=0; i<connections.size(); i++){
        connections[i] = default_val;
    }
    bias = default_val;
}


size_t Neuron::size()
{
    return connections.size();
}

double& Neuron::operator[](size_t index)
{
    return connections[index];
}


/*
void Neuron::set_value(double val)
{
    value=val;
}
double Neuron::get_value() const
{
    return value;
}
size_t Neuron::get_size() const
{
    return connections.size();
}
void Neuron::set_connection_at(int connection, double value)
{
    connections[connection] = value;
}
double Neuron::get_connection_at(int connection) const
{
    return connections[connection];
}
*/


void Neuron::randomize_connections(double min, double max)
{
    for(unsigned int i=0; i<connections.size(); i++){
        connections[i] = RAND_DOUBLE(min, max);
    }
    bias = RAND_DOUBLE(min, max);
}



void Neuron::print() const
{
    for(unsigned int i=0; i<connections.size(); i++){
        printf("\t%.5f", connections[i]);
    }
    printf(", bias: %.5f, value at %.5f\n", bias, value);
}





//serialization
void Neuron::write_to(std::FILE* stream) const
{
    fprintf(stream, "size: %lu,", connections.size());
    for(unsigned int i=0; i<connections.size(); i++){
        fprintf(stream, " %.17lf", connections[i]);
    }
    fprintf(stream, ", bias: %.17lf", bias);

    fprintf(stream, "\n");
}

void Neuron::read_from(std::FILE* stream)
{
    size_t size = 0;
    fscanf(stream, "size: %lu,", &size);
    connections.resize(size);
    for(unsigned int i=0; i<size; i++){
        fscanf(stream, " %lf", &connections[i]);
    }
    fscanf(stream, ", bias: %lf", &bias);
    fscanf(stream, "\n");
}

bool operator==(const Neuron& n1, const Neuron& n2)
{
    if(n1.connections.size() != n2.connections.size())
        return false;

    for(unsigned int i=0; i<n1.connections.size(); i++){
        if(abs(n1.connections[i] - n2.connections[i]) > EPSILON)
            return false;
    }

    if(abs(n1.bias - n2.bias) > EPSILON)
        return false;

    return true;
}

std::istream& operator>>(std::istream& is, Neuron& n)
{
    //value doesnt need to be saved
    //is >> n.value;
    n.value = 0.0;
    uint64_t size;
    is >> size;
    n.connections.resize(size);
    for(unsigned int i=0; i<size; i++){
        is >> n.connections[i];
    }
    is >> n.bias;
    //if( /* T could not be constructed */ )
    //    is.setstate(std::ios::failbit);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Neuron& n)
{
    //value doesnt need to be saved
    //os << n.value;
    os << n.connections.size();
    os << " " << std::setprecision(17);
    for(unsigned int i=0; i<n.connections.size(); i++){
        os << n.connections[i] << " ";
    }
    os << n.bias;
    os << "\n";
    return os;
}









