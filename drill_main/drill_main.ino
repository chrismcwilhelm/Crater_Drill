#include <Arduino.h>
#include "motor_control.h"
#include "sensor_control.h"
#include "can_control.h"

float max_distance = 0.0;
status_t last_status = IDLE;

const float DEFAULT_MAX_DISTANCE = 100.0;
const float MIN_WEIGHT = 130.0;
const float TOP_OFFSET = 3.0;
const float BOTTOM_OFFSET = 30.0;

TaskHandle_t speedHandle = NULL;

void drill_logic(void *pvParameters) {
  while (1) {
    status_t status = get_local_status();

    if (last_status != status) {
      handle_status(status);
      Serial.println("Handle Status called");
      last_status = status;
    }

    if (status == DRILLING && get_distance() < 20.0) {
      set_status(RETRACTING, RETRACTING);
    }

    if (status == RETRACTING && get_distance() < 10.0) {
      set_status(UNLOADING, UNLOADING);
      send_start_unload();
    }

    if (status == UNLOADING && get_weight() > MIN_WEIGHT) {
      set_status(UNLOADING, DONE);
      send_finish_unload();
    }
    
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void handle_status(status_t status) {
  switch(status) {
      case IDLE:
      case DONE:
      case STOPPED:
        set_speed(MOTOR_DRILL, PWMMIN);
        set_speed(MOTOR_LINEAR, PWMMIN);
        if (speedHandle != NULL) {
          vTaskDelete(speedHandle);
          speedHandle = NULL;
        }
        break;

      case DRILLING:
        set_direction(MOTOR_DRILL, HIGH);
        set_direction(MOTOR_LINEAR, HIGH);
        set_speed(MOTOR_DRILL, PWMMAX);
        set_speed(MOTOR_LINEAR, PWMMAX);

        if (speedHandle == NULL) {
          xTaskCreate(speed_control, "Speed Control", 3072, NULL, 5, &speedHandle);
        }
        break;

      case RETRACTING:
        set_direction(MOTOR_DRILL, HIGH);
        set_direction(MOTOR_LINEAR, LOW);
        set_speed(MOTOR_DRILL, PWMMAX);
        set_speed(MOTOR_LINEAR, PWMMAX);

        if (speedHandle != NULL) {
          vTaskDelete(speedHandle);
          speedHandle = NULL;
        }
        break;

      case UNLOADING:
        set_direction(MOTOR_DRILL, LOW);
        set_speed(MOTOR_DRILL, PWMMAX);
        set_speed(MOTOR_LINEAR, PWMMIN);
        break;
    }
}

void setup() {
  Serial.begin(115200);
  motor_init();
  sensor_init();
  can_init();

  unsigned long start = millis();

  while (max_distance == 0.0 && millis() - start < 3000) {
   max_distance = get_distance();
  }
  if (max_distance == 0.0) max_distance = DEFAULT_MAX_DISTANCE;

  Serial.print("distance: ");
  Serial.println(get_distance());

  xTaskCreate(drill_logic, "Drill Logic", 3072, NULL, 5, NULL);
}

void loop() {}