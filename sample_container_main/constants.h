#pragma once

#define SERVO_PIN 21

#define RX_PIN 4
#define TX_PIN 5

#define DT1 18
#define SCK1 19

#define DT2 25
#define SCK2 26

typedef enum {
  DRILL_CMD = 0x200,
  DRILL_STATUS = 0x201,
  SAMPLE_CMD = 0x300,
  SAMPLE_STATUS = 0x301
} can_ids_t;

typedef enum {
  UNLOAD = 0x03,
  CONFIRM_DONE = 0x04
} command_t;