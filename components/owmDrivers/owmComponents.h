#ifndef OWMCOMPONENTS_H
#define OWMCOMPONENTS_H

//This library includes only for a L298N motor

#include "driver/gpio.h"
#include "driver/ledc.h"

//This is for c
#ifdef __cplusplus
extern "C" {
#endif

void* l298n_create(gpio_num_t in1, gpio_num_t in2, gpio_num_t in3, gpio_num_t in4, 
                   gpio_num_t ena, gpio_num_t enb);
void l298n_motorA_forward(void* instance, uint32_t speed);
void l298n_motorA_backward(void* instance, uint32_t speed);
void l298n_motorA_stop(void* instance);

void l298n_motorB_forward(void* instance, uint32_t speed);
void l298n_motorB_backward(void* instance, uint32_t speed);
void l298n_motorB_stop(void* instance);
#ifdef __cplusplus
}
#endif

//This is for c++
#ifdef __cplusplus
class L298N{
public:
    L298N(gpio_num_t in1, gpio_num_t in2, gpio_num_t in3, gpio_num_t in4, 
          gpio_num_t ena, gpio_num_t enb);
    void motorA_forward(uint32_t speed);
    void motorA_backward(uint32_t speed);
    void motorA_stop();
    void motorB_forward(uint32_t speed);
    void motorB_backward(uint32_t speed);
    void motorB_stop();

private:
    gpio_num_t IN1, IN2, IN3, IN4;
    gpio_num_t ENA, ENB;
    void setup_pwm();
    void set_motor_speed(ledc_channel_t channel, uint32_t speed);
};
#endif
#endif