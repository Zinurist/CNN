#include "neuralnetwork.h"

#include <time.h>
#include <inttypes.h>
#include <math.h>

NeuralNetwork::NeuralNetwork()
{
    net.resize(0);
    num_input = 0;
    num_output = 0;
}

NeuralNetwork::NeuralNetwork(TYPE default_val, const std::vector<int>& structure)
{
    if(structure.size() < 1) throw "Invalid structure!";

    //use resize: already adds elements (layer_ts)
    net.resize(structure.size()-1);

    for(int i=0; i<structure.size()-1; i++){
        //use reserve: doesnt add elements (neurons)
        net[i].reserve(structure[i+1]);

        for(int k=0; k<structure[i+1]; k++){
            net[i].push_back( Neuron(default_val, structure[i]) );
        }
    }

    num_input = structure[0];
    num_output = structure[structure.size()-1];

}

NeuralNetwork::NeuralNetwork(const std::vector<int>& structure) : NeuralNetwork(0.0, structure)
{
    randomize_values((int)time(0), -0.1, 0.1);
}


//FORWARD FEEDING

void NeuralNetwork::process(const values_t& input, values_t& output)
{
#ifndef OPTIMIZED
    if(input.size() != num_input){
        throw "Invalid input size!";
    }
#endif

    if(output.size() != num_output){
        output.resize(num_output);
    }

    //feed input
    for(int k=0; k<net[0].size(); k++){
        net[0][k].feed_forward(input);
    }

    //fire
    for(int i=1; i<net.size(); i++){
        for(int k=0; k<net[i].size(); k++){
            net[i][k].feed_forward(net[i-1]);
        }
    }

    //write to output
    for(int k=0; k<num_output; k++){
        output[k] = net[net.size()-1][k].value;
    }

}


values_t* NeuralNetwork::process(const values_t& input)
{
    values_t* output = new values_t;
    output->resize(num_output);
    process(input, *output);
    return output;
}


void NeuralNetwork::process_from(int layer, values_t& output)
{
    if(layer <=0) return;

    if(output.size() != num_output){
        output.resize(num_output);
    }

    //fire
    for(int i=layer; i<net.size(); i++){
        for(int k=0; k<net[i].size(); k++){
            net[i][k].feed_forward(net[i-1]);
        }
    }

    //last fire to output
    for(int k=0; k<num_output; k++){
        output[k] = net[net.size()-1][k].value;
    }

}


//BACKWARD FEEDING

void NeuralNetwork::reverse_process(const values_t& input, values_t& output)
{
#ifndef OPTIMIZED
    if(input.size() != num_output){
        throw "Invalid input size!";
    }
#endif

    if(output.size() != num_input){
        output.resize(num_input);
    }

    size_t last = net.size()-1;

    //feed input
    for(int k=0; k<net[last].size(); k++){
        net[last][k].value = input[k];

    }

    //backward feeding
    for(int i=last; i>0; i--){
        for(int k=0; k<net[i-1].size(); k++){
            net[i-1][k].value = 0;
        }
        for(int k=0; k<net[i].size(); k++){
            net[i][k].reverse_activate();
        }
        //TODO solve LGS net[i-1]*x = net[i]
    }

    //last fire to output
    for(int k=0; k<num_input; k++){
        output[k] = 0;
    }
    for(int k=0; k<net[0].size(); k++){
        net[0][k].reverse_activate();
    }
    //TODO solve LGS output*x = net[0]

}


values_t* NeuralNetwork::reverse_process(const values_t& input)
{
    values_t* output = new values_t;
    output->resize(num_input);
    reverse_process(input, *output);
    return output;
}



//OTHER

void NeuralNetwork::randomize_values(int seed, TYPE min, TYPE max)
{
    srand(seed);
    for(int i=0; i<net.size(); i++){
        for(int k=0; k<net[i].size(); k++){
            net[i][k].randomize_connections(min, max);
        }
    }
}



void NeuralNetwork::set_values(TYPE default_val)
{
    for(int i=0; i<net.size(); i++){
        for(int k=0; k<net[i].size(); k++){
            net[i][k].set_connections(default_val);
        }
    }
}

void NeuralNetwork::set_bias(TYPE default_val)
{
    for(int i=0; i<net.size(); i++){
        for(int k=0; k<net[i].size(); k++){
            net[i][k].bias = default_val;
        }
    }
}

void NeuralNetwork::print() const
{
    printf("Printing neural network:\n");
    printf("Layer 0:\t%lu input neurons\n", num_input);
    for(int i=1; i<net.size(); i++){
        printf("Layer %i:\n", i);
        for(int k=0; k<net[i].size(); k++){
            printf("\tNeuron %i: ", k);
            net[i][k].print();
        }
    }
}

//SERIALIZATION

void NeuralNetwork::write_to(std::FILE* stream) const
{
    fprintf(stream, "layers: %lu\n", net.size());
    fprintf(stream, "%lu\n", num_input);
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

    fscanf(stream, "%lu\n", &size_l);
    num_input = size_l;

    for(int i=0; i<size; i++){
        fscanf(stream, "%lu\n", &size_l);

        net[i].resize(size_l);
        for(int k=0; k<size_l; k++){
            net[i][k].read_from(stream);
        }

    }
    fscanf(stream, "\n");

    num_output = net[net.size()-1].size();
}

