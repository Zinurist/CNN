#ifndef POOLING_H
#define POOLING_H

#include "types.h"

#define POOL_ID 0
#define POOL_MAX 1
#define POOL_MIN 2
#define POOL_AVG 3



class Pooling
{
public:
    Pooling(int type, int width, int height);

    virtual void pool(const values_matrix_t& input, values_matrix_t& output) = 0;

    friend bool operator==(const Pooling& p1, const Pooling& p2);

    values_matrix_t output_index_x, output_index_y;
    int width, height, type;
};

Pooling* create_pooling(int type, int width, int height);

class PoolingMax : public Pooling
{
public:
    PoolingMax(int width, int height) : Pooling(POOL_MAX, width, height){}

    void pool(const values_matrix_t& input, values_matrix_t& output);
};


class PoolingMin : public Pooling
{
public:
    PoolingMin(int width, int height) : Pooling(POOL_MIN, width, height){}

    void pool(const values_matrix_t& input, values_matrix_t& output);
};


class PoolingAvg : public Pooling
{
public:
    PoolingAvg(int width, int height) : Pooling(POOL_AVG, width, height){}

    void pool(const values_matrix_t& input, values_matrix_t& output);
};


#endif // POOLING_H
