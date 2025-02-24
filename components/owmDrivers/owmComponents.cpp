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








