#include "training.h"


TYPE error_single(NeuralNetwork& nn, const values_t& input, const values_t& expected_output)
{
    values_t output;
    nn.process(input, output);
    TYPE res = abs(output - expected_output);
    return res*res/2;
}

TYPE error(NeuralNetwork& nn, const train_set& set, int batch_size)
{
    if(batch_size == -1)
        batch_size = set.input.size();

    TYPE avg_error = 0.0;
    TYPE weight_decay = 0.0;

    //avg error
    for(int i=0; i<batch_size; i++){
        avg_error += error_single(nn, set.input[i], set.output[i]);
    }
    avg_error = avg_error/set.input.size();

    //weight decay
    if(set.weight_decay != 0){
        TYPE tmp;
        for(int layer=0; layer<nn.net.size(); layer++){
            for(int neuron=0; neuron<nn.net[layer].size(); neuron++){
                for(int con=0; con<nn.net[layer][neuron].size(); con++){
                    tmp = nn.net[layer][neuron][con];
                    weight_decay += tmp*tmp ;
                }
            }
        }
    }
    //printf("%f %f\n",avg_error,weight_decay);

    return avg_error + set.weight_decay*weight_decay/2;
}

TYPE transform(TYPE value, TYPE lower_from, TYPE upper_from, TYPE lower_to, TYPE upper_to)
{
    return ((value - lower_from)/(upper_from - lower_from)) *(upper_to - lower_to) + lower_to;
}


TYPE transform(TYPE value, TYPE lower, TYPE upper)
{
    return value*(upper-lower) + lower;
}


void back_propagation(NeuralNetwork& nn, train_set& set, int batch_size, int offset)
{
    //algorithm from http://ufldl.stanford.edu/tutorial/supervised/MultiLayerNeuralNetworks/

    if(batch_size == -1)
        batch_size = set.input.size();

    set.progress = 0;
    size_t num_lyrs, num_neurons, num_connections;
    int m;

    std::vector<values_matrix_t> delta_W;
    values_matrix_t delta_b;

    num_lyrs = nn.net.size();
    delta_W.resize(num_lyrs);
    delta_b.resize(num_lyrs);
    //size delta_W/b
    for(int lyr=0; lyr<num_lyrs; lyr++){
        num_neurons = nn.net[lyr].size();
        delta_W[lyr].resize(num_neurons);
        delta_b[lyr].resize(num_neurons);
        for(int neu=0; neu<num_neurons; neu++){
            num_connections = nn.net[lyr][neu].size();
            delta_W[lyr][neu].resize(num_connections);
        }
    }

    for(; set.progress < set.iterations; set.progress++){
        //normal algorithm:

        //prep delta W, delta b
        for(int lyr=0; lyr<num_lyrs; lyr++){
            num_neurons = nn.net[lyr].size();

            for(int neu=0; neu<num_neurons; neu++){
                num_connections = nn.net[lyr][neu].size();
                delta_b[lyr][neu] = 0.0;

                for(int con=0; con<num_connections; con++){
                    delta_W[lyr][neu][con] = 0.0;
                }
            }
        }


        //training calculations
        if(offset + batch_size >= set.input.size()){
            offset = 0;
        }

        for(m=0; m<batch_size; m++){
            back_propagate(nn, set.input[offset+m], set.output[offset+m], delta_W, delta_b);
        }
        offset += batch_size;

        //update parameters
        num_lyrs = nn.net.size();
        for(int lyr=0; lyr<num_lyrs; lyr++){
            num_neurons = nn.net[lyr].size();

            for(int neu=0; neu<num_neurons; neu++){
                num_connections = nn.net[lyr][neu].size();
                nn.net[lyr][neu].bias -= set.learn_rate * delta_b[lyr][neu]/batch_size;//see step above, m=input.size()

                for(int con=0; con<num_connections; con++){
                    nn.net[lyr][neu][con] -= set.learn_rate * (delta_W[lyr][neu][con]/batch_size  + set.weight_decay*nn.net[lyr][neu][con]  );
                }
            }
        }

    }
}


