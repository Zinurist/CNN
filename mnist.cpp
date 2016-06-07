
#include "mnist.h"

#include <cstdio>
#include <inttypes.h>
#include <netinet/in.h>

void load(mnist& set){
    read_labels("../../train-labels.idx1-ubyte", set.training_labels);
    read_set("../../train-images.idx3-ubyte", set.training_set);
    read_labels("../../t10k-labels.idx1-ubyte", set.test_labels);
    read_set("../../t10k-images.idx3-ubyte", set.test_set);
}

mnist* load(){
    mnist* set = new mnist;
    load(*set);
    return set;
}

void read_labels(const char* file, labels& l){
    FILE* f = fopen(file, "r");
    if(f==NULL){
        printf("Couldn't open file \"%s\"!\n", file);
        exit(2);
    }

    uint32_t num = 0;

    fread(&num, 4, 1, f);//magic num
    fread(&num, 4, 1, f);//number of items
    num = ntohl(num);

    l.resize(num);

    fread(&l[0], 1, num, f);

    fclose(f);
}


void read_set(const char* file, set& s){
    FILE* f = fopen(file, "r");
    if(f==NULL){
        printf("Couldn't open file \"%s\"!\n", file);
        exit(2);
    }

    uint32_t num = 0;

    fread(&num, 4, 1, f);//magic num
    fread(&num, 4, 1, f);//number of items
    num = ntohl(num);

    uint32_t rows, columns;
    fread(&rows, 4, 1, f);
    rows = ntohl(rows);
    fread(&columns, 4, 1, f);
    columns = ntohl(columns);

    s.reserve(num);

    for(uint32_t i=0; i<num; i++){
        image img =(image) new char[rows*columns];
        s.push_back(img);
        fread(s[i], 28*28, 1, f);
    }

    fclose(f);
}
