#include "sensor_control.h"
#include "can_control.h"
#include "motor_control.h"
#include "Arduino.h"

volatile long pulses_linear = 0;
volatile long pulses_drill = 0;
volatile float distance = 0;

void IRAM_ATTR endschalter() {
  //set_speed(MOTOR_DRILL, 0);
  //set_speed(MOTOR_LINEAR, 0);
  Serial.println("endschalter gedrückt");

  //set_status(STOPPED, STOPPED);
}

void IRAM_ATTR countPulsesLinear() {
  pulses_linear++;
}

void IRAM_ATTR countPulsesDrill() {
  pulses_drill++;
}

void measure_distance(void *pvParameters) {
  while (1) {
    digitalWrite(trigger_pin, LOW);
    vTaskDelay(pdMS_TO_TICKS(2));

    digitalWrite(trigger_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger_pin, LOW);

    long duration = pulseIn(echo_pin, HIGH, 30000);

    distance = duration * 0.0343 / 2;

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void sensor_init(void) {
  pinMode(lichtschranke_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(lichtschranke_pin), countPulsesDrill, FALLING);

  pinMode(encoder_A, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder_A), countPulsesLinear, RISING);

  pinMode(endschalter_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(endschalter_pin), endschalter, FALLING);

  pinMode(trigger_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

  xTaskCreate(measure_distance, "Distance Measurement", 3072, NULL, 5, NULL);
}

volatile long get_pulses_drill() {
  return pulses_drill;
}

volatile long get_pulses_linear() {
  return pulses_linear;
}

void reset_pulses() {
  pulses_drill = 0;
  pulses_linear = 0;
}

float get_distance() {
  return distance;
}