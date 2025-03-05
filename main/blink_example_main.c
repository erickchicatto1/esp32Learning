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

#define TRIG_GPIO   5
#define ECHO_GPIO   18
#define MAX_TRAINING_DATA 100  // Tamaño máximo del conjunto de entrenamiento

volatile int64_t start_time = 0;
volatile int64_t end_time = 0;
volatile bool measurement_done = false;

// Estructura para almacenar datos de entrenamiento
typedef struct {
    float distance;
    int label;
} TrainingData;

TrainingData training_data[MAX_TRAINING_DATA];
int training_count = 0;  // Contador de datos de entrenamiento

// Función de interrupción para el pin ECHO
static void IRAM_ATTR echo_isr_handler(void *arg) {
    if (gpio_get_level(ECHO_GPIO)) {
        // Flanco de subida: inicio del pulso
        start_time = esp_timer_get_time();
    } else {
        // Flanco de bajada: fin del pulso
        end_time = esp_timer_get_time();
        measurement_done = true;
    }
}

void TrigEchoSensor(void) {
    gpio_config_t io_config;
    io_config.intr_type = GPIO_INTR_DISABLE;
    io_config.mode = GPIO_MODE_OUTPUT;
    io_config.pin_bit_mask = (1ULL << TRIG_GPIO);
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_config);

    io_config.intr_type = GPIO_INTR_ANYEDGE;  // Detectar flancos de subida y bajada
    io_config.mode = GPIO_MODE_INPUT;
    io_config.pin_bit_mask = (1ULL << ECHO_GPIO);
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_config);

    // Configurar la interrupción para el pin ECHO
    gpio_install_isr_service(0);
    gpio_isr_handler_add(ECHO_GPIO, echo_isr_handler, NULL);
}

float medir_distancia() {
    measurement_done = false;

    // Enviar pulso al pin TRIG
    gpio_set_level(TRIG_GPIO, 0);
    esp_rom_delay_us(2);
    gpio_set_level(TRIG_GPIO, 1);
    esp_rom_delay_us(10);
    gpio_set_level(TRIG_GPIO, 0);

    // Esperar a que la medición esté lista
    int timeout = 0;
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

void app_main(void) {
    // Inicializar el sensor ultrasónico
    TrigEchoSensor();

    // Inicializar el perceptrón con 1 entrada (distancia) y una tasa de aprendizaje de 0.1
    Perceptron perceptron;
    perceptron_init(&perceptron, 1, 0.1);

    // Bucle principal
    while (1) {
        // Medir la distancia
        float distance = medir_distancia();
        ESP_LOGI("SENSOR", "Distancia: %.2f cm", distance);

        // Guardar la distancia en el conjunto de entrenamiento
        if (training_count < MAX_TRAINING_DATA) {
            training_data[training_count].distance = distance;

            // Asignar una etiqueta basada en la distancia (1 = "cerca", 0 = "lejos")
            if (distance < 50.0) {
                training_data[training_count].label = 1;  // Cerca
            } else {
                training_data[training_count].label = 0;  // Lejos
            }

            training_count++;
            ESP_LOGI("TRAINING", "Dato de entrenamiento guardado: Distancia = %.2f cm, Etiqueta = %d",
                     training_data[training_count - 1].distance, training_data[training_count - 1].label);
        } else {
            ESP_LOGE("TRAINING", "Conjunto de entrenamiento lleno. No se pueden guardar más datos.");
        }

        // Entrenar el perceptrón con los datos guardados
        if (training_count > 0) {
            float distances[training_count][1] __attribute__((aligned(4)));  // Asegurar alineación
            int labels[training_count];

            for (int i = 0; i < training_count; i++) {
                distances[i][0] = training_data[i].distance;
                labels[i] = training_data[i].label;
            }

            perceptron_train(&perceptron, (float **)distances, labels, training_count, 1);
        }

        // Predecir si la distancia es "cerca" o "lejos"
        float input[1] __attribute__((aligned(4)));  // Asegurar alineación
        input[0] = distance;
        int prediction = perceptron_predict(&perceptron, input);
        ESP_LOGI("PERCEPTRON", "Predicción: %s", prediction == 1 ? "Cerca" : "Lejos");

        // Esperar 1 segundo antes de la siguiente medición
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Liberar memoria (opcional, ya que app_main nunca termina)
    perceptron_free(&perceptron);
}