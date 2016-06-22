#include "pooling.h"
#include "convolutionalneuralnetwork.h"


Pooling* create_pooling(int type, int width, int height)
{
    switch(type){
    case POOL_ID:
        return NULL;
    case POOL_MAX:
        return new PoolingMax(width, height);
    case POOL_MIN:
        return new PoolingMin(width, height);
    case POOL_AVG:
        return new PoolingAvg(width, height);
    default:
        return NULL;
    }
}

bool operator==(const Pooling& p1, const Pooling& p2)
{
    return (p1.type == p2.type && p1.width == p2.width && p1.height == p2.height);
}

Pooling::Pooling(int t, int w, int h)
{
    type = t;
    width = w;
    height = h;

    output_index_x.resize(w);
    output_index_y.resize(w);
    for(int i=0; i<w; i++){
        output_index_x[i].resize(h);
        output_index_y[i].resize(h);
    }
}

void PoolingMax::pool(const values_matrix_t& input, values_matrix_t& output)
{
    TYPE max;
    int conv_width = input.size()/width;
    int conv_height = input[0].size()/height;

    output.resize(width);

    for(int y=0; y < height; y++){

        output[y].resize(height);

        for(int x=0; x < width; x++){
            max = input[y][x];
            output_index_x[y][x] = 0;
            output_index_y[y][x] = 0;

            for(int iy=0; iy < conv_height; iy++){
                for(int ix=1; ix < conv_width; ix++){
                    if(input[iy][ix] > max){
                        max = input[iy][ix];
                        output_index_x[y][x] = ix;
                        output_index_y[y][x] = iy;
                    }
                }
            }

            output[y][x] = max;
        }
    }
}

void PoolingMin::pool(const values_matrix_t& input, values_matrix_t& output)
{
    TYPE min;
    int conv_width = input.size()/width;
    int conv_height = input[0].size()/height;

    output.resize(width);

    for(int y=0; y < height; y++){

        output[y].resize(height);

        for(int x=0; x < width; x++){
            min = input[y][x];
            output_index_x[y][x] = 0;
            output_index_y[y][x] = 0;

            for(int iy=0; iy < conv_height; iy++){
                for(int ix=1; ix < conv_width; ix++){
                    if(input[iy][ix] < min){
                        min = input[iy][ix];
                        output_index_x[y][x] = ix;
                        output_index_y[y][x] = iy;
                    }
                }
            }

            output[y][x] = min;
        }
    }
}

void PoolingAvg::pool(const values_matrix_t& input, values_matrix_t& output)
{
    TYPE sum;
    int conv_width = input.size()/width;
    int conv_height = input[0].size()/height;

    output.resize(width);

    for(int y=0; y < height; y++){

        output[y].resize(height);

        for(int x=0; x < width; x++){
            sum = input[y][x];

            for(int iy=0; iy < conv_height; iy++){
                for(int ix=1; ix < conv_width; ix++){
                    sum += input[iy][ix];
                }
            }

            output[y][x] = sum / (conv_width*conv_height);
        }
    }
}



