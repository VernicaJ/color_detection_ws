#include <blue/color_filter.hpp>

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  try {
    rclcpp::spin(std::make_shared<ColorFilter>());
  } catch (const std::runtime_error & e) {
    RCLCPP_ERROR(rclcpp::get_logger("color_filter"), "Exception caught: %s", e.what());
  }

  rclcpp::shutdown();
  return 0;
}
