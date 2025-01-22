/* Examples for practicing the esp-idf framework

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//1. Add the library for the gpios
#include "driver/gpio.h"

#define TAG   "BLINK"
// LL define the pin 
#define PINR  0   // Red
#define PING  2   // Green
#define PINB 4  // Blue

void app_main(void)
{

 //Config if the gpio is input or output
 //gpio_set_direction(GPIO_NUM_0,GPIO_MODE_DEF_OUTPUT); my error
  gpio_set_direction(PINR,GPIO_MODE_OUTPUT); 
  gpio_set_direction(PING,GPIO_MODE_OUTPUT);
  gpio_set_direction(PINB,GPIO_MODE_OUTPUT);

  uint32_t isOn = 0 ;  

  while(1){
    //Create variable flag to on and off
    isOn = !isOn;
    //uint8_t LedOn=0; my error
    //2.Set the level of the gpio
    gpio_set_level(PINR,isOn);
    //LedOn = !LedOn;my error
    //3.make a infinite loop to on and off the led every 1 second
    vTaskDelay(1000/portTICK_PERIOD_MS);
    gpio_set_level(PING,isOn);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    gpio_set_level(PINB,isOn);
    
  }

  
}
