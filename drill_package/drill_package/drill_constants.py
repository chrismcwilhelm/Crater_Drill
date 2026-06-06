from enum import IntEnum

class DrillId(IntEnum):
    DRILL_COMMAND = 0x200
    DRILL_STATUS = 0x201
    SAMPLE_COMMAND = 0x300
    SAMPLE_STATUS = 0x301