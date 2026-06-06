import rclpy
from rclpy.node import Node

from std_msgs.msg import String

class Follower(Node):
    def __init__(self):
        super().__init__("follower")
        self.get_logger().info("Initilizing follower")

        self.follower = self.create_subscription(String, "/example_topic", self.run, 10)

        
    def run(self, msg):
        self.get_logger().info(f"msg received: {msg.data}")

def main(args=None):
    rclpy.init(args=args)

    follwer = Follower()

    rclpy.spin(follwer)

    follwer.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()