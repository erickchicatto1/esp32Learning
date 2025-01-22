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

void app_main(void)
{

//1. Declare array to store what we write in the cmd
char data;//Data in where we are going to save the data user
char str[100];//array to store the characters
memset(str,0,sizeof(str));//to start all the elements with cero

//2. Check if we don't use \n
if(str != '\n'){ //Check for the line break
   data = getchar();//Get the data from the cmd
   str[100]=data // don't understand
   
}
//3. Check if we don't write 0xff

//4. Convert the values in characters

//5.   
  
}
