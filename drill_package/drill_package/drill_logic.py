from drill_interfaces.msg import DrillCommand, DrillStatus
from can_msgs.msg import Frame
from drill_package.drill_constants import DrillId

import time
#import serial
#import base64
#from datetime import datetime

class DrillLogic():
    def __init__(self, logger, can_pub, status_pub):
        self.log = logger
        self.log.info("Initializing Drill logic")

        self.state = DrillStatus.IDLE

        self.distance = 0.0
        self.weight = 0.0

        self.drillonline = False
        self.sampleonline = False
        self.log.info("test_1")
        self.last_drill_status = time.time()
        self.last_sample_status = time.time()
        self.log.info("test_2")
        self.can_pub = can_pub
        self.status_pub = status_pub
        self.log.info("test_3")
        #self.cam_serial = serial.Serial("/dev/ttyUSB0", 115200, timeout=10)


    def command_callback(self, msg):
        self.log.info(f"Received {msg.command}")
        frame = Frame()
        frame.id = DrillId.DRILL_COMMAND
        frame.dlc = 1
        frame.data = [msg.command]

        if msg.command == DrillCommand.START and self.state == DrillStatus.IDLE:
            #self.save_photo("before")
            self.can_pub.publish(frame)
            self.log.info(f"Sent frame {frame.id}, {frame.data}")

        if msg.command == DrillCommand.STOP:
            self.can_pub.publish(frame)
            self.log.info(f"Sent frame {frame.id}, {frame.data}")

        if msg.command == DrillCommand.RETRACT and self.state == DrillStatus.STOPPED:
            self.can_pub.publish(frame)
            self.log.info(f"Sent frame {frame.id}, {frame.data}")


    def can_callback(self, msg):
        self.log.info(f"Received {msg.data} from id {msg.id}")
        if msg.id == DrillId.DRILL_STATUS:
            #if int(msg.data[0]) == DrillStatus.DONE and self.state != DrillStatus.DONE:
                #self.save_photo("after")

            self.state = int(msg.data[0])
            self.distance = float(msg.data[1])
            self.drillonline = True
            self.last_drill_status = time.time()

        if msg.id == DrillId.SAMPLE_STATUS:
            weight_encoded = (msg.data[0] << 8) | msg.data[1]
            self.weight = weight_encoded / 100.0
            self.sampleonline = True
            self.last_sample_status = time.time()

            

    def send_status(self):
        self.log.info("Sending status")
        curr_time = time.time()

        if curr_time - self.last_drill_status > 3:
            self.drillonline = False
            self.log.info(f"Drill went offline {curr_time - self.last_drill_status} seconds ago.")
        
        if curr_time - self.last_sample_status > 3:
            self.sampleonline = False
            self.log.info(f"Sample Container went offline {curr_time - self.last_drill_status} seconds ago.")
        
        curr_state = DrillStatus()
        curr_state.status = self.state
        curr_state.distance = self.distance
        curr_state.weight = self.weight
        curr_state.drillonline = self.drillonline
        curr_state.sampleonline = self.sampleonline

        self.status_pub.publish(curr_state)

    # def take_photo(self, tag: str):
    #     self.cam_serial.write(b"s")

    #     collecting = False
    #     encoded = ""

    #     while True:
    #         line = self.cam_serial.readline().decode(errors="ignore").strip()

    #         if "Photo Start" in line:
    #             collecting = True
    #             continue

    #         if "Photo End" in line:
    #             break

    #         if collecting:
    #             encoded += line

    #     image_bytes = base64.b64decode(encoded)

    #     timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    #     filename = f"drill_photo_{tag}_{timestamp}.jpg"

    #     with open(filename, "wb") as f:
    #         f.write(image_bytes)

    #     self.log.info("Saved image")