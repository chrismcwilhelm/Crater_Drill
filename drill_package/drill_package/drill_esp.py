import rclpy
from rclpy.node import Node

from drill_interfaces.msg import DrillStatus, DrillCommand
from drill_package.drill_constants import DrillId
from can_msgs.msg import Frame

class DrillEsp(Node):
    def __init__(self):
        super().__init__("drill_esp")
        self.get_logger().info("init drill esp")

        self.state = DrillStatus.IDLE
        self.distance = 100.42

        self.unloading_time = 30

        self.can_sub = self.create_subscription(Frame, "/can_tx", self.rx_task, 10)
        self.can_pub = self.create_publisher(Frame, "/can_rx", 10)

        self.status_timer = self.create_timer(1, self.status_task)

    def rx_task(self, msg):
        self.get_logger().info(f"received message {msg.data} from id {msg.id}")

        if msg.id == DrillId.DRILL_COMMAND:
            if msg.data[0] == DrillCommand.START:
                self.state = DrillStatus.DRILLING
            
            if msg.data[0] == DrillCommand.STOP:
                self.state = DrillStatus.STOPPED
                frame = Frame()
                frame.id = DrillId.SAMPLE_COMMAND
                frame.dlc = 1
                frame.data = [1]
                self.can_pub.publish(frame)


            if msg.data[0] == DrillCommand.RETRACT:
                self.state = DrillStatus.RETRACTING
                frame = Frame()
                frame.id = DrillId.SAMPLE_COMMAND
                frame.dlc = 1
                frame.data = [2]
                self.can_pub.publish(frame)

    def status_task(self):
        self.get_logger().info("Sending status")
        self.state_handler()

        frame = Frame()
        frame.id = DrillId.DRILL_STATUS
        frame.dlc = 2
        frame.data[0] = self.state
        frame.data[1] = self.distance

        self.can_pub.publish(frame)

    def state_handler(self):
        self.get_logger().info(f"handling state {self.state}")
        if self.state == DrillStatus.DRILLING:
            if self.distance >= 20:
                self.distance -= 1
            else:
                self.state = DrillStatus.RETRACTING

        if self.state == DrillStatus.RETRACTING:
            if self.distance <= 100:
                self.distance += 1
            else:
                self.state = DrillStatus.UNLOADING
                frame = Frame()
                frame.id = DrillId.SAMPLE_COMMAND
                frame.dlc = 1
                frame.data = [0]
                self.can_pub.publish(frame)

        if self.state == DrillStatus.UNLOADING:
            if self.unloading_time > 0:
                self.unloading_time -= 1
            else:
                self.state = DrillStatus.DONE

def main(args=None):
    rclpy.init(args=args)

    drill_esp = DrillEsp()

    rclpy.spin(drill_esp)

    drill_esp.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()