bool operator==(const NeuralNetwork& n1, const NeuralNetwork& n2)
{
    if(n1.net.size() != n2.net.size())
        return false;

    if(n1.num_input != n2.num_input || n1.num_output != n2.num_output)
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
    os << n.num_input << " ";
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
    size_t size;
    size_t size_l;

    is >> size;
    n.net.resize(size);
    is >> n.num_input;
    for(int i=0; i<size; i++){
        is >> size_l;
        n.net[i].resize(size_l);
    }

    for(int i=0; i<size; i++){
        for(int k=0; k<n.net[i].size(); k++){
            is >> n.net[i][k];
        }
    }

    n.num_output = n.net[n.net.size()-1].size();
    //if( /* T could not be constructed */ )
    //    is.setstate(std::ios::failbit);
    return is;
}



//BUILDERS

void NeuralNetwork::readjust_connections(int layer)
{
    //change size of neurons to fit previous layer/num_input
    if(layer == 0){
        for(int k=0; k<net[layer].size(); k++)
            net[layer][k].connections.resize(num_input);
    }else{
        for(int k=0; k<net[layer].size(); k++)
            net[layer][k].connections.resize(net[layer-1].size());
    }

    //change size of neurons in next layer to fit the new layer (or num_output, if there is no next layer)
    if(layer == net.size()-1){
        num_output = net[layer].size();
    }else{
        for(int k=0; k<net[layer+1].size(); k++)
            net[layer+1][k].connections.resize(net[layer].size());
    }
}

void NeuralNetwork::add_layer(size_t size)
{
    add_layer(size, net.size());
}

void NeuralNetwork::add_layer(size_t size, int layer)
{
    layer_t lyr;
    lyr.resize(size);
    add_layer(lyr, layer);
}

void NeuralNetwork::add_layer(const layer_t& lyr)
{
    add_layer(lyr, net.size());
}

void NeuralNetwork::add_layer(const layer_t& lyr, int layer)
{
    /* alternative:
    if(index > net.size()) index == net.size();
    if(index < 0) index = 0;
    */
    if(layer > net.size()) throw "Invalid index!";
    if(layer < 0) throw "Invalid index!";

    net.reserve(net.size()+1);
    network_t::iterator it = net.begin();
    net.insert(it+layer, lyr);

    readjust_connections(layer);
}

void NeuralNetwork::remove_layer()
{
    remove_layer(net.size()-1);
}

void NeuralNetwork::remove_layer(int layer)
{
    if(layer >= net.size()) throw "Invalid layer!";
    if(layer < 0) throw "Invalid layer!";

    network_t::iterator it = net.begin();
    net.erase(it+layer);

    layer = layer==0? layer:layer-1;
    readjust_connections(layer);
}

void NeuralNetwork::resize_layer(size_t size, int layer)
{
    //check bounds
    if(layer >= net.size()) throw "Invalid layer!";
    if(layer < 0) throw "Invalid layer!";

    //check if resize is necessary
    size_t prev = net[layer].size();
    if(size == prev) return;

    net[layer].resize(size);

    readjust_connections(layer);
}

void NeuralNetwork::add_neuron(int layer)
{
    Neuron n;
    add_neuron(layer, n);
}

void NeuralNetwork::add_neuron(int layer, const Neuron& n)
{
    //check bounds
    if(layer >= net.size()) throw "Invalid layer!";
    if(layer < 0) throw "Invalid layer!";

    net[layer].reserve(net[layer].size()+1);//maybe let vector handle reserving?
    net[layer].push_back(n);

    readjust_connections(layer);
}

void NeuralNetwork::remove_neuron(int layer, int neuron)
{
    //check bounds
    if(layer >= net.size()) throw "Invalid layer!";
    if(layer < 0) throw "Invalid layer!";

    if(neuron >= net[layer].size()) throw "Invalid index!";
    if(neuron < 0) throw "Invalid index!";

    //copy last neuron to index, then delete last neuron by resizing layer
    size_t last = net[layer].size()-1;
    if(neuron != last) net[layer][neuron] = net[layer][last];
    resize_layer(last, layer);
}

void NeuralNetwork::set_input_size(size_t size)
{
    //can be allowed
    //if(size == 0) throw "Invalid size!";
    num_input = size;
    if(net.size() == 0) return;
    for(int k=0; k<net[0].size(); k++)
        net[0][k].connections.resize(num_input);
}

void NeuralNetwork::set_output_size(size_t size)
{
    //can be allowed
    //if(size == 0) throw "Invalid size!";
    num_output = size;
    if(net.size() == 0) return;
    net[net.size()-1].resize(num_output);
}






//SOFTMAX

void NeuralNetworkSoftmax::process(const values_t& input, values_t& output)
{
#ifndef OPTIMIZED
    if(input.size() != num_input){
        throw "Invalid input size!";
    }
#endif

    if(output.size() != num_output){
        output.resize(num_output);
    }

    //feed input
    if(net.size() > 1){
        for(int k=0; k<net[0].size(); k++){
            net[0][k].feed_forward(input);
        }
    }

    //fire
    int i;
    for(i=1; i<net.size()-1; i++){
        for(int k=0; k<net[i].size(); k++){
            net[i][k].feed_forward(net[i-1]);
        }
    }

    double sum = 0;
    for(int k=0; k<net[i].size(); k++){
        net[i][k].feed(net[net.size()-1]);
        net[i][k].value = exp(net[i][k].value);
        sum += net[i][k].value;
    }

    for(int k=0; k<net[i].size(); k++){
        net[i][k].value /= sum;
    }

    //write to output
    for(int k=0; k<num_output; k++){
        output[k] = net[net.size()-1][k].value;
    }

}






