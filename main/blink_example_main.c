#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static TaskHandle_t receiverHandler = NULL;


void sender(void *params){
    while(true){
        xTaskNotifyGive(receiverHandler);
        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}

void receiver(void *params){
    while(true){
        ulTaskNotifyTake(pdFALSE,portMAX_DELAY);
        printf("Receive notification \n");
    }
}




void app_main(void){
    
    //Define a char
    char c = 0;

    //Define an array with chars
    char str[100];

    //Fill the array with zeros
    memset(str,0,sizeof(str));

    //check that we dont have a line brake
    while(c != '\n'){
      
      c = getchar(); //get the char

        if(c!=0xff){
            str[strlen(str)] = c; // eg. hola -> str[4]= c -> holac, to add characters
            printf("%c",c);
        }
        vTaskDelay(100/portTICK_PERIOD_MS);
    }

    printf("you typed : %s \n",str);


    //Create the task.
    xTaskCreate(&receiver,"sender",2048,NULL,2,&receiverHandler);
    xTaskCreate(&sender, "receiver", 2048, NULL, 2, NULL);

}