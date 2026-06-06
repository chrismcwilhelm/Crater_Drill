#pragma once
#include "crater_can.h"

typedef enum {
  CMD_ENABLE = 0x00,
  CMD_DISABLE = 0x01,
  CMD_SET_SPEED = 0x02,
  CMD_SET_DIRECTION = 0x03,
  CMD_SPEED_CONTROL = 0x04
} motor_cmd_t;


typedef struct {
  uint8_t motor_id;
  uint8_t command;
  uint8_t direction;
  uint8_t speed;
} motor_packet_t;

int parse_motor_frame(const can_frame_t *frame, motor_packet_t *out);