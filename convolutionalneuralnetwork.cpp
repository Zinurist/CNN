#include "convolutionalneuralnetwork.h"

#include <math.h>
#include <inttypes.h>

ConvolutionalNeuralNetwork::ConvolutionalNeuralNetwork(const int* types, const int* widths, const int* heights, const int* pool_widths, const int* pool_heights, int size)
{
    cnn.resize(size);
    pool.resize(size);
    output_imgs.resize(size);
    output_pools.resize(size);
    conv_width.resize(size);
    conv_height.resize(size);

    std::vector<int> nets(2);
    for(int i=0; i<size; i++){
        conv_width[i] = widths[i];
        conv_height[i] = heights[i];

        nets[0] = heights[i]*widths[i];
        nets[1] = 1;
        cnn[i] = new NeuralNetwork(nets);

        if(i == size-1){
            nets[0] = 1;
            nets[1] = 1;
        }else{
            nets[0] = std::max(pool_widths[i+1], widths[i+1]);
            nets[1] = std::max(pool_heights[i+1], heights[i+1]);
        }

        pool[i] = create_pooling(types[i], nets[0], nets[1]);
    }

    calculate_minimum_size();
}

ConvolutionalNeuralNetwork::~ConvolutionalNeuralNetwork()
{
}

void ConvolutionalNeuralNetwork::calculate_minimum_size()
{
    minimum_width = 1-cnn.size();
    minimum_height = 1-cnn.size();
    for(int i=0; i<cnn.size(); i++){
        minimum_width += conv_width[i];
        minimum_height += conv_height[i];
    }
}


TYPE ConvolutionalNeuralNetwork::process(const values_matrix_t& img)
{
    if(img.size() < minimum_width || img[0].size() < minimum_height){
        throw "Invalid image size (too small)!";
    }

    values_t input;
    values_t output(1);
    //values_matrix_t* tmp_img;
    const values_matrix_t* input_img;
    //values_matrix_t* output_img;
    int output_width, output_height;

    //input_img = new values_matrix_t();
    //output_img = new values_matrix_t();
    input_img = &img;

    for(int i=0; i<cnn.size(); i++){
        input.resize(cnn[i]->num_input);

        output_height = (*input_img).size() - conv_height[i] + 1;
        output_width = (*input_img)[0].size() - conv_width[i] + 1;
        //output_img->resize(output_height);
        output_imgs[i].resize(output_height);

        for(int y=0; y < output_height; y++){
            //(*output_img)[y].resize(output_width);
            output_imgs[i][y].resize(output_width);

            for(int x=0; x < output_width; x++){

                //TODO optimise matrix->vec
                for(int iy=0; iy < conv_height[i]; iy++){
                    for(int ix=0; ix < conv_width[i]; ix++){
                        input[iy*conv_height[i] + ix] = (*input_img)[(iy+y)][ix+x];
                    }
                }

                cnn[i]->process(input, output);
                //(*output_img)[y][x] = output[0];
                output_imgs[i][y][x] = output[0];
            }
        }

        if(pool[i]){
            pool[i]->pool(output_imgs[i], output_pools[i]);
            input_img = &output_pools[i];
        }else{
            //tmp_img = input_img;
            //input_img = output_img;
            //output_img = tmp_img;
            input_img = &output_imgs[i];
        }

    }

    return (*input_img)[0][0];
}



void ConvolutionalNeuralNetwork::back_propagate(train_set& set)
{
    //propagate error through last network
    //propagate it to the input neurons
    //error (vector) at input neurons = error at pooling

    //propagate through pooling to find what part of the previous image is responsible

    //save values during convolution

}



void ConvolutionalNeuralNetwork::print() const
{
    printf("Printing convolutional neural network:\n");//TODO print pooling
    for(int c=0; c<cnn.size(); c++){
        printf("Network %i:\n", c);
        cnn[c]->print();
        printf("--------------------\n\n");
    }
}



