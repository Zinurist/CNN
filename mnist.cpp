
#include "mnist.h"
#include "types.h"

#include <cstdio>
#include <inttypes.h>
#ifdef _WIN32
    int ntohl(int num){
        char * p = (char*) &num;
        char c = p[0];
        p[0] = p[3];
        p[3] = c;
        c = p[1];
        p[1] = p[2];
        p[2] = c;
        return num;
    }
#else
    #include <netinet/in.h>
#endif

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
        fread(s[i], IMAGE_SIZE, 1, f);
    }

    fclose(f);
}


void to_train_set(train_set& t, const set& s, const labels& l){
    to_train_set(t, s, l, s.size());
}


void to_train_set(train_set& t, const set& s, const labels& l, size_t batch_size){
    if(s.size() != l.size() || s.size() < batch_size) throw "Invalid set sizes!";

    t.input.resize(batch_size);
    t.output.resize(batch_size);
    for(int i=0; i<batch_size; i++){
        t.input[i].resize(IMAGE_SIZE);
        t.output[i].resize(LABEL_SIZE);

        for(int k=0; k<IMAGE_SIZE; k++){
            t.input[i][k] = (TYPE) ((unsigned char*) s[i]) [k];
            t.input[i][k] /= 255.0;
        }
        for(int k=0; k<LABEL_SIZE; k++){
            t.output[i][k] = 0.0;
        }
        t.output[i][ l[i] ] = 1.0;
    }
    /*only 1 and 0:
    int count = 0;
    t.input.resize(batch_size);
    t.output.resize(batch_size);
    for(int i=0; i<batch_size; i++){
        t.input[i].resize(IMAGE_SIZE);
        t.output[i].resize(LABEL_SIZE);
        if(l[count] == 0 || l[count] == 1){
            for(int k=0; k<IMAGE_SIZE; k++){
                t.input[i][k] = (TYPE) ((unsigned char*) s[i]) [k];
                t.input[i][k] /= 255.0;
            }
            for(int k=0; k<LABEL_SIZE; k++){
                t.output[i][k] = 0.0;
            }
            t.output[i][ l[count] ] = 1.0;
        }
        count++;
    }
    */

}


