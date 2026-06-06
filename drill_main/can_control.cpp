#include "can_control.h"
#include "crater_can.h"
#include "sensor_control.h"
#include "Arduino.h"

volatile status_t global_status = IDLE;
volatile status_t local_status = IDLE;

volatile float sample_weight = 0.0;

void can_listen(void *pvParameters) {
  while (1) {
    can_frame_t frame;
    if (crater_can_receive(&frame, 10) == CAN_OK) {
      Serial.println(frame.identifier);
      Serial.println(frame.data[0]);
      if (frame.identifier == DRILL_CMD) {
        switch(frame.data[0]) {
          case START:
            global_status = DRILLING;
            local_status = DRILLING;
            break;

          case STOP:
            global_status = STOPPED;
            local_status = STOPPED;
            break;

          case RETRACT:
            global_status = RETRACTING;
            local_status = RETRACTING;
            break;

          case UNLOAD:
            local_status = UNLOADING;
            break;

          case CONFIRM_DONE:
            global_status = DONE;
            local_status = DONE;
            break;
        }
      }

      if (frame.identifier == SAMPLE_STATUS) {
        uint16_t weight_encoded = ((uint16_t)frame.data[0] << 8) | frame.data[1];
        sample_weight = weight_encoded / 100.0f;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void can_send_status(void *pvParameters) {
  while (1) {
    uint8_t distance = (uint8_t) get_distance();

    can_frame_t frame = {
        .identifier = DRILL_STATUS,
        .data_length_code = 2,
        .data = {global_status, distance},
        .is_rtr = false
    };

    if (crater_can_transmit(&frame, 100) == CAN_OK) {
            //Serial.println("Status sent.");
    } else {
            Serial.println("Status failed.");
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void can_init(void) {
    if (crater_can_init(TX_PIN, RX_PIN) != CAN_OK) {
      Serial.println("Failed to initialize CAN hardware.");
      return;
  }

  xTaskCreate(can_send_status, "CAN send status", 3072, NULL, 5, NULL);
  xTaskCreate(can_listen, "CAN listen", 3072, NULL, 5, NULL);
}

float get_weight() {
  return sample_weight;
}

status_t get_local_status() {
  return local_status;
}

void set_status(status_t global, status_t local) {
  global_status = global;
  local_status = local;
}

void send_start_unload() {
  can_frame_t frame = {
        .identifier = SAMPLE_CMD,
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

void send_finish_unload() {
  can_frame_t frame = {
        .identifier = SAMPLE_CMD,
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