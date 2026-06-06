from enum import IntEnum

class DRILLID(IntEnum):
    TO_DRILL = 0x200
    FROM_DRILL = 0x201
    TO_SAMPLE = 0x300
    FROM_SAMPLE = 0x301

class MOTORID(IntEnum):
    DRILL_MOTOR = 0x00
    LINEAR_MOTOR = 0x01
    SAMPLE_MOTOR = 0x02

class MotorCommand(IntEnum):
    ENABLE = 0x00
    DISABLE = 0x01
    SET_SPEED = 0x02
    SET_DIRECTION = 0x03
    MOTOR_CONTROL = 0x04

class Direction(IntEnum):
    FORWARD = 0x00
    REVERSE = 0x01

class Data(IntEnum):
    DATA_COMMAND = 0x05
    ULTRASONIC = 0x00
    SCALE = 0x01