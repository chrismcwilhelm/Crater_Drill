import rclpy
from rclpy.node import Node

from std_msgs.msg import String

class Publisher(Node):
    def __init__(self):
        super().__init__("publisher")
        self.get_logger().info("Initilizing Publisher")

        self.publisher = self.create_publisher(String, "/example_topic", 10)

        self.publish_timer = self.create_timer(3, self.publish_test)

    def publish_test(self):
        self.get_logger().info("publishing")
        msg = String()
        msg.data = "Test rocky rocks"
        self.publisher.publish(msg)

def main(args=None):
    rclpy.init(args=args)

    publisher = Publisher()

    rclpy.spin(publisher)

    publisher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()