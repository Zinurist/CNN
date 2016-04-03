#include "neuralnetwork.h"

#include <time.h>
#include <inttypes.h>



NeuralNetwork::NeuralNetwork(double default_val, const std::vector<int>& structure)
{
    if(structure.size() < 2) throw "Invalid structure!";

    //use resize: already adds elements (layer_ts)
    net.resize(structure.size());

    for(int k=0; k<structure[0]; k++){
        net[0].push_back( Neuron(0.0, 0) );
    }

    for(int i=1; i<structure.size(); i++){
        //use reserve: doesnt add elements (neurons)
        net[i].reserve(structure[i]);

        for(int k=0; k<structure[i]; k++){
            net[i].push_back( Neuron(default_val, structure[i-1]) );
        }
    }

    num_input = structure[0];
    num_output = structure[structure.size()-1];

}

NeuralNetwork::NeuralNetwork(const std::vector<int>& structure) : NeuralNetwork(0.0, structure)
{
    randomize_values((int)time(0), -0.1, 0.1);
}



void NeuralNetwork::process(const values_t& input, values_t& output)
{
#ifndef OPTIMIZED
    if(input.size() != num_input){
        throw "Invalid input/output sizes!";
    }
#endif

    if(output.size() != num_output){
        output.resize(num_output);
    }

    //feed input
    for(int k=0; k<num_input; k++){
        net[0][k].value = input[k];
    }

    //fire (note the -1!)
    for(int i=1; i<net.size(); i++){
        for(int k=0; k<net[i].size(); k++){
            net[i][k].calc_value(net[i-1]);
        }
    }

    //last fire to output
    for(int k=0; k<num_output; k++){
        output[k] = net[net.size()-1][k].value;
    }

}

values_t* NeuralNetwork::process(const values_t& input)
{
    values_t* output = new values_t;
    output->resize(input.size());
    process(input, *output);
    return output;
}

void NeuralNetwork::process_from(int layer, values_t& output)
{
    if(output.size() != num_output){
        output.resize(num_output);
    }

    //fire
    for(int i=layer; i<net.size(); i++){
        for(int k=0; k<net[i].size(); k++){
            net[i][k].calc_value(net[i-1]);
        }
    }

    //last fire to output
    for(int k=0; k<num_output; k++){
        output[k] = net[net.size()-1][k].value;
    }

}


void NeuralNetwork::randomize_values(int seed, double min, double max)
{
    srand(seed);
    for(int i=0; i<net.size(); i++){
        for(int k=0; k<net[i].size(); k++){
            net[i][k].randomize_connections(min, max);
        }
    }
}



void NeuralNetwork::set_values(double default_val)
{
    for(int i=0; i<net.size(); i++){
        for(int k=0; k<net[i].size(); k++){
            net[i][k].set_connections(default_val);
        }
    }
}

void NeuralNetwork::set_bias(double default_val)
{
    for(int i=0; i<net.size(); i++){
        for(int k=0; k<net[i].size(); k++){
            net[i][k].bias = default_val;
        }
    }
}


/*
void NeuralNetwork::set_value_at(double value, int layer, int neuron)
{
    if(layer >= net.size()){
        throw "Invalid layer!";
    }

    if(neuron >= net[layer].size()){
        throw "Invalid neuron!";
    }

    net[layer][neuron].set_connections(value);
}
*/




void NeuralNetwork::print() const
{
    printf("Printing neural network:\n");
    printf("Layer 0:\t%i input neurons\n", num_input);
    for(int i=1; i<net.size(); i++){
        printf("Layer %i:\n", i);
        for(int k=0; k<net[i].size(); k++){
            printf("\tNeuron %i: ", k);
            net[i][k].print();
        }
    }
}





//serialization
void NeuralNetwork::write_to(std::FILE* stream) const
{
    fprintf(stream, "layers: %lu\n", net.size());
    for(int i=0; i<net.size(); i++){
        fprintf(stream, "%lu\n", net[i].size());
        for(int k=0; k<net[i].size(); k++){
            net[i][k].write_to(stream);
        }
    }
    fprintf(stream, "\n");
}

void NeuralNetwork::read_from(std::FILE* stream)
{
    size_t size = 0;
    size_t size_l = 0;

    fscanf(stream, "layers: %lu\n", &size);
    net.resize(size);
    for(int i=0; i<size; i++){
        fscanf(stream, "%lu\n", &size_l);

        net[i].resize(size_l);
        for(int k=0; k<size_l; k++){
            net[i][k].read_from(stream);
        }

    }
    fscanf(stream, "\n");

    num_input = net[0].size();
    num_output = net[net.size()-1].size();
}

bool operator==(const NeuralNetwork& n1, const NeuralNetwork& n2)
{
    if(n1.net.size() != n2.net.size())
        return false;
    for(int i=0; i<n1.net.size(); i++){
        if(n1.net[i].size() != n2.net[i].size())
            return false;

        for(int k=0; k<n1.net[i].size(); k++){
            if(! (n1.net[i][k] == n2.net[i][k]) )
                return false;
        }
    }

    return true;
}

std::ostream& operator<<(std::ostream& os, const NeuralNetwork& n)
{
    os << n.net.size();
    os << " ";
    for(int i=0; i<n.net.size(); i++){
        os << n.net[i].size() << " ";
    }
    os << "\n";
    for(int i=0; i<n.net.size(); i++){
        for(int k=0; k<n.net[i].size(); k++){
            os << n.net[i][k];
        }
    }
    os << "\n";
    return os;
}

std::istream& operator>>(std::istream& is, NeuralNetwork& n)
{
    uint64_t size;
    uint64_t size_l;

    is >> size;
    n.net.resize(size);
    for(int i=0; i<size; i++){
        is >> size_l;
        n.net[i].resize(size_l);
    }

    for(int i=0; i<size; i++){
        for(int k=0; k<n.net[i].size(); k++){
            is >> n.net[i][k];
        }
    }

    n.num_input = n.net[0].size();
    n.num_output = n.net[n.net.size()-1].size();
    //if( /* T could not be constructed */ )
    //    is.setstate(std::ios::failbit);
    return is;
}





