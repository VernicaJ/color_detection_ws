#include <blue/color_filter.hpp>

// Constructor
ColorFilter::ColorFilter()
: Node("color_filter")
{
  // Default parameters
  this->declare_parameter<std::string>("image_sub_topic", "/webcam/image");
  this->declare_parameter<std::string>("image_pub_topic", "/webcam/image_filtered");
  this->declare_parameter<int>("lower_h", 100);
  this->declare_parameter<int>("lower_s", 50);
  this->declare_parameter<int>("lower_v", 50);
  this->declare_parameter<int>("upper_h", 130);
  this->declare_parameter<int>("upper_s", 255);
  this->declare_parameter<int>("upper_v", 255);
  this->declare_parameter<int>("set_h", 0);
  this->declare_parameter<int>("set_s", 0);
  this->declare_parameter<int>("set_v", 255);
  this->declare_parameter<bool>("show_images", true);

  // Get Launch file parameters
  this->get_parameter("image_sub_topic", image_sub_topic_);
  this->get_parameter("image_pub_topic", image_pub_topic_);
  this->get_parameter("lower_h", lower_h_);
  this->get_parameter("lower_s", lower_s_);
  this->get_parameter("lower_v", lower_v_);
  this->get_parameter("upper_h", upper_h_);
  this->get_parameter("upper_s", upper_s_);
  this->get_parameter("upper_v", upper_v_);
  this->get_parameter("set_h", set_h_);
  this->get_parameter("set_s", set_s_);
  this->get_parameter("set_v", set_v_);
  this->get_parameter("show_images", show_images_);

  rclcpp::QoS qos_profile =
    rclcpp::QoS(rclcpp::QoSInitialization::from_rmw(rmw_qos_profile_sensor_data)).best_effort();

  image_sub = this->create_subscription<sensor_msgs::msg::Image>(
    image_sub_topic_, qos_profile,
    std::bind(&ColorFilter::image_callback, this, std::placeholders::_1));

  image_pub =
    this->create_publisher<sensor_msgs::msg::Image>(image_pub_topic_, qos_profile);

  RCLCPP_INFO(this->get_logger(), "ColorFilter node has started!");

}

// Resize GUI
void ColorFilter::show_resized(
  const std::string & window_name, const cv::Mat & image, int width,
  int height)
{
  cv::namedWindow(window_name, cv::WINDOW_NORMAL);
  cv::resizeWindow(window_name, width, height);
  cv::imshow(window_name, image);
}

// Subscriber callback
void ColorFilter::image_callback(const sensor_msgs::msg::Image::SharedPtr msg)
{
  // Check for image frames
  if (!msg || msg->data.empty()) {
    RCLCPP_WARN(this->get_logger(), "No image received");
    return;
  }
  cv_bridge::CvImagePtr cv_ptr;
  // Convert ROS 2 image message to OpenCV
  try {
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
  } catch (cv_bridge::Exception & e) {
    RCLCPP_ERROR(this->get_logger(), "CV_bridge exception: %s", e.what());
  }

  cv::Mat image = cv_ptr->image;
  cv::Mat hsv, mask, masked_image, output_image;

  // Convert to HSV
  cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

  // Define mask color range
  cv::Scalar lower(lower_h_, lower_s_, lower_v_);
  cv::Scalar upper(upper_h_, upper_s_, upper_v_);
  cv::inRange(hsv, lower, upper, mask);
  cv::bitwise_and(image, image, masked_image, mask);

  output_image = image.clone();

  // Change mask color
  for (int y = 0; y < image.rows; ++y) {
    for (int x = 0; x < image.cols; ++x) {
      if (mask.at<uchar>(y, x) > 0) {
        output_image.at<cv::Vec3b>(y, x) = cv::Vec3b(set_h_, set_s_, set_v_);
      }
    }
  }
  // Show images
  if (show_images_) {
    show_resized("Original image", image);
    show_resized("Detected mask", masked_image);
    show_resized("Output: Changed mask color", output_image);
    cv::waitKey(1);
  }
  // Convert output image to ROS2 msg
  auto output_msg = cv_bridge::CvImage(
    std_msgs::msg::Header(), "bgr8", output_image).toImageMsg();
  // Publish ROS2 msg
  image_pub->publish(*output_msg);

}
