import time
from drill_can_manager import CANManager
from drill_constants import *
import argparse

MOTOR_MAP = {"drill": MOTORID.DRILL_MOTOR,
                 "linear": MOTORID.LINEAR_MOTOR,
                 "servo": MOTORID.SAMPLE_MOTOR
                 }


def main(port):
    can = CANManager(port)

    while True:
        
        raw = input("> ")
        cmd = raw.split()

        if len(cmd) == 0:
            continue

        if cmd[0] == "exit":
            break

        try:
            command = cmd[0]
            if len(cmd) != 1:
                motor = MOTOR_MAP[cmd[1]]

            if command == "enable":
                can.send_motor_command(DRILLID.TO_DRILL, motor, MotorCommand.ENABLE)

            elif command == "disable":
                can.send_motor_command(DRILLID.TO_DRILL, motor, MotorCommand.DISABLE)

            elif command == "forward":
                can.send_motor_command(DRILLID.TO_DRILL, motor, MotorCommand.SET_DIRECTION, direction = Direction.FORWARD)

            elif command == "reverse":
                can.send_motor_command(DRILLID.TO_DRILL, motor, MotorCommand.SET_DIRECTION, direction = Direction.REVERSE)
            elif command == "speed":
                speed = int(cmd[2])
                if motor == MOTORID.SAMPLE_MOTOR:
                    can.send_sample_command(DRILLID.TO_SAMPLE, MotorCommand.SET_SPEED, speed)
                else:
                    can.send_motor_command(DRILLID.TO_DRILL, motor, MotorCommand.SET_SPEED, speed = speed)
            elif command == "speed_control_on":
                can.send_motor_command(DRILLID.TO_DRILL, motor, MotorCommand.MOTOR_CONTROL, direction=Direction.FORWARD)
            elif command == "speed_control_off":
                can.send_motor_command(DRILLID.TO_DRILL, motor, MotorCommand.MOTOR_CONTROL, direction = Direction.REVERSE)
            elif command == "read_weight":
                can.send_sample_command(DRILLID.TO_DRILL, Data.DATA_COMMAND)
            else:
                print("Unkown Command")
        except Exception as e:
            print("Exception")
            print(e)

        


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", type=str, default="COM9")
    port = parser.parse_args().port

    main(port)