void back_propagation_instant(NeuralNetwork& nn, train_set& set)
{
    //algorithm from http://ufldl.stanford.edu/tutorial/supervised/MultiLayerNeuralNetworks/

    set.progress = 0;
    size_t num_lyrs, num_neurons, num_connections;

    std::vector<values_matrix_t> delta_W;
    values_matrix_t delta_b;

    num_lyrs = nn.net.size();
    delta_W.resize(num_lyrs);
    delta_b.resize(num_lyrs);
    //size delta_W/b
    for(int lyr=0; lyr<num_lyrs; lyr++){
        num_neurons = nn.net[lyr].size();
        delta_W[lyr].resize(num_neurons);
        delta_b[lyr].resize(num_neurons);
        for(int neu=0; neu<num_neurons; neu++){
            num_connections = nn.net[lyr][neu].size();
            delta_W[lyr][neu].resize(num_connections);
        }
    }

    for(; set.progress < set.iterations; set.progress++){
        //alternative algorithm:

        for(int m=0; m<set.input.size(); m++){
            for(int lyr=0; lyr<num_lyrs; lyr++){
                num_neurons = nn.net[lyr].size();

                for(int neu=0; neu<num_neurons; neu++){
                    num_connections = nn.net[lyr][neu].size();
                    delta_b[lyr][neu] = 0.0;

                    for(int con=0; con<num_connections; con++){
                        delta_W[lyr][neu][con] = 0.0;
                    }
                }
            }

            back_propagate(nn, set.input[m], set.output[m], delta_W, delta_b);

            num_lyrs = nn.net.size();
            for(int lyr=0; lyr<num_lyrs; lyr++){
                num_neurons = nn.net[lyr].size();

                for(int neu=0; neu<num_neurons; neu++){
                    num_connections = nn.net[lyr][neu].size();
                    nn.net[lyr][neu].bias -= set.learn_rate * delta_b[lyr][neu];

                    for(int con=0; con<num_connections; con++){
                        nn.net[lyr][neu][con] -= set.learn_rate * (delta_W[lyr][neu][con]  + set.weight_decay*nn.net[lyr][neu][con]  );
                    }
                }
            }
        }

    }
}



void back_propagate(NeuralNetwork& nn, const values_t& input, const values_t& expected_output, std::vector<values_matrix_t>& delta_W, values_matrix_t& delta_b)
{
    //algorithm from http://ufldl.stanford.edu/tutorial/supervised/MultiLayerNeuralNetworks/
    //Neuron(W) -> z -> func() -> a
    //input -> NN -> output/y

    //step 1
    values_t output;
    nn.process(input, output);

    values_t& prev = output;
    TYPE tmp;
    int lyr;
    int num_lyrs, num_neurons, num_connections;
    values_matrix_t error;

    error.resize(nn.net.size());

    //step 2
    //output layer first
    lyr = error.size()-1;
    num_neurons = nn.net[lyr].size();
    error[lyr].resize(num_neurons);
    for(int n=0; n<num_neurons; n++){
        error[lyr][n] = func_deriv( func_inv(output[n]) ); //->f'(z)
        error[lyr][n] *= (output[n] - expected_output[n]); //-> -(y-a) = (a-y)
    }
    prev = error[lyr];

    //step 3
    //from output layer to layer 1 (layer after the input layer), but counting from output-1 to 0
    for(; --lyr >=0;){
        num_neurons = nn.net[lyr].size();
        error[lyr].resize(num_neurons);

        for(int n=0; n<num_neurons; n++){
            error[lyr][n] = func_deriv( func_inv( nn.net[lyr][n].value ) ); //->f'(z)
            tmp = 0;
            for(int j=0; j<prev.size(); j++){ //-> W^T*error[lyr+1] = W^T*prev
                tmp += prev[j] * nn.net[lyr+1][j][n];
            }
            error[lyr][n] *= tmp;
        }

        prev = error[lyr];
    }


    //step 4
    //add to delta W, delta b
    num_lyrs = error.size();
    for(lyr=0; lyr<num_lyrs; lyr++){
        num_neurons = error[lyr].size();

        for(int neu=0; neu<num_neurons; neu++){
            num_connections = nn.net[lyr][neu].size();

            delta_b[lyr][neu] += error[lyr][neu];
            for(int con=0; con<num_connections; con++){
                //nn.net[lyr-1] -> lyr before current layer
                //nn.net[lyr-1][con] -> specific neuron before current layer (num_neurons of [lyr-1] = num_connections of [lyr])
                if(lyr == 0)
                    delta_W[lyr][neu][con] += error[lyr][neu]*input[con];
                else
                    delta_W[lyr][neu][con] += error[lyr][neu]*nn.net[lyr-1][con].value;
            }
        }
    }

}





void linear_regression(NeuralNetwork& nn, train_set& t)
{
    if(nn.net.size() != 1) return;

    if(t.input.size() != t.output.size())
        throw "Training set misaligned!";

    values_t output_tmp(nn.num_output,0);

    for(int k=0; k<t.iterations; k++){
        for(int set=0; set<t.input.size(); set++){
            nn.process(t.input[set], output_tmp);

            //NP Buch S.266
            for(int i=0; i<nn.num_output; i++){
                nn.net[0][i].connections += (t.learn_rate/abs(t.input[set])) * (t.output[set][i] - output_tmp[i])*t.input[set];
            }
        }
    }
}


void logistic_regression(NeuralNetwork& nn, train_set& t){
    if(nn.net.size() != 1 || nn.net[0].size() != 2) return;

    if(t.input.size() != t.output.size())
        throw "Training set misaligned!";

    //TODO
}



void softmax_regression(NeuralNetwork& nn, train_set& t){
    if(nn.net.size() != 1) return;

    if(t.input.size() != t.output.size())
        throw "Training set misaligned!";

    //TODO
}



