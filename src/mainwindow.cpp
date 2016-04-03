#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <time.h>

#define WIDTH 3
#define HEIGHT 3

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //first tab
    int testo[] = {WIDTH*HEIGHT,1};
    std::vector<int> structure (testo, testo+2);
    nn_field = new NeuralNetwork(1.0, structure);
    nn_field->randomize_values(time(0), -1.0, 1.0);

    set_field = new train_set;
    set_field->iterations = 10000;
    set_field->learn_rate = 0.001;

    //second tab
    int testo_ae[] = {10,5,10};
    std::vector<int> structure_ae (testo_ae, testo_ae+3);
    nn_ae = new NeuralNetwork(1.0, structure_ae);
    nn_ae->randomize_values(time(0), -1.0, 1.0);

    set_ae = new train_set;
    set_ae->iterations = 10000;
    set_ae->learn_rate = 0.00001;

}

MainWindow::~MainWindow()
{
    delete ui;
    delete nn_field;
    delete set_field;
    delete nn_ae;
    delete set_ae;
}

//first tab

void MainWindow::on_btn_print_clicked()
{
    nn_field->print();
}

void MainWindow::on_btn_add_sets_clicked()
{
    values_t* input = new values_t(nn_field->num_input, 0.0);
    values_t* output = new values_t(nn_field->num_output, 0.0);

    read_grid(*input);
    (*output)[0] = ui->btn_output->isChecked()? 1.0 : 0.0;

    set_field->input.push_back(*input);
    set_field->output.push_back(*output);
    ui->lbl_sets->setText(QString("%1 sets").arg(set_field->input.size()));
}

void MainWindow::on_btn_delete_sets_clicked()
{
    set_field->input.clear();
    set_field->output.clear();
    ui->lbl_sets->setText("0 sets");
}

void MainWindow::on_btn_train_clicked()
{
    set_field->iterations = ui->spin_iter->value();
    double err = error(*nn_field,*set_field);
    back_propagation(*nn_field, *set_field);
    ui->lbl_err->setText(QString("%1").arg(err-error(*nn_field,*set_field)));
}

void MainWindow::on_btn_reset_clicked()
{
    nn_field->randomize_values(time(0), -1.0, 1.0);
}

void MainWindow::on_btn_activate_clicked()
{
    values_t input(nn_field->num_input, 0.0);
    values_t output(nn_field->num_output, 0.0);

    read_grid(input);

    nn_field->process(input, output);

    ui->lbl_output->setText(QString("%1").arg(output[0]));

}


void MainWindow::read_grid(values_t& input)
{
    QPushButton* btn;
    for(int y=0; y<HEIGHT; y++){
        for(int x=0; x<WIDTH; x++){
            btn = (QPushButton*) ui->gridLayout->itemAtPosition(y,x)->widget();
            input[x+y*HEIGHT] = btn->isChecked()? 1.0:0.0;
        }
    }
}


void MainWindow::on_btn_save_clicked()
{
    FILE * f= fopen("nn_field.txt", "w");
    nn_field->write_to(f);
    fclose(f);
}

void MainWindow::on_btn_load_clicked()
{
    FILE * f = fopen("nn_field.txt", "r");
    nn_field->read_from(f);
    fclose(f);
}



//second tab

void MainWindow::on_btn_print_ae_clicked()
{
    nn_ae->print();
}

void MainWindow::on_btn_save_ae_clicked()
{
    FILE * f= fopen("nn_ae.txt", "w");
    nn_ae->write_to(f);
    fclose(f);
}

void MainWindow::on_btn_load_ae_clicked()
{
    FILE * f = fopen("nn_ae.txt", "r");
    nn_ae->read_from(f);
    fclose(f);
}

void MainWindow::on_btn_reset_ae_clicked()
{
    nn_ae->randomize_values(time(0), -1.0, 1.0);
}


void MainWindow::on_btn_train_ae_clicked()
{
    set_ae->iterations = ui->spin_iter_ae->value();
    int num_sets = ui->spin_sets_ae->value();
    set_ae->input.resize(num_sets);
    set_ae->output.resize(num_sets);
    if(ui->cb_random_ae->isChecked()){
        for(int i=0; i<num_sets; i++){
            set_ae->input[i].resize(10);
            set_ae->output[i].resize(10);
            for(int k=0; k<nn_ae->num_input; k++){//num_input=num_output!
                set_ae->input[i][k] = RAND_DOUBLE(0.0,1.0);
                set_ae->output[i][k] = set_ae->input[i][k];
            }
        }
    }else{//first one random, after that use result of NN
        set_ae->input[0].resize(10);
        set_ae->output[0].resize(10);
        for(int k=0; k<nn_ae->num_input; k++){
            set_ae->input[0][k] = RAND_DOUBLE(0.0,1.0);
            set_ae->output[0][k] = set_ae->input[0][k];
        }

        for(int i=1; i<num_sets; i++){
            set_ae->input[i].resize(10);
            set_ae->output[i].resize(10);
            nn_ae->process(set_ae->input[i-1],set_ae->output[i]);
            for(int k=0; k<nn_ae->num_input; k++){
                set_ae->input[i][k] = set_ae->output[i][k];
            }
        }
    }

    back_propagation(*nn_ae, *set_ae);
}

void MainWindow::on_btn_encode_ae_clicked()
{
    values_t input(10,0.0);
    values_t output(10,0.0);
    QDoubleSpinBox * box;
    for(int i=0; i<10; i++){
        box = (QDoubleSpinBox *) ui->grid_input_ae->itemAtPosition(i,0)->widget();
        input[i] = box->value();
    }

    nn_ae->process(input,output);
    for(int i=0; i<5; i++){
        box = (QDoubleSpinBox *) ui->grid_middle_ae->itemAtPosition(i,0)->widget();
        box->setValue(nn_ae->net[1][i].value);
    }
    for(int i=0; i<10; i++){
        box = (QDoubleSpinBox *) ui->grid_output_ae->itemAtPosition(i,0)->widget();
        box->setValue(output[i]);
    }

}

void MainWindow::on_btn_decode_ae_clicked()
{
    values_t output(10,0.0);
    QDoubleSpinBox * box;
    for(int i=0; i<5; i++){
        box = (QDoubleSpinBox *) ui->grid_middle_ae->itemAtPosition(i,0)->widget();
        nn_ae->net[1][i].value = box->value();
    }

    nn_ae->process_from(2,output);

    for(int i=0; i<10; i++){
        box = (QDoubleSpinBox *) ui->grid_output_ae->itemAtPosition(i,0)->widget();
        box->setValue(output[i]);
    }
}


void MainWindow::on_btn_randomize1_ae_clicked()
{
    QDoubleSpinBox * box;
    for(int i=0; i<10; i++){
        box = (QDoubleSpinBox *) ui->grid_input_ae->itemAtPosition(i,0)->widget();
        box->setValue(RAND_DOUBLE(0.0,1.0));
    }
}

void MainWindow::on_btn_randomize2_ae_clicked()
{
    QDoubleSpinBox * box;
    for(int i=0; i<5; i++){
        box = (QDoubleSpinBox *) ui->grid_middle_ae->itemAtPosition(i,0)->widget();
        box->setValue(RAND_DOUBLE(0.0,1.0));
    }
}




















