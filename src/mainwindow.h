#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "neuralnetwork.h"
#include "types.h"
#include "training.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_print_clicked();

    void on_btn_add_sets_clicked();

    void on_btn_delete_sets_clicked();

    void on_btn_train_clicked();

    void on_btn_reset_clicked();

    void on_btn_activate_clicked();

    void on_btn_save_clicked();

    void on_btn_load_clicked();

    void on_btn_print_ae_clicked();

    void on_btn_save_ae_clicked();

    void on_btn_load_ae_clicked();

    void on_btn_reset_ae_clicked();

    void on_btn_train_ae_clicked();

    void on_btn_encode_ae_clicked();

    void on_btn_decode_ae_clicked();

    void on_btn_randomize1_ae_clicked();

    void on_btn_randomize2_ae_clicked();

private:
    Ui::MainWindow *ui;


    NeuralNetwork * nn_field;
    train_set * set_field;
    NeuralNetwork * nn_ae;
    train_set * set_ae;

    void read_grid(values_t& input);
};

#endif // MAINWINDOW_H
