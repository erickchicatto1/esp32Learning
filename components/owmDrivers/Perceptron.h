#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <stdlib.h>
#include <time.h>

typedef struct {
  float *weights;
  float bias;
  float learning_rate;
  int input_size;
}Perceptron;

//Inicializar el perceptron
void perceptron_init(Perceptron *p,int input_size,float learning_rate);

//Activation function
int perceptron_activation(float value);

//Predecir la salida para una entrada dada
int perceptron_predict(Perceptron *p,float *inputs);

//Entrenar al perceptron
void perceptron_train(Perceptron *p, float **training_data, int *labels, int data_size, int epochs);

//liberar la memoria 
void perceptron_free(Perceptron *p);

#endif