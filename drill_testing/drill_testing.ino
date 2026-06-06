#include "drill_constants.h"
#include "drill_protocol.h"
#include "motor_control.h"

unsigned long lastEndschalter = 0;
unsigned long lastControlTime = 0;

volatile long pulses_Lin = 0;
volatile long pulses_Drill = 0;

float lin_target_rpm = 0.0;
int pwm_Lin = 255;

volatile bool emergencyInterrupt = false;

volatile bool speedControl = false;

void handle_motor_command(const motor_packet_t * p) {
  motor_id_t id = (motor_id_t)p->motor_id;
  Serial.println(id);

  switch (p->command) {
    case CMD_ENABLE:
      motor_enable(id);
      Serial.println("motor_enable");
      break;
    case CMD_DISABLE:
      motor_disable(id);
      Serial.println("motor_disable");
      break;
    case CMD_SET_SPEED:
      motor_set_speed(id, p->speed);
      Serial.println("set_speed");
      break;
    case CMD_SET_DIRECTION:
      motor_set_direction(id, p->direction);
      Serial.println("set_direction");
      break;
    case CMD_SPEED_CONTROL:
      if (p->direction == 0) {
        speedControl = true;
        xTaskCreatePinnedToCore(speed_control, "Speed Control", 4096, NULL, 4, NULL, 0);
      }
      if (p->direction == 1) {
        speedControl = false;
      }
      
      break;
  }
}

void speed_control(void *pvParameters) {
  while (speedControl) {
    unsigned long now = millis();

    if (now - lastControlTime >= 500) {
      noInterrupts();
      long p_D = pulses_Drill;
      pulses_Drill = 0;

      long p_L = pulses_Lin;
      pulses_Lin = 0;
      interrupts();

      float drill_rpm = (p_D / (float)PPR_Drill) * (60000.0 / 500.0);

      lin_target_rpm = (drill_rpm / (60.0 * drill_to_linear) * 20.0);

      float lin_current_rpm = (p_L / float(PPR_Lin)) * (60000.0 / 500.0);

      float error = lin_target_rpm - lin_current_rpm;

      pwm_Lin += error * 0.5;
      if (pwm_Lin > pwmMax) pwm_Lin = pwmMax;
      if (pwm_Lin < pwmMin) pwm_Lin = pwmMin;
      
      ledcWrite(pwmPinLinear, pwm_Lin);

      lastControlTime = now;
    }
    vTaskDelay(1000);
  }
}

void can_rx_task(void *pvParameters) {
  can_frame_t frame;
  motor_packet_t packet;

  while (1) {
      if (crater_can_receive(&frame, 10) == CAN_OK) {
        Serial.println(frame.identifier);
          if (frame.identifier == TO_DRILL) {
              parse_motor_frame(&frame, &packet);
              for (int i = 0; i < frame.data_length_code; i++) {
                Serial.println(frame.data[i]);
              }
              
              handle_motor_command(&packet);
          }
      }
      delay(5);
  }
}

void IRAM_ATTR endschalter() {
  unsigned long now = millis();

  if (now - lastEndschalter > 100) {
    motor_disable(MOTOR_DRILL);
    motor_disable(MOTOR_LINEAR);
  }
}

void IRAM_ATTR countPulseDrill() {
  pulses_Drill++;
}

void IRAM_ATTR countPulseLin() {
  pulses_Lin++;
}

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("test1");
  motor_init();

  sensors_init();
  attachInterrupt(digitalPinToInterrupt(encA), countPulseLin, RISING);
  attachInterrupt(digitalPinToInterrupt(lichtschranke_pin), countPulseDrill, FALLING);
  attachInterrupt(digitalPinToInterrupt(end_pin), endschalter, RISING);

  lastEndschalter = millis();
  
  Serial.println("Initiated");
  if (crater_can_init(TX_PIN, RX_PIN) != CAN_OK) {
      Serial.println("Failed to initialize CAN hardware.");
      return;
  }

  xTaskCreatePinnedToCore(can_rx_task, "CAN_RX", 4096, NULL, 2, NULL, 1);
}

void loop() {}