#ifndef MNIST
#define MNIST

#include "training.h"

#include <vector>
#include <stdlib.h>

#define IMAGE_SIZE 28*28
#define LABEL_SIZE 10

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
void to_train_set(train_set& t, const set& s, const labels& l);

#endif // MNIST

