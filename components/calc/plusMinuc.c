#include<stdio.h>

//Exercise one Ex 1
void plusMinus(int arr_count, int *arr){
  
  int posCount=0;//3.PositiveCounter
  int negCount=0;
  int zeroCount=0;
  float resultPos=0;
  float resultNeg=0;
  float resultZero=0;
  
  //1. print all the elements 
  for(int i = 0 ; i<arr_count;i++){
    printf("This is the values %d", arr[i]);
    //2. Check if mayor to cero
    if(arr[i] > 0){
      posCount++;
    }
    else if(arr[i] < 0){
      negCount++;
    }
    else if(arr[i] == 0){
      zeroCount++;
    }
    
    
  }
  resultPos = (float)posCount/arr_count;
  resultNeg = (float)negCount/arr_count;
  resultZero = (float)zeroCount/arr_count;
  
}


//Exercise two Ex 2 
// Function to get the min and max sum of 4 elements in an array
void minMaxSum(int *array, int length) {
    if (length < 2) {
        printf("At least 2 elements are required.\n");
        return;
    }

    // Bubble Sort to sort the array
    for (int i = 0; i < length - 1; ++i) {
        for (int j = 0; j < length - i - 1; ++j) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }

    // Calculate the min sum (sum of the first 4 elements)
    long int minSum = 0, maxSum = 0;
    for (int i = 0; i < 4; ++i) {
        minSum += array[i];
    }

    // Calculate the max sum (sum of the last 4 elements)
    for (int i = length - 4; i < length; ++i) {
        maxSum += array[i];
    }

    // Print results
    printf("Min sum: %ld, Max sum: %ld\n", minSum, maxSum);
}



