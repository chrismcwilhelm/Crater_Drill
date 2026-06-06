#pragma once
#include <stdint.h>
//Drill Pins
#define pwmPinDrill 19 
#define dirPinDrill 18

//Linear Unit Pins
#define pwmPinLinear 26
#define dirPinLinear 25

const int PPR_DRILL = 10; //Pulses Per Rotation
const int PPR_LINEAR = 700; //Pulses Per Rotation

const int PWMMIN = 0;
const int PWMMAX = 255;

const int SPEED_FACTOR = 0.075; //Tune this

typedef enum {
  MOTOR_DRILL = 0,
  MOTOR_LINEAR = 1
} motor_id_t;

void motor_init(void);

void speed_control(void *pvParameters);

void set_speed(motor_id_t id, uint8_t speed);

void set_direction(motor_id_t id, uint8_t dir);

uint8_t get_linear_pwm();