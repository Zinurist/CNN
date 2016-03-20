#include "neuralnetwork.h"

NeuralNetwork::NeuralNetwork(double default_val, const std::vector<int>& structure)
{
    if(structure.size() < 2){
        throw "Invalid structure!";
    }

    //use resize: already adds elements (layer_ts)
    neurons.resize(structure.size());

    for(int k=0; k<structure[0]; k++){
        neurons[0].push_back( Neuron(0.0, 0) );
    }

    for(int i=1; i<structure.size(); i++){
        //use reserve: doesnt add elements (neurons)
        neurons[i].reserve(structure[i]);

        for(int k=0; k<structure[i]; k++){
            neurons[i].push_back( Neuron(default_val, structure[i-1]) );
        }
    }

    num_input = structure[0];
    num_output = structure[structure.size()-1];

}


NeuralNetwork::~NeuralNetwork()
{
}


void NeuralNetwork::process(values_t& input, values_t& output)
{
#ifndef OPTIMIZED
    if(input.size() != num_input || output.size() != num_output){
        throw "Invalid input/output sizes!";
    }
#endif

    //feed input
    for(int k=0; k<num_input; k++){
        neurons[0][k].set_value(input[k]);
    }

    //fire (note the -1!)
    for(int i=1; i<neurons.size(); i++){
        for(int k=0; k<neurons[i].size(); k++){
            neurons[i][k].calc_value(neurons[i-1]);
        }
    }

    //last fire to output
    for(int k=0; k<num_output; k++){
        output[k] = neurons[neurons.size()-1][k].get_value();
    }

}



void NeuralNetwork::randomize_values(int seed, double min, double max)
{
    srand(seed);
    for(int i=0; i<neurons.size(); i++){
        for(int k=0; k<neurons[i].size(); k++){
            neurons[i][k].randomize_connections(min, max);
        }
    }
}



void NeuralNetwork::set_values(double default_val)
{
    for(int i=0; i<neurons.size(); i++){
        for(int k=0; k<neurons[i].size(); k++){
            neurons[i][k].set_connections(default_val);
        }
    }
}



void NeuralNetwork::set_value_at(double value, int layer, int neuron)
{
    if(layer >= neurons.size()){
        throw "Invalid layer!";
    }

    if(neuron >= neurons[layer].size()){
        throw "Invalid neuron!";
    }

    neurons[layer][neuron].set_connections(value);
}




void NeuralNetwork::print()
{
    printf("Printing neural network:\n");
    printf("Layer 0:\t%i input neurons\n", num_input);
    for(int i=1; i<neurons.size(); i++){
        printf("Layer %i:\n", i);
        for(int k=0; k<neurons[i].size(); k++){
            printf("\tNeuron %i: ", k);
            neurons[i][k].print();
        }
    }
}






