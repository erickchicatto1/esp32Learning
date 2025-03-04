#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#include "owmDrivers/owmComponents.h"

//1.Ultrasonic sensor
#define TRIG_GPIO   5
#define ECHO_GPIO   18

void custom_delay_us(uint32_t us){
    uint32_t cycles = us *(CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ);
    volatile uint32_t count = 0 ;

    for(volatile uint32_t i=0;i<cycles;++i){
        count++;
    }
}

//2.Motors
//3.Engine to clean
//4.PID?

void app_main(void){
  
 //1.Set the trig as output
 gpio_config_t io_config;
 io_config.intr_type = GPIO_INTR_DISABLE;
 io_config.mode = GPIO_MODE_OUTPUT;
 io_config.pin_bit_mask = (1ULL << TRIG_GPIO); //0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0010 0000
 io_config.pull_down_en = GPIO_PULLDOWN_DISABLE; // Desactiva pull-down
 io_config.pull_up_en = GPIO_PULLUP_DISABLE;  
 gpio_config(&io_config);

 //1. Set the pin echo as input
 io_config.intr_type = GPIO_INTR_POSEDGE;
 io_config.mode = GPIO_MODE_INPUT;
 io_config.pin_bit_mask = (1ULL << ECHO_GPIO);
 io_config.pull_down_en = GPIO_PULLDOWN_DISABLE; // Desactiva pull-down
 io_config.pull_up_en = GPIO_PULLUP_DISABLE;  
 gpio_config(&io_config);

 while(1){
    //Send the pulse to echo pin
    gpio_set_level(TRIG_GPIO,0);
    custom_delay_us(2);
    gpio_set_level(TRIG_GPIO,1);
    custom_delay_us(10);
    gpio_set_level(TRIG_GPIO,0);

    //Wait until the echo pin is activate
    while(gpio_get_level(ECHO_GPIO)==0);

    //Measue the time 
    int64_t start_time = esp_timer_get_time();
    while(gpio_get_level(ECHO_GPIO)==1);
    int64_t end_time = esp_timer_get_time();

    //Calculate the distance
    int64_t duration = end_time - start_time;
    float distance = (duration * 0.0343) / 2;

    printf("Distancia: %.2f cm\n", distance);

    vTaskDelay(pdMS_TO_TICKS(1000));

    //Use of the L298N
    
 }


}