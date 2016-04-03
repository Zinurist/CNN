#include "testtraining.h"

#include <time.h>

TestTraining::TestTraining(QObject *parent) : QObject(parent)
{

}

void TestTraining::test_back_propagation_random()
{
    std::vector<int> str(3,0);
    str[0] = 4; str[1] = 8; str[2] = 3;
    NeuralNetwork n1 (1.0, str);

    n1.randomize_values(time(0), -0.01, 0.01);

    train_set set;
    set.iterations = 10000;
    set.learn_rate = 0.00001;

    srand(time(0));
    int m = rand()%10+3;
    set.input.resize(m);
    set.output.resize(m);
    for(int i=0; i<m; i++){
        set.input[i].resize(n1.num_input);
        set.output[i].resize(n1.num_output);
        for(int in=0; in<n1.num_input; in++){
            set.input[i][in] = RAND_DOUBLE(0.0,15.0);
        }
        for(int out=0; out<n1.num_output; out++){
            set.output[i][out] = RAND_DOUBLE(0.0,15.0);
        }
    }

    if(T_BPR){
        n1.print();
        printf("error: %f\n", error(n1, set));
    }

    back_propagation(n1, set);

    if(T_BPR){
        n1.print();
        printf("error: %f\n", error(n1, set));
    }

}

void TestTraining::test_back_propagation_simple()
{
    std::vector<int> str(2,3);
    NeuralNetwork n1 (1.0, str);

    n1.randomize_values(time(0), 0.01, -0.01);

    train_set set;
    set.iterations = 1000;
    set.learn_rate = 0.01;
    values_t a, b, ar, br;//remember bias!
    a.push_back(.5);ar.push_back(2.5);
    a.push_back(.5);ar.push_back(2.5);
    a.push_back(.5);ar.push_back(2.5);
    b.push_back(1);br.push_back(4);
    b.push_back(1);br.push_back(4);
    b.push_back(1);br.push_back(4);
    set.input.push_back(a);
    set.input.push_back(b);
    set.output.push_back(ar);
    set.output.push_back(br);

    if(T_BPS){
        n1.print();
        printf("error: %f\n", error(n1, set));
    }

    back_propagation(n1, set);

    if(T_BPS){
        n1.print();
        printf("error: %f\n", error(n1, set));
    }

}

void TestTraining::test_error()
{
    std::vector<int> str(2,3);
    NeuralNetwork n1 (1.0, str);

    train_set set;
    set.iterations = 1000;
    set.learn_rate = 0.0001;
    values_t a, b, ar, br;//remember bias!
    a.push_back(.5);ar.push_back(2.5);
    a.push_back(.5);ar.push_back(2.5);
    a.push_back(.5);ar.push_back(2.5);
    b.push_back(1);br.push_back(4);
    b.push_back(1);br.push_back(4);
    b.push_back(1);br.push_back(4);
    set.input.push_back(a);
    set.input.push_back(b);
    set.output.push_back(ar);
    set.output.push_back(br);

    double err = error(n1, set);

    if(T_ERR){
        printf("error: %f\n", err);
    }

    //only if func in neuron is f(x)=x
    //QVERIFY(abs(err - 4.5) < EPSILON);
}



void TestTraining::test_linear_regression()
{
    std::vector<int> str(2,4);

    NeuralNetwork n1 (1.0, str);
    NeuralNetwork n2 (1.0, str);
    n2.randomize_values(time(0), -5.0, 5.0);

    train_set set;
    set.iterations = 1000;
    set.learn_rate = 0.0001;
    values_t a, b, c, ar, br, cr;
    a.push_back(.5);ar.push_back(.5);
    a.push_back(.5);ar.push_back(1);
    a.push_back(.5);ar.push_back(.25);
    a.push_back(.5);ar.push_back(.125);
    b.push_back(1);br.push_back(1);
    b.push_back(1);br.push_back(2);
    b.push_back(1);br.push_back(.5);
    b.push_back(1);br.push_back(1);
    c.push_back(2);cr.push_back(2);
    c.push_back(2);cr.push_back(4);
    c.push_back(2);cr.push_back(1);
    c.push_back(2);cr.push_back(4);
    set.input.push_back(a);
    set.input.push_back(b);
    set.input.push_back(c);
    set.output.push_back(ar);
    set.output.push_back(br);
    set.output.push_back(cr);

    if(T_LR){
        n1.print();
        n2.print();
    }

    FILE * f = fopen("res.txt", "w");
    n1.write_to(f);
    linear_regression(n1, set);
    n1.write_to(f);

    n2.write_to(f);
    linear_regression(n2, set);
    n2.write_to(f);
    fclose(f);

    if(T_LR){
        n1.print();
        n2.print();


        n1.process(a, ar);
        n1.process(b, br);
        n1.process(c, cr);
        print(ar);
        print(br);
        print(cr);
        n2.process(a, ar);
        n2.process(b, br);
        n2.process(c, cr);
        print(ar);
        print(br);
        print(cr);
    }

}
