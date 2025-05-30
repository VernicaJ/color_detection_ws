#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

class ColorFilter : public rclcpp::Node
{
public:
  ColorFilter();

private:
  // Define Publisher, Subscriber
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub;
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_pub;

  // Parameters
  std::string image_sub_topic_, image_pub_topic_;
  int lower_h_, lower_s_, lower_v_;
  int upper_h_, upper_s_, upper_v_;
  int set_h_, set_s_, set_v_;
  bool show_images_;

  // Subscriber callback
  void image_callback(const sensor_msgs::msg::Image::SharedPtr msg);
  void show_resized(
    const std::string & window_name,
    const cv::Mat & image,
    int width = 420,
    int height = 310);
};
