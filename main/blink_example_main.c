#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


//1. Define one task for FreeRtos
void task1(void *params){
    while(true){
        printf("reading temperature from %s\n", (char *) params);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}





//2. Define another task for FreeRtos 
void app_main(void){

    
}