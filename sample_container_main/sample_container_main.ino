#include <ESP32Servo.h>
#include "crater_can.h"
#include "constants.h"
#include "HX711.h"

Servo servo;

HX711 scale1;
HX711 scale2;

float weight = 0.0;

const float SCALE_FACTOR = -0.975;

void update_weight() {
  float weight1 = scale1.get_units(20);
  float weight2 = scale2.get_units(20);

  weight = (weight1 + weight2) * SCALE_FACTOR;
}

void extend() {
  servo.write(130);
  delay(1000);
  servo.write(90);

  delay(500);

  scale1.tare();
  scale2.tare();

  delay(500);

  can_frame_t frame = {
        .identifier = DRILL_CMD,
        .data_length_code = 1,
        .data = {UNLOAD},
        .is_rtr = false
  };

  if (crater_can_transmit(&frame, 100) == CAN_OK) {
            Serial.println("Sent start unload.");
    } else {
            Serial.println("Send failed.");
    }
}

void retract() {
  servo.write(50);
  delay(1000);
  servo.write(90);

  can_frame_t frame = {
        .identifier = DRILL_CMD,
        .data_length_code = 1,
        .data = {CONFIRM_DONE},
        .is_rtr = false
  };

  if (crater_can_transmit(&frame, 100) == CAN_OK) {
            Serial.println("Sent finish unload.");
    } else {
            Serial.println("Send failed.");
    }
}

void listen(void *pvParameters) {
  can_frame_t frame;

  while (1) {
      if (crater_can_receive(&frame, 10) == CAN_OK) {
        Serial.println(frame.identifier);
          if (frame.identifier == SAMPLE_CMD) {
            switch(frame.data[0]){
              case(UNLOAD):
                extend();
                break;

              case(CONFIRM_DONE):
                retract();
                break;
            }
          }
      }
      vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void send_status(void *pvParameters) {
  while (1) {
    update_weight();
    uint16_t weight_encoded = (uint16_t)(weight * 100.0f);

    can_frame_t frame = {
        .identifier = SAMPLE_STATUS,
        .data_length_code = 2,
        .data = {
          (uint8_t)(weight_encoded >> 8),
          (uint8_t)(weight_encoded & 0xFF)
        },
        .is_rtr = false
    };

    if (crater_can_transmit(&frame, 100) == CAN_OK) {
            Serial.println("Status sent.");
    } else {
            Serial.println("Status failed.");
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {
  Serial.begin(115200);

  servo.attach(SERVO_PIN);

  scale1.begin(DT1, SCK1);
  scale2.begin(DT2, SCK2);

  scale1.set_scale(1000);
  scale2.set_scale(1000);

  scale1.tare();
  scale2.tare();

  if (crater_can_init(TX_PIN, RX_PIN) != CAN_OK) {
      Serial.println("Failed to initialize CAN hardware.");
      return;
  }

  xTaskCreate(listen, "Listen", 3072, NULL, 5, NULL);
  xTaskCreate(send_status, "Send Status", 3072, NULL, 5, NULL);
}

void loop() {}
