#pragma once
#include <stdint.h>

//CAN Pins
#define RX_PIN 4
#define TX_PIN 5

typedef enum {
  DRILL_CMD = 0x200,
  DRILL_STATUS = 0x201,
  SAMPLE_CMD = 0x300,
  SAMPLE_STATUS = 0x301
} can_ids_t;

typedef enum {
  START = 0x00,
  STOP = 0x01,
  RETRACT = 0x02,
  UNLOAD = 0x03,
  CONFIRM_DONE = 0x04
} command_t;

typedef enum {
  IDLE = 0x00,
  TAKING_PHOTO = 0x01,
  DRILLING = 0x02,
  RETRACTING = 0x03,
  UNLOADING = 0x04,
  DONE = 0x05,
  STOPPED = 0x06
} status_t;

void can_init(void);

void can_listen(void *pvParameters);

void can_send(void *pvParameters);

float get_weight();

status_t get_local_status();

void set_status(status_t global, status_t local);

void send_start_unload();

void send_finish_unload();