//Exercise tree just study all 
/* 
-----------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char* timeConversion(char* s) {
    char time;
    //. Define the cases9
    switch(time){
      case '0':
          s = s + 'AM';
      case 'PM':
          s = s + 12 + 'PM';
          
    }
  
  return s;    
}


int main() {
    
  //1. Create the input 
  char exampTime = {'0','7','0','5','4','5','P','M'};
  //2. how to use the function?
  timeConversion(exampTime);
  
  return 0;
}

---------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>


void convertTo24HourFormat(int *hh , int *mm  ,int *ss , const char *a){
  
  if(*hh>=1 && *hh <=12 && *mm>=0 && *mm<=59 && *ss >=0 && *ss<=59){
    //AM or PM
    if( (strcmp(a,"PM")== 0 || strcmp(a,"pm")==0) && *hh !=12){
     *hh = *hh + 12;
    }
    
     if( (strcmp(a,"AM")== 0 || strcmp(a,"am")==0) && *hh ==12){
     *hh = 0;
    }
    
    printf("The obtained 24-hour format of input is: \t");
    printf("%02d:%02d:%02d\n", *hh, *mm, *ss);
    
  }
  
  else{
    printf("Invalid input for time values.\n");  
  }

}

int main() {
  //to can work from main , we use pointers
  int hh = 7,mm=5,ss=45;
  char a[]="PM";
  convertTo24HourFormat(&hh,&mm,&ss,a);
  
     
  return 0;
}

---------------------------------------------------------------------------
Ex 3
char* timeConversion(char* s) {
    static char result[9];  
    int hh, mm, ss;
    char period[3];  

    hh = (s[0] - '0') * 10 + (s[1] - '0');  
    mm = (s[3] - '0') * 10 + (s[4] - '0');  
    ss = (s[6] - '0') * 10 + (s[7] - '0');  
    period[0] = s[8];  
    period[1] = s[9];  
    period[2] = '\0';  

    if (strcmp(period, "PM") == 0 && hh != 12) {
        hh += 12;  
    } else if (strcmp(period, "AM") == 0 && hh == 12) {
        hh = 0; 
    }

    result[0] = (hh / 10) + '0';  
    result[1] = (hh % 10) + '0';  
    result[2] = ':';  
    result[3] = (mm / 10) + '0';  
    result[4] = (mm % 10) + '0';  
    result[5] = ':';  
    result[6] = (ss / 10) + '0';  
    result[7] = (ss % 10) + '0';  
    result[8] = '\0';  

    return result;  
}


---------------------------------------------------------------------
my mistakes
#include <stdio.h>

int* breakingrecords(int scores_count, int* scores, int* result_count){
  
  int countMin;
  int countMax;
  int records[2];
  
  for(int i=0;i<scores_count;i++){
    //count the index 
    printf("This is the index : %d",scores[i]);
    
    //check for the min
    if(score[i]<score[i+1]){
      score[i] = score[i+1];
      countMin++;
    }
    
    //check for the max 
    if(score[i]>score[i+1]){
      score[i] = score[i+1];
      countMax++;
    }
    
    
    //assing the records
    records[0] = countMin;
    records[1] = countMax;
    
  }                                             
  
  return records;
}


int main() {
  
  //1. Make the test for the logic in the function
  int scores[4]={12,24,10,24};
  //3. 
  int scores_count = 4;
  int numberforResult=4;
  int *result_count= &numberforResult;
  
  int *functionReturn = breakingrecords;
  
  
  return 0;
}

-----------------First solution correction---------------------------------------
#include <stdio.h>
#include <stdlib.h> // Para usar malloc y free

int* breakingrecords(int scores_count, int* scores, int* result_count) {
    // Inicializar contadores
    int countMin = 0;
    int countMax = 0;

    // Inicializar min y max con el primer valor del arreglo
    int min = scores[0];
    int max = scores[0];

    // Recorrer el arreglo de puntajes
    for (int i = 1; i < scores_count; i++) {
        // Verificar si se rompe el récord mínimo
        if (scores[i] < min) {
            min = scores[i];
            countMin++;
        }

        // Verificar si se rompe el récord máximo
        if (scores[i] > max) {
            max = scores[i];
            countMax++;
        }
    }

    // Asignar resultados a un arreglo dinámico
    int* records = (int*)malloc(2 * sizeof(int));
    records[0] = countMax; // Número de veces que se rompe el récord máximo
    records[1] = countMin; // Número de veces que se rompe el récord mínimo

    // Asignar el tamaño del resultado
    *result_count = 2;

    return records;
}

------------------------------------------------------------------------------------
i#include <stdio.h>
#include <stdlib.h>


int *breaking_records(int scores_count , int *scores, int* result_counts){
  
  //1. Verificar que no hay partidos 
  if(scores_count==0){
    *result_counts=0;
    return NULL;
  }
  
  //2. Asignar memoria al resultado
  int* result = calloc(2,sizeof(int));
  if(result==NULL){
    *result_counts=0;
    return NULL;
  }
  
  //3. Inicializar min , max con la primera puntuacion
  int min = scores[0] , max = scores[0];
  
  //4. Inicializar los contadores
  result[0] = 0;
  result[1] = 0;
  
  //5. Iterar desde el 2do partido 
  for(int i=1;i<scores_count;++i){
    
    //6. Check the min,max
    if(scores[i]<min){
      min = scores[i];
      result[0]++;
    }
    
    if(scores[i]>max){
      max=scores[i];
      result[1]++;
    }
  }
   //7. Establecer el tam del resultado
    *result_counts = 2;
    return result;
}

int main() {
    int scores[] = {12, 24, 10, 24};
    int scores_count = 4;
    int result_count;
    
    int* result = breaking_records(scores_count, scores, &result_count);
    
    if (result != NULL) {
        printf("Max records broken: %d\n", result[0]);
        printf("Min records broken: %d\n", result[1]);
        
        // Liberar la memoria asignada
        free(result);
    } else {
        printf("Error: No se pudieron calcular los récords.\n");
    }
    
    return 0;
}
------------------------------------------------------------------------------------------------------------------------------------
Ex.5
int divisibleSumPairs(int n, int k, int* arr) {
    // Contador para devolver
    int counter = 0;

    // Iterar sobre el array
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            // Verificar si la suma de arr[i] y arr[j] es divisible por k
            if ((arr[i] + arr[j]) % k == 0) {
                counter++;
            }
        }
    }

    return counter;
}

int main() {
    // Definir el array y los parámetros
    int arr[6] = {1, 3, 2, 6, 1, 2};
    int n = 6;
    int k = 3;

    // Llamar a la función y obtener el resultado
    int result = divisibleSumPairs(n, k, arr);

    // Imprimir el resultado
    printf("Number of divisible sum pairs: %d\n", result);

    return 0;
}

/--------------------------------------------------------------------------------------------------------------------------------------

*/

/*
int main() {
  
  //Ex.1
  int testVal[5]={1,1,0,-1,-1};
  int n = 5;
  plusMinus(n,testVal);

  //Ex.2
  int array[5] = {1, 3, 5, 7, 9}; // Test array
  int length = sizeof(array) / sizeof(array[0]);

  minMaxSum(array, length);

  //Ex3
  // Casos de prueba
  char* test1 = "07:05:45PM";
  char* test2 = "12:00:00AM";
  char* test3 = "12:00:00PM";
  char* test4 = "01:30:15AM";
  char* test5 = "11:59:59PM";

    // Llamadas a la función y verificación de resultados
  printf("Entrada: %s -> Salida: %s\n", test1, timeConversion(test1)); // Debe ser "19:05:45"
  printf("Entrada: %s -> Salida: %s\n", test2, timeConversion(test2)); // Debe ser "00:00:00"
  printf("Entrada: %s -> Salida: %s\n", test3, timeConversion(test3)); // Debe ser "12:00:00"
  printf("Entrada: %s -> Salida: %s\n", test4, timeConversion(test4)); // Debe ser "01:30:15"
  printf("Entrada: %s -> Salida: %s\n", test5, timeConversion(test5)); // Debe ser "23:59:59"

  

  return 0;
}

 */
