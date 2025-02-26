#include "owmComponents.h"

L298N::L298N(gpio_num_t in1, gpio_num_t in2, gpio_num_t in3, gpio_num_t in4, 
             gpio_num_t ena, gpio_num_t enb)
    : IN1(in1), IN2(in2), IN3(in3), IN4(in4), ENA(ena), ENB(enb) {
    
    gpio_config_t io_config;
    io_config.intr_type = GPIO_INTR_DISABLE;
    io_config.mode = GPIO_MODE_OUTPUT;
    io_config.pin_bit_mask = (1ULL << IN1) | (1ULL << IN2) | (1ULL << IN3) | (1ULL << IN4);
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE; // Desactiva pull-down
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;  
    gpio_config(&io_config);
    setup_pwm();
}


void L298N::setup_pwm(){
    ledc_timer_config_t timer_config = {
     .speed_mode = LEDC_LOW_SPEED_MODE,
     .duty_resolution = LEDC_TIMER_13_BIT,
     .timer_num = LEDC_TIMER_0,
     .freq_hz = 5000,
     .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_config);

    ledc_channel_config_t channel_config_a = {
        .gpio_num = ENA,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_config_a);

    ledc_channel_config_t channel_config_b = {
        .gpio_num = ENB,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_config_b);
}

void L298N::set_motor_speed(ledc_channel_t channel, uint32_t speed){
    ledc_set_duty(LEDC_LOW_SPEED_MODE,channel,speed);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
}


void L298N::motorA_forward(uint32_t speed){
    gpio_set_level(IN1, 1);
    gpio_set_level(IN2, 0);
    set_motor_speed(LEDC_CHANNEL_0, speed);
}


void L298N::motorA_backward(uint32_t speed) {
    gpio_set_level(IN1, 0);
    gpio_set_level(IN2, 1);
    set_motor_speed(LEDC_CHANNEL_0, speed);
}

void L298N::motorA_stop() {
    gpio_set_level(IN1, 0);
    gpio_set_level(IN2, 0);
    set_motor_speed(LEDC_CHANNEL_0, 0);
}

void L298N::motorB_forward(uint32_t speed) {
    gpio_set_level(IN3, 1);
    gpio_set_level(IN4, 0);
    set_motor_speed(LEDC_CHANNEL_1, speed);
}

void L298N::motorB_backward(uint32_t speed) {
    gpio_set_level(IN3, 0);
    gpio_set_level(IN4, 1);
    set_motor_speed(LEDC_CHANNEL_1, speed);
}

void L298N::motorB_stop() {
    gpio_set_level(IN3, 0);
    gpio_set_level(IN4, 0);
    set_motor_speed(LEDC_CHANNEL_1, 0);
}

// Implementación de la interfaz en C
extern "C" {
  
  void* l298n_create(gpio_num_t in1, gpio_num_t in2, gpio_num_t in3, gpio_num_t in4, 
                       gpio_num_t ena, gpio_num_t enb) {
        return new L298N(in1, in2, in3, in4, ena, enb);
    }

     void l298n_motorA_forward(void* instance, uint32_t speed) {
        static_cast<L298N*>(instance)->motorA_forward(speed);
    }

    void l298n_motorA_backward(void* instance, uint32_t speed) {
        static_cast<L298N*>(instance)->motorA_backward(speed);
    }

    void l298n_motorA_stop(void* instance) {
        static_cast<L298N*>(instance)->motorA_stop();
    }

    void l298n_motorB_forward(void* instance, uint32_t speed) {
        static_cast<L298N*>(instance)->motorB_forward(speed);
    }

    void l298n_motorB_backward(void* instance, uint32_t speed) {
        static_cast<L298N*>(instance)->motorB_backward(speed);
    }

    void l298n_motorB_stop(void* instance) {
        static_cast<L298N*>(instance)->motorB_stop();
    }

}





