#include <ESP32Servo.h>
#include <crater_can.h>

#define SERVO_PIN 21

#define RX_PIN 4
#define TX_PIN 5

#define MOTOR 0x02
#define SCALE 0x05

#define TO_SAMPLE 0x300
#define FROM_SAMPLE 0x301

Servo servo;



void sendWeight() {
  can_frame_t frame;
  if (crater_can_receive(&frame, 10) == CAN_OK) {
    Serial.print("Still waiting");
    Serial.print(frame.identifier);
    Serial.println(frame.data[0]);
  }

  int weight = 100;
  can_frame_t weight_frame = {
    .identifier = 0x301,
    .data_length_code = 1,
    .data = {weight},
    .is_rtr = false
};

  if (crater_can_transmit(&weight_frame, 100) == CAN_OK) {
      Serial.println("Weight sent");
  } else {
      Serial.println("Weight failed");
  }
}

void can_rx_task(void *pvParameters) {
  can_frame_t frame;


  while (1) {
      if (crater_can_receive(&frame, 10) == CAN_OK) {
        Serial.println(frame.identifier);
          if (frame.identifier == TO_SAMPLE) {
            if (frame.data[0] == MOTOR) {
              servo.write(frame.data[1]);
            }
            if (frame.data[0] == SCALE) {
              sendWeight();
            }
            
          }
          if (crater_can_transmit(&frame, 100) == CAN_OK) {
            Serial.println("Echo sent.");
        } else {
            Serial.println("Echo transmit failed.");
        }
      }
      delay(5);
  }
}

void setup() {
  servo.attach(SERVO_PIN);

  Serial.begin(115200);

  if (crater_can_init(TX_PIN, RX_PIN) != CAN_OK) {
      Serial.println("Failed to initialize CAN hardware.");
      return;
  }

  xTaskCreatePinnedToCore(can_rx_task, "CAN_RX", 4096, NULL, 2, NULL, 1);
}

void loop() {
  //sendWeight();

  //delay(500);
}
