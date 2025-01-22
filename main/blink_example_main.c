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

//Add libraries 
#include "esp_random.h"
#include "math.h"

#define TAG   "DICE"

//Function to ensure that only we are going to have six faces to random numbers
int numberDice(){
    //1.Get the number random
    int randomNumber = esp_random();
    //Need to be positive
    int positiveNumber = abs(randomNumber);
    //2. Ensure that only its going to be into 6 range
    int ensureNumber = (positiveNumber % 6) + 1;
    //3.return the number
    return ensureNumber;
}



void app_main(void)
{
  //1.Make a delay
  while(1){
     vTaskDelay(1000/portTICK_PERIOD_MS);
     //2.Ensure that delay be an infinite loop 
     //3.print into the log the value of the random number beetween 1 - 6
     int espNumberRandom = numberDice();
     ESP_LOGI(TAG,"This is the random number : %d",espNumberRandom);
  }
 
}
