#ifndef MNIST
#define MNIST

#include <vector>
#include <stdlib.h>

typedef std::vector<char> labels;
typedef char** image;
typedef std::vector<image> set;

typedef struct mnist{
    set training_set;
    labels training_labels;
    set test_set;
    labels test_labels;
} mnist;

void load(mnist& set);
mnist* load();
void read_labels(const char* file, labels& l);
void read_set(const char* file, set& i);

#endif // MNIST

