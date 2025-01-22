/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG   "DELAY"

void app_main(void)
{
    int i=0;
    
    while(1){
      vTaskDelay(1000/portTICK_PERIOD_MS);
      ESP_LOGI(TAG,"\n Counting %d ",i++);
    }
 
}
