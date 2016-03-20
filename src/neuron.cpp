#include "neuron.h"

Neuron::Neuron(double default_val, int size)
{
    value = 0.0;
    connections.resize(size);
    set_connections(default_val);
}


Neuron::~Neuron()
{

}


void Neuron::calc_value(layer_t& layer)
{
#ifndef OPTIMIZED
    if(layer.size() != connections.size()){
        throw "Invalid fire sizes!";
    }
#endif

    value = 0.0;

    for(int i=0; i<layer.size(); i++){
        value += layer[i].value * connections[i];
    }

}




void Neuron::set_value(double val)
{
    value=val;
}



double Neuron::get_value()
{
    return value;
}



void Neuron::set_connections(double default_val)
{
    for(int i=0; i<connections.size(); i++){
        connections[i] = default_val;
    }
}



void Neuron::set_connection_at(double value, int connection)
{
    connections[connection] = value;
}



void Neuron::randomize_connections(double min, double max)
{
    for(int i=0; i<connections.size(); i++){
        connections[i] = (max - min) * ( (double)rand() / (double)RAND_MAX ) + min;
    }
}



void Neuron::print()
{
    for(int i=0; i<connections.size(); i++){
        printf("%.3f\t", connections[i]);
    }
    printf(", value at %.3f\n", value);
}





//serialization
void Neuron::write_to(std::FILE* stream)
{
    fprintf(stream, "size: %lu,", connections.size());
    for(int i=0; i<connections.size(); i++){
        fprintf(stream, " %lf", connections[i]);
    }
    fprintf(stream, "\n");
}

void Neuron::read_from(std::FILE* stream)
{
    size_t size = 4;
    fscanf(stream, "size: %lu,", &size);
    connections.resize(size);
    for(size_t i=0; i<size; i++){
        fscanf(stream, " %lf", &connections[i]);
    }
    fscanf(stream, "\n");
}

std::ostream& operator<<(std::ostream& os, const Neuron& n)
{
    //value doesnt need to be saved
    //os << n.value;
    os << n.connections.size();
    os << " ";
    for(int i=0; i<n.connections.size(); i++){
        os << n.connections[i] << " ";
    }
    os << "\n";
    return os;
}

std::istream& operator>>(std::istream& is, Neuron& n)
{
    //value doesnt need to be saved
    //os >> n.value;
    n.value = 0.0;
    uint64_t size;
    is >> size;
    n.connections.resize(size);
    for(int i=0; i<size; i++){
        is >> n.connections[i];
    }
    //if( /* T could not be constructed */ )
    //    is.setstate(std::ios::failbit);
    return is;
}







