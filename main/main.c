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
//Sensor Ultrasonico
#define TRIG_GPIO         5
#define ECHO_GPIO         18
//Power Train
#define MAX_TRAINING_DATA 100  // Tamaño máximo del conjunto de entrenamiento
#define PIN_RED           0
#define PIN_BLUE          4

//Motor dc sistema de succion
#define IN1_GPIO GPIO_NUM_1  // Control de dirección
#define IN2_GPIO GPIO_NUM_2  // Control de dirección
#define ENA_GPIO GPIO_NUM_3  // Control de velocidad (PWM)
// Configuración del PWM
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT  // Resolución de 13 bits (0-8191)
#define LEDC_FREQUENCY 5000              // Frecuencia de 5 kHz



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


void SuctionSystem(){

  gpio_reset_pin(IN1_GPIO);
  gpio_set_direction(IN1_GPIO, GPIO_MODE_OUTPUT);
  gpio_reset_pin(IN2_GPIO);
  gpio_set_direction(IN2_GPIO, GPIO_MODE_OUTPUT);

    // Configurar el PWM
  ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK
    };
  ledc_timer_config(&timer_config);

  ledc_channel_config_t channel_config = {
        .gpio_num = ENA_GPIO,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .duty = 0,  // Iniciar con duty cycle 0 (motor apagado)
        .hpoint = 0
    };
  ledc_channel_config(&channel_config);

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
    SuctionSystem();
    TrigEchoSensor();

    // Inicializar el perceptrón con 1 entrada (distancia) y una tasa de aprendizaje de 0.1
    Perceptron perceptron;
    perceptron_init(&perceptron, 1, 0.1);

    // // Crear una instancia de L298N
    void* motorController = l298n_create(GPIO_NUM_12, GPIO_NUM_13, GPIO_NUM_14, GPIO_NUM_15, 
                                        GPIO_NUM_16, GPIO_NUM_17);

    //Leds
    gpio_reset_pin(PIN_RED);
    gpio_set_direction(PIN_RED,GPIO_MODE_OUTPUT);
    gpio_reset_pin(PIN_BLUE);
    gpio_set_direction(PIN_BLUE,GPIO_MODE_OUTPUT);

    
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
            //Arrays to store the data
            float distances[training_count][1] __attribute__((aligned(4)));  // Asegurar alineación
            int labels[training_count];

        //Copiar los datos de entrenamiento
            for (int i = 0; i < training_count; i++) {
                distances[i][0] = training_data[i].distance;
                labels[i] = training_data[i].label;
            }

            perceptron_train(&perceptron, (float **)distances, labels, training_count, 1);

            //Sucction system
        gpio_set_level(IN1_GPIO, 1);
        gpio_set_level(IN2_GPIO, 0);
        printf("Girando en sentido horario \n");

        //Aumentar la velocidad gradualmente 
        for(int duty=0;duty<8191;duty+=100){
           ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
           ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }

        //Detener el motor 
        gpio_set_level(IN1_GPIO,0);
        gpio_set_level(IN2_GPIO,0);
        printf("Motor detenido \n");
        vTaskDelay(100/ portTICK_PERIOD_MS);

        gpio_set_level(IN1_GPIO,0);
        gpio_set_level(IN1_GPIO,1);
        printf("Sentido anti horario \n");

        for(int duty=8191;duty>0;duty-=100){
           ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
           ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
           vTaskDelay(10 / portTICK_PERIOD_MS); 
        }

        // Detener el motor
        gpio_set_level(IN1_GPIO, 0);
        gpio_set_level(IN2_GPIO, 0);
        printf("Motor detenido\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        }

        // Predecir si la distancia es "cerca" o "lejos"
        float input[1] __attribute__((aligned(4)));  // Asegurar alineación
        input[0] = distance;
        int prediction = perceptron_predict(&perceptron, input);
        ESP_LOGI("PERCEPTRON", "Predicción: %s", prediction == 1 ? "Cerca" : "Lejos");
        
        //Escribir la logica para que se muevan los motores, agregar lo de opencv
        if(prediction == 1){
          //MotorA Turn ON
          //MotorB Turn On
          gpio_set_level(PIN_RED,1);
          l298n_motorA_forward(motorController, 4095); // 50% de velocidad
          l298n_motorB_forward(motorController, 4095);
          vTaskDelay(pdMS_TO_TICKS(2000)); // Espera 2 segundos
          
          l298n_motorA_stop(motorController);
          l298n_motorB_stop(motorController);
          gpio_set_level(PIN_RED,0);

          // Mover ambos motores hacia atrás a velocidad máxima
          l298n_motorA_backward(motorController, 8191); // 100% de velocidad
          l298n_motorB_backward(motorController, 8191);

          vTaskDelay(pdMS_TO_TICKS(2000)); // Espera 2 segundos
          //Turn led red
        }

        else if(prediction == 0){
          //MotorA Turn Off
          //MotorB Turn Off
        gpio_set_level(PIN_BLUE,1);
        l298n_motorA_stop(motorController);
        l298n_motorB_stop(motorController);
        gpio_set_level(PIN_BLUE,0 );
        vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo
        //Turn led blue
        }


        //Agregar el modulo de can mcp2515? para hacerle diagnosticos?


    
        // Esperar 1 segundo antes de la siguiente medición
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Liberar memoria (opcional, ya que app_main nunca termina)
    perceptron_free(&perceptron);
}