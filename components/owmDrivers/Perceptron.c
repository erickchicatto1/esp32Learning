#include "Perceptron.h"
#include <stdlib.h>
#include <time.h>

void perceptron_init(Perceptron *p,int input_size,float learning_rate){
 
 p->input_size = input_size;
 p->learning_rate = learning_rate;

 p->weights = (float *)malloc(input_size * sizeof(float));
 if(p->weights ==NULL){
   return;
 }

 srand(time(NULL));
 for(int i=0;i<input_size;++i){
  p->weights[i] = ((float)rand() / RAND_MAX) * 2 - 1; //Escala el valor al rango de [-1,1]
 }
 
 //Inicializar bias aleatoriamente
 p->bias = ((float)rand() / RAND_MAX) * 2 - 1; // entre [-1,1]

}


int perceptron_activation(float value){
    return (value >= 0) ? 1 : 0;
}

// Predecir la salida para una entrada dada
int perceptron_predict(Perceptron *p,float *inputs){
    float sum = p->bias;

    for(int i=0;i<p->input_size;++i){
         sum += inputs[i] * p->weights[i];
    }
   return perceptron_activation(sum);
}

void perceptron_train(Perceptron *p, float **training_data, int *labels, int data_size, int epochs){

   for(int epoch=0;epoch<epochs;++epoch){
    for(int i=0;i<data_size;++i){
        int prediction = perceptron_predict(p,training_data[i]);
        int error = labels[i]-prediction;

        //Actualizar pesos y bias
        for(int j=0;j<p->input_size;++j){
            p->weights[j] += p->learning_rate*error*training_data[i][j];
        }
        p->bias += p->learning_rate*error;
    }

   }
}

void perceptron_free(Perceptron *p){
    free(p->weights); 
}







