import rclpy
from rclpy.node import Node

from drill_interfaces.msg import DrillStatus, DrillCommand
from can_msgs.msg import Frame

class Listener(Node):
    def __init__(self):
        super().__init__("test_controller")
        self.get_logger().info("init listener")
        self.sub_status = self.create_subscription(DrillStatus, "/drill/status", self.handle_status, 10)
        self.sub_command = self.create_subscription(DrillCommand, "/drill/command", self.handle_command, 10)
        self.sub_rx = self.create_subscription(Frame, "/can_rx", self.handle_rx, 10)
        self.sub_tx = self.create_subscription(Frame, "/can_tx", self.handle_tx, 10)
        
    def handle_status(self, msg):
        self.get_logger().info(f"Received Status: {msg.status} with distance: {msg.distance} and weight: {msg.weight}. Drill online is {msg.drillonline}, Sample online is {msg.sampleonline}")

    def handle_command(self, msg):
        self.get_logger().info(f"Drill Command: {msg.command}")

    def handle_rx(self, frame):
        self.get_logger().info(f"Can RX: {frame.id}, {frame.data[0]}")

    def handle_tx(self, frame):
        self.get_logger().info(f"Can TX: {frame.id}, {frame.data[0]}")

def main(args=None):
    rclpy.init(args=args)

    listener = Listener()

    rclpy.spin(listener)

    listener.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()