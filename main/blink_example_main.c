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
    printf("Received notification \n");
   }


}



//2. Define another task for FreeRtos 
void app_main(void){
   xTaskCreate(&receiver, "sender", 2048,NULL, 2,&receiverHandler);
   xTaskCreate(&sender, "receiver", 2048, NULL, 2, NULL);
}