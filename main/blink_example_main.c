/* Examples for practicing the esp-idf framework

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Implement a function to store values in array using pointers
void SaveIntoArray(char *elementArray){ // To point to the first element in the array

char tempArray[10]={0};
//1. Make a for to save every element that are passed with the pointer , to the array 
for(int i = 0 ; i<=10;i++){
   tempArray[10] = *elementArray;
}
//2. 

}

void app_main(void)
{
    //Define a char
    char c = 0 ; // this char is going to be used with the getchar function
    //Define an array with chars
    char str[100];
    //Fill the array with ceros
    memset(str,0,sizeof(str));

    while(c != '\n'){ //This is for check that we don't have line break
       c = getchar();//get the char

       if(c!=0xff){ //check if the char is different to 0xff
        str[strlen(str)] = c; // to avoid the void char

        //Save the char into the array 
        SaveIntoArray(str);
        printf("%c",c);
       }
      vTaskDelay(100/portTICK_PERIOD_MS);
    }
    printf("you typed : %s\n",str);
}
