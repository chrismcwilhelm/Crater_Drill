#include "motor_control.h"
#include "sensor_control.h"
#include "Arduino.h"

unsigned long lastControlTime = millis();

float pwm_linear = PWMMAX;

void motor_init(void) {
  pinMode(dirPinDrill, OUTPUT);
  pinMode(dirPinLinear, OUTPUT);

  ledcAttach(pwmPinDrill, 5000, 8);
  ledcAttach(pwmPinLinear, 5000, 8);

  digitalWrite(dirPinDrill, LOW);
  digitalWrite(dirPinLinear, LOW);
}

void set_speed(motor_id_t id, uint8_t speed) {
  if (speed > PWMMAX) speed = PWMMAX;
  if (speed < PWMMIN) speed = PWMMIN;

  if (id == MOTOR_DRILL) {
    ledcWrite(pwmPinDrill, speed);
  }

  if (id == MOTOR_LINEAR) {
    ledcWrite(pwmPinLinear, speed);
  }
}

void set_direction(motor_id_t id, uint8_t dir) {
  if (id == MOTOR_DRILL) {
    digitalWrite(dirPinDrill, (dir == 0) ? HIGH : LOW);
  }

  if (id == MOTOR_LINEAR) {
    digitalWrite(dirPinLinear, (dir == 0) ? LOW : HIGH);
  }
}

uint8_t get_linear_pwm() {
  return PWMMAX;
}

void speed_control(void *pvParameters) {
  while (1) {
    // unsigned long now = millis();

    // if (now - lastControlTime >= 2000) {
    //   noInterrupts();
    //   long pulses_drill = get_pulses_drill();
    //   long pulses_linear = get_pulses_linear();
    //   reset_pulses();
    //   interrupts();

    //   float drill_rpm = (pulses_drill / (float)PPR_DRILL) * (60000.0 / 2000.0);

    //   float lin_target_rpm = (drill_rpm / SPEED_FACTOR);

    //   float lin_current_rpm = (pulses_linear / (float)PPR_LINEAR) * (60000.0 / 2000.0);

    //   float error = lin_target_rpm - lin_current_rpm;

    //   pwm_linear += error * 0.5;
      pwm_linear = 100;

      set_speed(MOTOR_LINEAR, (int)pwm_linear);

      //lastControlTime = now;
    //}
    vTaskDelay(5);
  }
}