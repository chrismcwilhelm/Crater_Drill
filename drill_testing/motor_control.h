#pragma once
#include <stdint.h>

typedef enum {
  MOTOR_DRILL = 0,
  MOTOR_LINEAR = 1
} motor_id_t;

void motor_init(void);

void sensors_init(void);

void speed_control(void *pvParameters);

void motor_enable(motor_id_t id);
void motor_disable(motor_id_t id);

void motor_set_speed(motor_id_t id, uint8_t speed);
void motor_set_direction(motor_id_t id, uint8_t dir);