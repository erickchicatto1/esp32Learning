#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_rom_sys.h"

#include "owmDrivers/Perceptron.h"
#include "owmDrivers/owmComponents.h"
#include "owmDrivers/vl53l0x.h"

// Definiciones de pines
#define TRIG_GPIO           5
#define ECHO_GPIO           18
#define MOTOR1_IN1_GPIO     GPIO_NUM_12
#define MOTOR1_IN2_GPIO     GPIO_NUM_13
#define MOTOR2_IN1_GPIO     GPIO_NUM_14
#define MOTOR2_IN2_GPIO     GPIO_NUM_15
#define LED_RED_GPIO        GPIO_NUM_0
#define LED_BLUE_GPIO       GPIO_NUM_4

#define MAX_TRAINING_DATA   100  // Tamaño máximo del conjunto de entrenamiento

// Variables globales
volatile int64_t start_time = 0;
volatile int64_t end_time = 0;
volatile bool measurement_done = false;

typedef struct{
  float distance;
  int label;
}TrainingData;

TrainingData training_data[MAX_TRAINING_DATA];
int training_count = 0;  // Contador de datos de entrenamiento

Perceptron perceptron;

// Función de interrupción para el pin ECHO
static void IRAM_ATTR echo_isr_handler(void *arg) {
    if (gpio_get_level(ECHO_GPIO)) {
        start_time = esp_timer_get_time();  // Flanco de subida
    } else {
        end_time = esp_timer_get_time();    // Flanco de bajada
        measurement_done = true;
    }
}


float medir_distancia(){

    measurement_done = false;

    gpio_set_level(TRIG_GPIO,0);
    esp_rom_delay_us(2);
    gpio_set_level(TRIG_GPIO,1);
    esp_rom_delay_us(10);
    gpio_set_level(TRIG_GPIO,0);

    int timeout =0; 

    while (!measurement_done && timeout < 1000) {
        vTaskDelay(pdMS_TO_TICKS(1));  // Esperar 1 ms
        timeout++;
    }

    if (!measurement_done) {
        ESP_LOGE("SENSOR", "Error: Timeout en la medición de distancia");
        return -1;  // Devolver un valor inválido
    }

    // Calcular la distancia
    int64_t duration = end_time - start_time;
    float distance = (duration * 0.0343) / 2;  // Distancia en cm

     // Ignorar distancias inválidas o demasiado cortas
    if (distance < 2.0) {
        ESP_LOGE("SENSOR", "Distancia inválida: %.2f cm", distance);
        return -1;  // Devolver un valor inválido
    }

    return distance;

}




// Tarea: Medición de distancia con sensor ultrasónico
void TrigEchoSensorTask(void *pvParameters) {
    gpio_config_t io_config;
    io_config.intr_type = GPIO_INTR_DISABLE;
    io_config.mode = GPIO_MODE_OUTPUT;
    io_config.pin_bit_mask = (1ULL << TRIG_GPIO);
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_config);

    io_config.intr_type = GPIO_INTR_ANYEDGE;
    io_config.mode = GPIO_MODE_INPUT;
    io_config.pin_bit_mask = (1ULL << ECHO_GPIO);
    gpio_config(&io_config);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(ECHO_GPIO, echo_isr_handler, NULL);

    while (1) {
        float distance = medir_distancia();
        if (distance >= 2.0) {
            ESP_LOGI("SENSOR", "Distancia: %.2f cm", distance);

            if (training_count < MAX_TRAINING_DATA) {
                training_data[training_count].distance = distance;
                training_data[training_count].label = (distance < 50.0) ? 1 : 0;
                training_count++;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));  // Esperar 1 segundo
    }
}

// Tarea: Entrenamiento del perceptrón
void PerceptronTask(void *pvParameters) {
    while (1) {
        if (training_count > 0) {
            float distances[training_count][1];
            int labels[training_count];

            for (int i = 0; i < training_count; i++) {
                distances[i][0] = training_data[i].distance;
                labels[i] = training_data[i].label;
            }

            perceptron_train(&perceptron, (float **)distances, labels, training_count, 1);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));  // Esperar 1 segundo
    }
}

// Tarea: Control de motores y LEDs
void MotorControlTask(void *pvParameters) {
    // Configurar pines de motores y LEDs
    gpio_reset_pin(MOTOR1_IN1_GPIO);
    gpio_set_direction(MOTOR1_IN1_GPIO, GPIO_MODE_OUTPUT);
    gpio_reset_pin(MOTOR1_IN2_GPIO);
    gpio_set_direction(MOTOR1_IN2_GPIO, GPIO_MODE_OUTPUT);
    gpio_reset_pin(MOTOR2_IN1_GPIO);
    gpio_set_direction(MOTOR2_IN1_GPIO, GPIO_MODE_OUTPUT);
    gpio_reset_pin(MOTOR2_IN2_GPIO);
    gpio_set_direction(MOTOR2_IN2_GPIO, GPIO_MODE_OUTPUT);
    gpio_reset_pin(LED_RED_GPIO);
    gpio_set_direction(LED_RED_GPIO, GPIO_MODE_OUTPUT);
    gpio_reset_pin(LED_BLUE_GPIO);
    gpio_set_direction(LED_BLUE_GPIO, GPIO_MODE_OUTPUT);

    while (1) {
        if (training_count > 0) {
            float input[1] = {training_data[training_count - 1].distance};
            int prediction = perceptron_predict(&perceptron, input);
            ESP_LOGI("PERCEPTRON", "Predicción: %s", prediction == 1 ? "Cerca" : "Lejos");

            if (prediction == 1) {
                // Encender motor 1 y LED rojo
                gpio_set_level(MOTOR1_IN1_GPIO, 1);
                gpio_set_level(MOTOR1_IN2_GPIO, 0);
                gpio_set_level(LED_RED_GPIO, 1);
                gpio_set_level(LED_BLUE_GPIO, 0);
            } else if (prediction == 0) {
                // Encender motor 2 y LED azul
                gpio_set_level(MOTOR2_IN1_GPIO, 1);
                gpio_set_level(MOTOR2_IN2_GPIO, 0);
                gpio_set_level(LED_RED_GPIO, 0);
                gpio_set_level(LED_BLUE_GPIO, 1);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));  // Esperar 1 segundo
    }
}

void app_main(void) {
    // Inicializar el perceptrón
    perceptron_init(&perceptron, 1, 0.1);

    // Crear tareas
    xTaskCreate(TrigEchoSensorTask, "TrigEchoSensor", 2048, NULL, 1, NULL);
    xTaskCreate(PerceptronTask, "Perceptron", 2048, NULL, 2, NULL);
    xTaskCreate(MotorControlTask, "MotorControl", 2048, NULL, 3, NULL);

    // Mantener el programa en ejecución
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}