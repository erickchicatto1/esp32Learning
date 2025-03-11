#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_attr.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "esp_timer.h"
#include "esp_log.h"


//L298N
#define GPIO_PWM0A_OUT 15   // Set GPIO 15 as PWM0A (IN1 en L298N)
#define GPIO_PWM0B_OUT 16   // Set GPIO 16 as PWM0B (IN2 en L298N)
#define ENABLE_PIN 4        // Set GPIO 4 como ENA en L298N


//-----------------------------Ultrasonic module--------------------------- 
#define TRIG_GPIO   5
#define ECHO_GPIO   18

volatile int64_t start_time = 0 ;
volatile int64_t stop_time = 0;
volatile bool measurement_done = false;

//Interrupt the ECHO 
static void IRAM_ATTR echo_isr_handler(void *arg){

    if(gpio_get_level(ECHO_GPIO)){
        start_time = esp_timer_get_time();
    }
    else{
        stop_time =  esp_timer_get_time();
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

float medir_distancia(){

    measurement_done = false;
    int timeout = 0;

    gpio_set_level(TRIG_GPIO,0);
    esp_rom_delay_us(2);
    gpio_set_level(TRIG_GPIO, 1);
    esp_rom_delay_us(10);
    gpio_set_level(TRIG_GPIO, 1);

    while(!measurement_done && timeout < 1000){
        vTaskDelay(pdMS_TO_TICKS(1));  // Esperar 1 ms
        timeout++;
    }

    if(!measurement_done){
      ESP_LOGE("SENSOR", "Error: Timeout en la medición de distancia");
      return -1;
    }

    //Calcular la distancia 
    int64_t duration = stop_time - start_time ;
    float distance = (duration * 0.0343) / 2;  // Distancia en cm

    return distance;
}

// -------------------------For the motor -----------------------------------
// Inicialización de los pines MCPWM y Enable
static void mcpwm_example_gpio_initialize(void)
{
    printf("initializing mcpwm gpio...\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM0A_OUT); // IN1
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, GPIO_PWM0B_OUT); // IN2

    // Configurar el pin Enable como salida
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << ENABLE_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    // Habilitar el puente H (ENA en HIGH)
    gpio_set_level(ENABLE_PIN, 1);
    printf("Enable pin set to HIGH\n");
}

// Mover el motor hacia adelante con un ciclo de trabajo específico
static void brushed_motor_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, float duty_cycle)
{
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_B); // IN2 en LOW
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_A, duty_cycle); // IN1 con PWM
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}

// Detener el motor
static void brushed_motor_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num)
{
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_A); // IN1 en LOW
    mcpwm_set_signal_low(mcpwm_num, timer_num, MCPWM_OPR_B); // IN2 en LOW
}

void app_main(void)
{   
    printf("Testing brushed motor with Enable pin...\n");
    TrigEchoSensor();

    // 1. Inicialización de los pines MCPWM y Enable
    mcpwm_example_gpio_initialize();

    // 2. Configuración inicial del MCPWM
    printf("Configuring Initial Parameters of mcpwm...\n");
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 1000;    // Frecuencia = 1000 Hz
    pwm_config.cmpr_a = 0;          // Ciclo de trabajo de PWMxA = 0
    pwm_config.cmpr_b = 0;          // Ciclo de trabajo de PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config); // Configurar PWM0A y PWM0B

    // 3. Bucle infinito para mantener el motor al 50% de duty cycle
    while (1) {

        float distance = medir_distancia();
        printf("Distance : %.2f cm \n",distance);
        
        if(distance < 5.0){
            printf("Distance < 5 cm\n");
            brushed_motor_forward(MCPWM_UNIT_0, MCPWM_TIMER_0, 50.0);
            printf("Motor moving forward at 50%% duty cycle\n");
        }

        else if(distance > 10.0){
            printf("Distance > 10 cm\n");
            brushed_motor_forward(MCPWM_UNIT_0, MCPWM_TIMER_0, 75.0);
            printf("Motor moving forward at 75%% duty cycle\n");
        }

        
        // Esperar un breve momento antes de repetir el bucle
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}