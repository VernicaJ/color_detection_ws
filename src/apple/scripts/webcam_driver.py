#!/usr/bin/env python3
import time
import cv2
import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, QoSReliabilityPolicy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

class WebcamDriver(Node):

    def __init__(self) -> None:
        # Initialize base class
        super().__init__("Webcam_driver")

        # Declare parameters
        self.declare_parameter("camera_index", 0)
        self.declare_parameter("fps", 30)

        # Get parameters
        self.camera_index = self.get_parameter("camera_index").get_parameter_value().integer_value
        self.fps = self.get_parameter("fps").get_parameter_value().integer_value

        # Create publisher
        self.get_logger().info('Initializing Webcam Driver', once=True)
        self.image_pub = self.create_publisher(Image,
                                               "/webcam/image",
                                               QoSProfile(reliability=QoSReliabilityPolicy.BEST_EFFORT, depth=10))
        # Create a timer
        self.timer_period = 1.0/self.fps
        self.timer = self.create_timer(self.timer_period, self.publish_image)

        self.cv_bridge = CvBridge()
        # Initialize video capture
        self.cap = cv2.VideoCapture(self.camera_index)
        if not self.cap.isOpened():
            self.get_logger().error('Can not open webcam')
            self.destroy_node()
            return

    def publish_image(self) -> None:
        """
        Image publisher triggered by the timer

        """
        # Capture frame-by-frame
        ret, frame = self.cap.read()

        # if frame is read correctly ret is True
        if not ret:
            self.get_logger().error("Can't receive frame. Exiting ...")
            return
        # Get the current date time
        now_time = str(int(time.time()))
        cv2.putText(frame,
                    now_time,
                    (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    1,
                    (255, 255, 244),
                    2,
                    cv2.LINE_AA)
        try:
            # Convert CV2 image to ROS2
            image = self.cv_bridge.cv2_to_imgmsg(frame, encoding='bgr8')
        except CvBridgeError as e:
            self.get_logger().error(
                f'Error converting or publishing image: {e}')

        self.get_logger().info('Publishing video frame')
        self.image_pub.publish(image)

    def shutdown_node(self):
        self.get_logger().info('Releasing Webcam')
        if self.cap and self.cap.isOpened():
            self.cap.release()
        self.destroy_node()


if __name__ == "__main__":
    # Initialize ROS2 context
    rclpy.init()
    # Instantiate the WebcamDriver node
    node = WebcamDriver()
    try:
        # Use ROS2 built-in schedular for executing the node
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    finally:
        node.shutdown_node()
        rclpy.shutdown()
