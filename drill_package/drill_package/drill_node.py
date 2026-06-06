import rclpy
from rclpy.lifecycle import LifecycleNode, TransitionCallbackReturn

from can_msgs.msg import Frame

from drill_package.drill_logic import DrillLogic
from drill_interfaces.msg import DrillCommand, DrillStatus

class DrillNode(LifecycleNode):
    def __init__(self):
        super().__init__("drill_node")

        self.can_pub = None
        self.can_sub = None

        self.command_sub = None
        self.status_pub = None
        self.status_timeout = 1

    def on_configure(self, state):
        self.get_logger().info("Configuring drill node")

        self.can_pub = self.create_publisher(Frame, "/can_tx", 10)
        self.status_pub = self.create_publisher(DrillStatus, "/drill/status", 10)
        self.get_logger().info("test1")
        self.logic = DrillLogic(logger=self.get_logger(),
                                can_pub=self.can_pub, 
                                status_pub=self.status_pub)
        
        self.get_logger().info("test2")
        self.command_sub = self.create_subscription(DrillCommand, "/drill/command", self.logic.command_callback, 10)
        self.can_sub = self.create_subscription(Frame, "/can_rx", self.logic.can_callback, 10)
        self.status_timer = self.create_timer(self.status_timeout, self.logic.send_status)

        return TransitionCallbackReturn.SUCCESS
    
    def on_activate(self, state):
        self.get_logger().info("Nothing implemented on activate")
        return TransitionCallbackReturn.SUCCESS
    
    def on_deactivate(self, state):
        self.get_logger().info("Nothing implemented on deactivate")
        return TransitionCallbackReturn.SUCCESS
    
    def on_cleanup(self, state):
        self.get_logger().info("Nothing implemented on cleanup")
        return TransitionCallbackReturn.SUCCESS
    
    def on_shutdown(self, state):
        self.get_logger().info("Nothing implemented on shutdown")
        return TransitionCallbackReturn.SUCCESS
    
def main(args=None):
    rclpy.init(args=args)

    node = DrillNode()
    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()