//serialization
void ConvolutionalNeuralNetwork::write_to(std::FILE* stream) const
{
    fprintf(stream, "size: %lu\n", cnn.size());
    fprintf(stream, "pooling: \n");
    for(int i=0; i<cnn.size(); i++)
        fprintf(stream, "%i %i %i\n", pool[i]->type, pool[i]->width, pool[i]->height);//put this in Pooling?

    fprintf(stream, "\nconv_width: ");
    for(int i=0; i<cnn.size(); i++)
        fprintf(stream, "%i ", conv_width[i]);

    fprintf(stream, "\nconv_height: ");
    for(int i=0; i<cnn.size(); i++)
        fprintf(stream, "%i ", conv_height[i]);

    fprintf(stream, "\nnetworks: \n");
    for(int c=0; c<cnn.size(); c++)
        cnn[c]->write_to(stream);

    fprintf(stream, "\n");
}

void ConvolutionalNeuralNetwork::read_from(std::FILE* stream)
{
    size_t size = 0;
    int w, h, t;

    fscanf(stream, "size: %lu\n", &size);
    cnn.resize(size);
    pool.resize(size);
    conv_width.resize(size);
    conv_height.resize(size);

    fscanf(stream, "pooling: \n");
    for(int i=0; i<size; i++){
        fscanf(stream, "%i %i %i\n", &t, &w, &h);
        pool[i] = create_pooling(t, w, h);
    }

    fscanf(stream, "\nconv_width: ");
    for(int i=0; i<size; i++)
        fscanf(stream, "%i ", &conv_width[i]);

    fscanf(stream, "\nconv_height: ");
    for(int i=0; i<size; i++)
        fscanf(stream, "%i ", &conv_height[i]);

    fscanf(stream, "\nnetworks: \n");
    for(int i=0; i<size; i++)
        cnn[i]->read_from(stream);

    fscanf(stream, "\n");

    calculate_minimum_size();
}

bool operator==(const ConvolutionalNeuralNetwork& n1, const ConvolutionalNeuralNetwork& n2)
{
    if(n1.cnn.size() != n2.cnn.size())
        return false;

    for(int i=0; i<n1.cnn.size(); i++){
        if( !(*(n1.cnn[i]) == *(n2.cnn[i]) && *(n1.pool[i]) == *(n2.pool[i])) )
            return false;

        if(n1.conv_width[i] != n2.conv_width[i] || n1.conv_height[i] != n2.conv_height[i])
            return false;
    }

    return true;
}


std::ostream& operator<<(std::ostream& os, const ConvolutionalNeuralNetwork& n)
{
    os << n.cnn.size();
    os << "\n";
    for(int i=0; i<n.cnn.size(); i++)
        os << n.pool[i]->type << " " << n.pool[i]->width << " " << n.pool[i]->height << "\n";

    for(int i=0; i<n.cnn.size(); i++)
        os << n.conv_width[i] << " ";

    os << "\n";
    for(int i=0; i<n.cnn.size(); i++)
        os << n.conv_height[i] << " ";

    os << "\n";
    for(int i=0; i<n.cnn.size(); i++)
        os << *(n.cnn[i]);

    return os;
}

std::istream& operator>>(std::istream& is, ConvolutionalNeuralNetwork& n)
{
    size_t size;
    int t, w, h;

    is >> size;
    n.cnn.resize(size);
    n.pool.resize(size);
    n.conv_width.resize(size);
    n.conv_height.resize(size);
    for(int i=0; i<n.cnn.size(); i++){
        is >> t;
        is >> w;
        is >> h;
        n.pool[i] = create_pooling(t, w, h);
    }

    for(int i=0; i<n.cnn.size(); i++)
        is >> n.conv_width[i];
    for(int i=0; i<n.cnn.size(); i++)
        is >> n.conv_height[i];

    for(int i=0; i<n.cnn.size(); i++)
        is >> *(n.cnn[i]);

    n.calculate_minimum_size();

    //if( /* T could not be constructed */ )
    //    is.setstate(std::ios::failbit);
    return is;
}








