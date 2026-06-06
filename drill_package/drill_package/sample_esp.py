import rclpy
from rclpy.node import Node

from can_msgs.msg import Frame

from drill_package.drill_constants import DrillId

class SampleEsp(Node):
    def __init__(self):
        super().__init__("sample_esp")
        self.get_logger().info("init sample esp")

        self.weight = 12.36

        self.can_pub = self.create_publisher(Frame, "/can_rx", 10)

        self.status_timer = self.create_timer(1, self.status_task)

    def status_task(self):
        self.get_logger().info("Sending status")

        frame = Frame()
        frame.id = DrillId.SAMPLE_STATUS
        frame.dlc = 1
        w = int(self.weight)
        frame.data[0] = w
        frame.data[1] = abs(w-self.weight)*100

        self.can_pub.publish(frame)

def main(args=None):
    rclpy.init(args=args)

    sample_esp = SampleEsp()

    rclpy.spin(sample_esp)

    sample_esp.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()