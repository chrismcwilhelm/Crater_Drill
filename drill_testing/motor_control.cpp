#include "motor_control.h"
#include "drill_constants.h"
#include "Arduino.h"

static uint8_t enabled_drill = 0;
static uint8_t enabled_linear = 0;

void motor_init(void) {
  pinMode(dirPinDrill, OUTPUT);
  pinMode(dirPinLinear, OUTPUT);

  ledcAttach(pwmPinDrill, 5000, 8);
  ledcAttach(pwmPinLinear, 5000, 8);

  digitalWrite(dirPinDrill, LOW);
  digitalWrite(dirPinLinear, LOW);

  Serial.println("Motors Initiated");
}

void sensors_init(void) {
  pinMode(lichtschranke_pin, INPUT);
  pinMode(encA, INPUT);
  pinMode(end_pin, INPUT_PULLUP);
}

void motor_enable(motor_id_t id) {
  if (id == MOTOR_DRILL) {
    enabled_drill = 1;
    Serial.println("Drill enabled");
  }

  if (id == MOTOR_LINEAR) {
    enabled_linear = 1;
    Serial.println("Linear enabled");
  }
  Serial.print("Enabled Motor");
  Serial.println(id);
}

void motor_disable(motor_id_t id) {
  if (id == MOTOR_DRILL) {
    ledcWrite(pwmPinDrill, 0);
    enabled_drill = 0;
  }

  if (id == MOTOR_LINEAR) {
    ledcWrite(pwmPinLinear, 0);
    enabled_linear = 0;
  }
}

void motor_set_speed(motor_id_t id, uint8_t speed) {
  if (id == MOTOR_DRILL) {
    if (enabled_drill == 0) return;
    ledcWrite(pwmPinDrill, speed);
  }

  if (id == MOTOR_LINEAR) {
    if (enabled_linear == 0) return;
    ledcWrite(pwmPinLinear, speed);
  }
  Serial.print("Set motor speed");
  Serial.print(id);
  Serial.println(speed);
}

void motor_set_direction(motor_id_t id, uint8_t dir) {
  if (id == MOTOR_DRILL) {
    digitalWrite(dirPinDrill, (dir == 0) ? LOW : HIGH);
  }

  if (id == MOTOR_LINEAR) {
    digitalWrite(dirPinLinear, (dir == 0) ? HIGH : LOW);
  }

  Serial.print("Set motor direction");
  Serial.print(id);
  Serial.println(dir);
}

