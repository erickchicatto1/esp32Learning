/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "esp_log.h"


void app_main(void)
{
    printf("Hello world \n!");

    esp_log_level_set("LOG",ESP_LOG_INFO);
    //Here only we are going to set until the number 3
    ESP_LOGE("LOG","This is an error");
    ESP_LOGW("LOG","This is a warning");
    ESP_LOGI("LOG","This is an info");
    ESP_LOGD("LOG","This is a Debug");
    ESP_LOGV("LOG","This is verbose");

    //
    int number = 0;
    ESP_LOGE("LOG","This is an error %d",number++ );
    ESP_LOGW("LOG","This is a warning %d" ,number++);
    ESP_LOGI("LOG","This is an info %d" ,number++);
    ESP_LOGD("LOG","This is a Debug %d" ,number++);
    ESP_LOGV("LOG","This is verbose %d" ,number++);

}
