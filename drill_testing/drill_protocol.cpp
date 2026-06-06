#include "drill_protocol.h"

int parse_motor_frame(const can_frame_t *frame, motor_packet_t * out) {
  if (!frame || !out) return -1;

  out->motor_id = frame->data[0];
  out->command = frame->data[1];
  out->direction = frame->data[2];
  out->speed = frame->data[3];

  return 0;
}